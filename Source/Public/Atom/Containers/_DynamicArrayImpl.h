#pragma once
#include "Atom/Contracts.h"
#include "Atom/Core.h"
#include "Atom/Range/ArrayIter.h"

#include <algorithm>

namespace Atom
{
    template <typename TElem_, typename TAlloc_>
    class _DynamicArrayImpl
    {
    public:
        using TElem = TElem_;
        using TAlloc = TAlloc_;
        using TIter = ArrayIter<TElem>;
        using TIterEnd = TIter;
        using TMutIter = MutArrayIter<TElem>;
        using TMutIterEnd = TMutIter;

    public:
        constexpr _DynamicArrayImpl():
            _arr{ nullptr }, _count{ 0 }, _capacity{ 0 }, _alloc{}
        {}

        constexpr _DynamicArrayImpl(_DynamicArrayImpl&& that) {}

        template <typename UIter, typename UIterEnd>
        constexpr _DynamicArrayImpl(UIter it, UIterEnd itEnd):
            _DynamicArrayImpl{}
        {
            insertRangeBack(it, itEnd);
        }

    public:
        constexpr auto onDestruct()
        {
            removeAll();
            releaseUnusedMem();
        }

        constexpr auto at(usize i) const -> const TElem&
        {
            return _getData()[i.val()];
        }

        constexpr auto mutAt(usize i) const -> const TElem&
        {
            return _getMutData()[i.val()];
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

        template <typename UIter, typename UIterEnd>
        constexpr auto assignRange(UIter it, UIterEnd itEnd)
        {
            removeAll();
            insertRangeBack(it, itEnd);
        }

        template <typename... TArgs>
        constexpr auto emplaceAt(usize i, TArgs&&... args)
        {
            return _emplaceAt(i, forward<TArgs>(args)...);
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
            return _emplaceFront(forward<TArgs>(args)...);
        }

        template <typename UIter, typename UIterEnd>
        constexpr auto insertRangeFront(UIter it, UIterEnd itEnd) -> usize
        {
            return insertRangeAt(0, it, itEnd);
        }

        template <typename... TArgs>
        constexpr auto emplaceBack(TArgs&&... args)
        {
            return _emplaceAt(_getCount() - 1, forward<TArgs>(args)...);
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
            if (_getCount() > 0)
            {
                _destructRange(0, _getCount().sub(1));
                _moveRangeFront(0, _getCount().sub(1));
            }
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

        constexpr auto data() const -> ConstMemPtr<TElem>
        {
            return _getData();
        }

        constexpr auto mutData() -> MemPtr<TElem>
        {
            return _getMutData();
        }

        constexpr auto alloc() const -> const TAlloc&
        {
            return _alloc;
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
            return i < 0 ? usize::Max() : usize(i);
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
            _constructAt(i, forward<TArgs>(args)...);

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
                _constructAt(i + i, forward<decltype(it.value())>(it.value()));
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
            _constructAt(_getCount(), forward<U>(el));
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
                _constructAt(i + j, forward<decltype(it.value())>(it.value()));
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
            MemPtr<TElem> newArray = _allocMem(newCap);

            _moveRangeTo(0, newArray);
            _deallocMem(_getMutData());
            _setData(newArray);
            _setCapacity(newCap);
        }

        template <typename... TArgs>
        constexpr auto _constructAt(usize i, TArgs&&... args) -> void
        {
            MemPtr<TElem> src = _getMutData() + i;
            std::construct_at(src.raw(), forward<TArgs>(args)...);
        }

        constexpr auto _destructAt(usize i) -> void
        {
            MemPtr<TElem> src = _getMutData() + i;
            std::destroy_at(src.raw());
        }

        constexpr auto _destructRange(usize i, usize count) -> void
        {
            MemPtr<TElem> begin = _getMutData() + i;
            MemPtr<TElem> end = begin + count;
            std::destroy(begin.raw(), end.raw());
        }

        constexpr auto _moveRangeFront(usize i, usize count) -> void
        {
            MemPtr<TElem> begin = _getMutData() + i;
            MemPtr<TElem> end = _getMutData() + _getCount() - 1;
            MemPtr<TElem> dest = begin - count;
            std::move(begin.raw(), end.raw(), dest.raw());
        }

        constexpr auto _moveRangeBack(usize i, usize count) -> void
        {
            MemPtr<TElem> begin = _getMutData() + i;
            MemPtr<TElem> end = _getMutData() + _getCount() - 1;
            MemPtr<TElem> dest = begin + count;
            std::move_backward(begin.raw(), end.raw(), dest.raw());
        }

        constexpr auto _moveRangeTo(usize i, MemPtr<TElem> dest) -> void
        {
            MemPtr<TElem> begin = _getMutData() + i;
            MemPtr<TElem> end = _getMutData() + _getCount() - 1;
            std::move_backward(begin.raw(), end.raw(), dest.raw());
        }

        constexpr auto _rotateRangeBack(usize i, usize count) -> void
        {
            MemPtr<TElem> begin = _getMutData();
            MemPtr<TElem> mid = begin + i;
            MemPtr<TElem> end = begin + _getCount() - 1;
            std::rotate(begin.raw(), mid.raw(), end.raw());
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
                return it.compare(itEnd).abs().template to<usize>();
            }

            usize count = 0;
            for (; it.equals(itEnd); it.next())
                count++;

            return count;
        }

        constexpr auto _getData() const -> ConstMemPtr<TElem>
        {
            return _arr;
        }

        constexpr auto _getMutData() -> MemPtr<TElem>
        {
            return _arr;
        }

        constexpr auto _setData(MemPtr<TElem> data)
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

        constexpr auto _getCapacity() const -> usize
        {
            return _capacity;
        }

        constexpr auto _setCapacity(usize capacity)
        {
            _capacity = capacity;
        }

        constexpr auto _allocMem(usize required) -> MemPtr<TElem>
        {
            return _alloc.Alloc(required);
        }

        constexpr auto _allocMemAtLeast(usize required, usize hint) -> MemPtr<TElem>
        {
            return _alloc.Alloc(required);
        }

        constexpr auto _deallocMem(MemPtr<TElem> mem)
        {
            _alloc.Dealloc(mem);
        }

    private:
        MemPtr<TElem> _arr;
        usize _count;
        usize _capacity;
        TAlloc _alloc;
    };
}
