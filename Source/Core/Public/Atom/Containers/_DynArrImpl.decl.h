#pragma once
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
            _arr{ nullptr }, _count{ 0 }, _capacity{ 0 }, _alloc{} {}

        constexpr _DynArrImpl(_DynArrImpl&& that) {}

    //// -------------------------------------------------------------------------------------------
    //// 
    //// -------------------------------------------------------------------------------------------

    public:
        constexpr auto onDestruct()
        {
            removeAll();
            releaseUnusedMem();
        }

    //// -------------------------------------------------------------------------------------------
    //// Insert
    //// -------------------------------------------------------------------------------------------

    public:
        template <typename... TArgs>
        constexpr auto emplaceAt(usize i, TArgs&&... args)
        {
            return _emplaceAt(i, fwd(args)...);
        }

        template <typename UIter, typename UIterEnd>
        constexpr auto insertRangeAt(usize i, UIter it, UIterEnd itEnd) -> usize
        {
            if constexpr (_canGetRangeSize<UIter, UIterEnd>())
            {
                return _insertRangeAtCounted(i, it, _getRangeSize(it, itEnd));
            }
            else
            {
                return _insertRangeAtUncounted(i, it, itEnd);
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
            return _emplaceBack(fwd(args)...);
        }

        template <typename UIter, typename UIterEnd>
        constexpr auto insertRangeBack(UIter it, UIterEnd itEnd) -> usize
        {
            if constexpr (_canGetRangeSize<UIter, UIterEnd>())
            {
                usize count = _getRangeSize(it, itEnd);
                _insertRangeBackCounted(it, count);
                return count;
            }
            else
            {
                return _insertRangeBackUncounted(it, itEnd);
            }
        }

    //// -------------------------------------------------------------------------------------------
    //// Remove
    //// -------------------------------------------------------------------------------------------

    public:
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

    //// -------------------------------------------------------------------------------------------
    //// Memory
    //// -------------------------------------------------------------------------------------------

    public:
        constexpr auto reserve(usize count)
        {
            return reserveMore(count);
        }

        constexpr auto reserveMore(usize count)
        {
            _ensureCapFor(_getCount() + count);
        }

        constexpr auto releaseUnusedMem()
        {
        }

        constexpr auto capacity() const -> usize
        {
            return _getCapacity();
        }

    //// -------------------------------------------------------------------------------------------
    //// 
    //// -------------------------------------------------------------------------------------------

    public:
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

    //// -------------------------------------------------------------------------------------------
    //// Validation
    //// -------------------------------------------------------------------------------------------

    public:
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

    //// -------------------------------------------------------------------------------------------
    //// Implementations
    //// -------------------------------------------------------------------------------------------

    private:
        template <typename U>
        constexpr auto _insertAt(usize i, U&& el) -> usize
        {
            _ensureCapFor(i);
            _moveRangeBack(i, 1);
            _constructAt(i, fwd(el));

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
        constexpr auto _insertRangeAtUncounted(usize i, UIter begin, UIterEnd end)
            -> usize
        {
            usize rotateSize = _getCount() - i;
            _insertRangeBackUncounted(begin, end);
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

    private:
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
            _deallocMem(_getData());
            _setData(newArr);
            _setCapacity(newCap);
        }

        constexpr auto _constructAt(usize i, auto&&... args) -> void
        {
            std::construct_at(_getData() + i, fwd(args)...);
        }

        constexpr auto _destructAt(usize i) -> void
        {
            std::destroy_at(_getData() + i);
        }

        constexpr auto _destructRange(usize i, usize count) -> void
        {
            std::destroy(_getData() + i, _getData() + i + count);
        }

        constexpr auto _moveRangeFront(usize i, usize count) -> void
        {
            T* begin = _getMutData() + i;
            T* end = _getMutData() + _getCount();
            T* dest = begin - count;
            std::move(begin, end, dest);
        }

        constexpr auto _moveRangeBack(usize i, usize count) -> void
        {
            T* begin = _getMutData() + i;
            T* end = _getMutData() + _getCount();
            T* dest = begin + count;
            std::move_backward(begin, end, dest);
        }

        constexpr auto _moveRangeTo(usize i, TElem* dest) -> void
        {
            std::move(_getData() + i, dest);
        }

        constexpr auto _rotateRangeBack(usize i, usize count) -> void
        {
            std::rotate(_getData() + i, _getData() + i + count, _getData() + _getCount() - 1);
        }

        template <typename UIter, typename UIterEnd>
        static constexpr auto _canGetRangeSize() -> bool
        {
            return RFwdIterPair<UIter, UIterEnd>;
        }

        template <typename UIter, typename UIterEnd>
        static constexpr auto _getRangeSize(TIter it, UIterEnd itEnd) -> usize
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

        constexpr auto _allocMem(usize required) const -> T*
        {
            return _alloc.allocMem(required);
        }

        constexpr auto _allocMemAtLeast(usize required, usize hint) const -> T*
        {
            return _alloc.allocMem(required);
        }

    private:
        T* _arr;
        usize _count;
        usize _capacity;
        TAlloc _alloc;
    };
}
