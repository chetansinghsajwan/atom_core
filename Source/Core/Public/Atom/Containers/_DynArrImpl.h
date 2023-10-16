#pragma once
#include "Atom/Contracts.h"
#include "Atom/Core.h"
#include "Atom/Range/ArrIter.h"

#include <algorithm>

namespace Atom
{
    template <typename T, typename TAlloc>
    class _DynArrImpl
    {
    public:
        using TElem = T;
        using TIter = ArrIter<TElem>;
        using TIterEnd = TIter;
        using TMutIter = MutArrIter<TElem>;
        using TMutIterEnd = TMutIter;

    public:
        constexpr _DynArrImpl():
            _arr{ nullptr }, _count{ 0 }, _capacity{ 0 }, _alloc{}
        {}

        constexpr _DynArrImpl(_DynArrImpl&& that) {}

        template <typename UIter, typename UIterEnd>
        constexpr _DynArrImpl(UIter it, UIterEnd itEnd)
        {}

    public:
        constexpr auto onDestruct()
        {
            removeAll();
            releaseUnusedMem();
        }

        constexpr auto iter(usize i = 0) const -> TIter
        {
            debug_expects(isIndexInRange(i));

            return TIter{ _getData() + i };
        }

        constexpr auto iterEnd() const -> TIterEnd
        {
            return TIterEnd{ _getData() + _getCount() };
        }

        constexpr auto mutIter(usize i = 0) -> TMutIter
        {
            debug_expects(isIndexInRange(i));

            return TMutIter{ _getMutData() + i };
        }

        constexpr auto mutIterEnd() -> TMutIterEnd
        {
            return TMutIterEnd{ _getMutData() + _getCount() };
        }

        template <typename... TArgs>
        constexpr auto emplaceAt(usize i, TArgs&&... args)
        {
            return _emplaceAt(i, fwd(args)...);
        }

        template <typename UIter, typename UIterEnd>
        constexpr auto insertRangeAt(usize i, UIter it, UIterEnd itEnd) -> usize
        {
            if constexpr (_CanGetRangeSize<UIter, UIterEnd>())
            {
                return _insertRangeAtCounted(i, it, _GetRangeSize(it, itEnd));
            }
            else
            {
                return _insertRangeAtUncounted(i, mov(it), mov(itEnd));
            }
        }

        template <typename... TArgs>
        constexpr auto emplaceFront(TArgs&&... args)
        {
            return _emplaceFront(fwd(args)...);
        }

        template <typename UIter, typename UIterEnd>
        constexpr auto insertRangeFront(UIter it, UIterEnd itEnd) -> usize
        {
            return insertRangeAt(0, it, itEnd);
        }

        template <typename... TArgs>
        constexpr auto emplaceBack(TArgs&&... args)
        {
            return _emplaceAt(_getCount() - 1, fwd(args)...);
        }

        template <typename UIter, typename UIterEnd>
        constexpr auto insertRangeBack(UIter it, UIterEnd itEnd) -> usize
        {
            if constexpr (_CanGetRangeSize<UIter, UIterEnd>())
            {
                usize count = _GetRangeSize(it, itEnd);
                _insertRangeBackCounted(it, count);
                return count;
            }
            else
            {
                return _insertRangeBackUncounted(mov(it), mov(itEnd));
            }
        }

        constexpr auto removeAt(usize i)
        {
            _destructAt(i);
            _moveRangeFront(i, 1);
        }

        constexpr auto removeRange(usize begin, usize count)
        {
            _destructRange(begin, count);
            _moveRangeFront(begin, count);
        }

        constexpr auto removeAll()
        {
            _destructRange(0, _getCount() - 1);
            _moveRangeFront(0, _getCount() - 1);
        }

        constexpr auto reserve(usize count)
        {
            return reserveMore(count);
        }

        constexpr auto reserveMore(usize count)
        {
            _ensureCapFor(_getCount() + count);
        }

        constexpr auto releaseUnusedMem() {}

        constexpr auto capacity() const -> usize
        {
            return _getCapacity();
        }

        constexpr auto count() const -> usize
        {
            return _getCount();
        }

        constexpr auto data() const -> const T*
        {
            return _getData();
        }

        constexpr auto mutData() -> T*
        {
            return _getMutData();
        }

        constexpr auto isIndexInRange(usize i) const -> bool
        {
            return i < _getCount();
        }

        constexpr auto isIndexInRangeOrEnd(usize i) const -> bool
        {
            return i <= _getCount();
        }

        constexpr auto isIterValid(TIter it) const -> bool
        {
            return true;
        }

        constexpr auto indexForIter(TIter it) const -> usize
        {
            isize i = it.data() - _getData();
            return i < 0 ? usize(-1) : i;
        }

        constexpr auto isIterInRange(TIter it) const -> bool
        {
            return indexForIter(it) < _getCount();
        }

        constexpr auto isIterInRangeOrEnd(TIter it) const -> bool
        {
            return indexForIter(it) <= _getCount();
        }

        ////////////////////////////////////////////////////////////////////////////////////////////
        ////
        //// Implementations
        ////
        ////////////////////////////////////////////////////////////////////////////////////////////

    private:
        template <typename... TArgs>
        constexpr auto _emplaceAt(usize i, TArgs&&... args) -> usize
        {
            _ensureCapFor(i);
            _moveRangeBack(i, 1);
            _constructAt(i, fwd(args)...);

            return i;
        }

        template <typename UIter>
        constexpr auto _insertRangeAtCounted(usize i, UIter it, usize count) -> usize
        {
            if (count == 0)
                return i;

            _ensureCapFor(count);
            _moveRangeBack(i, count);

            // Insert new elements
            for (usize i = 0; i < count; i++)
            {
                _constructAt(i + i, fwd(it.value()));
                it.next();
            }

            return i;
        }

        template <typename UIter, typename UIterEnd>
        constexpr auto _insertRangeAtUncounted(usize i, UIter it, UIterEnd itEnd) -> usize
        {
            usize rotateSize = _getCount() - i;
            _insertRangeBackUncounted(mov(it), mov(itEnd));
            _rotateRangeBack(i, rotateSize);

            return i;
        }

        template <typename U>
        constexpr auto _insertBack(U&& el)
        {
            _ensureCapFor(1);
            _constructAt(_getCount(), fwd(el));
            _setCount(_getCount() + 1);
        }

        template <typename UIter>
        constexpr auto _insertRangeBackCounted(UIter it, usize count)
        {
            if (count == 0)
                return;

            _ensureCapFor(count);

            usize i = _getCount();
            for (usize j = 0; j < count; j++)
            {
                _constructAt(i + j, fwd(it.value()));
                it.next();
            }

            _setCount(i + count);
        }

        template <typename UIter, typename UIterEnd>
        constexpr auto _insertRangeBackUncounted(UIter it, UIterEnd itEnd) -> usize
        {
            usize i = _getCount();
            usize count = 0;
            while (not it.equals(itEnd))
            {
                _ensureCapFor(1);
                _constructAt(i + count, it.value());

                it.next();
                count++;
            }

            _setCount(i + count);
            return count;
        }

        constexpr auto _updateIterDebugId() {}

        constexpr auto _calcCapGrowth(usize required) const -> usize
        {
            return required;
        }

        constexpr auto _ensureCapFor(usize count)
        {
            _updateIterDebugId();

            // We have enough capacity.
            if (_getCapacity() - _getCount() >= count)
                return;

            usize newCap = _calcCapGrowth(count);
            TElem* newArr = _allocMem(newCap);

            _moveRangeTo(0, newArr);
            _deallocMem(_getMutData());
            _setData(newArr);
            _setCapacity(newCap);
        }

        constexpr auto _constructAt(usize i, auto&&... args) -> void
        {
            T* src = _getMutData() + i;
            std::construct_at(src, fwd(args)...);
        }

        constexpr auto _destructAt(usize i) -> void
        {
            T* src = _getMutData() + i;
            std::destroy_at(src);
        }

        constexpr auto _destructRange(usize i, usize count) -> void
        {
            T* begin = _getMutData() + i;
            T* end = begin + count;
            std::destroy(begin, end);
        }

        constexpr auto _moveRangeFront(usize i, usize count) -> void
        {
            T* begin = _getMutData() + i;
            T* end = _getMutData() + _getCount() - 1;
            T* dest = begin - count;
            std::move(begin, end, dest);
        }

        constexpr auto _moveRangeBack(usize i, usize count) -> void
        {
            T* begin = _getMutData() + i;
            T* end = _getMutData() + _getCount() - 1;
            T* dest = begin + count;
            std::move_backward(begin, end, dest);
        }

        constexpr auto _moveRangeTo(usize i, TElem* dest) -> void
        {
            T* begin = _getMutData() + i;
            T* end = _getMutData() + _getCount() - 1;
            std::move_backward(begin, end, dest);
        }

        constexpr auto _rotateRangeBack(usize i, usize count) -> void
        {
            T* begin = _getMutData();
            T* mid = begin + i;
            T* end = begin + _getCount() - 1;
            std::rotate(begin, mid, end);
        }

        template <typename UIter, typename UIterEnd>
        static constexpr auto _CanGetRangeSize() -> bool
        {
            return RFwdIterPair<UIter, UIterEnd>;
        }

        template <typename UIter, typename UIterEnd>
        static constexpr auto _GetRangeSize(UIter it, UIterEnd itEnd) -> usize
        {
            if constexpr (RJumpIterPair<UIter, UIterEnd>)
            {
                return -it.compare(itEnd);
            }

            usize count = 0;
            for (; it.equals(itEnd); it.next())
                count++;

            return count;
        }

        constexpr auto _getData() const -> const T*
        {
            return _arr;
        }

        constexpr auto _getMutData() -> T*
        {
            return _arr;
        }

        constexpr auto _setData(T* data)
        {
            _arr = data;
        }

        constexpr auto _getCount() const -> usize
        {
            return _count;
        }

        constexpr auto _setCount(usize count)
        {
            _count = count;
        }

        constexpr auto _getCapacity() -> usize
        {
            return _capacity;
        }

        constexpr auto _setCapacity(usize capacity)
        {
            _capacity = capacity;
        }

        constexpr auto _allocMem(usize required) -> T*
        {
            return static_cast<T*>(_alloc.Alloc(required));
        }

        constexpr auto _allocMemAtLeast(usize required, usize hint) -> T*
        {
            return _alloc.Alloc(required);
        }

        constexpr auto _deallocMem(T* mem)
        {
            _alloc.Dealloc(mem);
        }

    private:
        T* _arr;
        usize _count;
        usize _capacity;
        TAlloc _alloc;
    };
}
