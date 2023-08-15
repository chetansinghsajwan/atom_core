#pragma once
#include "Atom/Core.h"
#include "Atom/Range/ArrIter.h"

namespace Atom
{
    template <typename Storage>
    class _DynArrImpl
    {
        using _TStorage = Storage;

    public:
        using TElem = typename _TStorage::TElem;
        using TIter = ArrIter<TElem>;
        using TIterEnd = TIter;
        using TMutIter = MutArrIter<TElem>;
        using TMutIterEnd = TMutIter;

    //// -------------------------------------------------------------------------------------------
    //// Insert
    //// -------------------------------------------------------------------------------------------

    public:
        template <typename... TArgs>
        constexpr auto emplaceAt(usize i, TArgs&&... args)
        {
            return _emplaceAt(i, fwd(args)...);
        }

        template <typename TIter1, typename TIterEnd1>
        constexpr auto insertRangeAt(usize i, TIter1 it, TIterEnd1 itEnd) -> usize
        {
            if constexpr (_canGetRangeSize<TIter1, TIterEnd1>())
            {
                return _insertAtCounted(i, it, _getRangeSize(it, itEnd));
            }
            else
            {
                return _insertAtUncounted(i, it, itEnd);
            }
        }

        template <typename... TArgs>
        constexpr auto emplaceFront(TArgs&&... args)
        {
            return _emplaceFront(fwd(args)...);
        }

        template <typename TIter1, typename TIterEnd1>
        constexpr auto insertRangeFront(TIter1 it, TIterEnd1 itEnd) -> usize
        {
            return insertRangeAt(0, it, itEnd);
        }

        template <typename... TArgs>
        constexpr auto emplaceBack(TArgs&&... args)
        {
            return _emplaceBack(fwd(args)...);
        }

        template <typename TIter1, typename TIterEnd1>
        constexpr auto insertRangeBack(TIter1 it, TIterEnd1 itEnd) -> usize
        {
            if constexpr (_canGetRangeSize<TIter1, TIterEnd1>())
            {
                return _insertBackCounted(it, _getRangeSize(it, itEnd));
            }
            else
            {
                return _insertBackUncounted(it, itEnd);
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
            _ensureCapFor(_count() + count);
        }

        constexpr auto releaseUnusedMem()
        {
        }

        constexpr auto capacity() const -> usize
        {
            return _capacity();
        }

    //// -------------------------------------------------------------------------------------------
    //// 
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

        constexpr auto validateIter(TIter it) const -> bool
        {
            return true;
        }

        constexpr auto indexForIter(TIter it) const -> usize
        {
            return true;
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
        template <typename T1>
        constexpr auto _insertAt(usize i, T1&& el) -> usize
        {
            _ensureCapFor(1);
            _moveRangeBack(i, 1);
            _constructAt(i, fwd(el));

            return i;
        }

        template <typename TIter1>
        constexpr auto _insertAtCounted(usize i, TIter1 it, usize count) -> usize
        {
            if (count == 0)
                return i;

            _ensureCapFor(count);
            _moveRangeBack(i, count);

            // Insert new elements
            for (usize i = 0; i < count; i++)
            {
                _constructAt(i + i, fwd(*it++));
            }

            return i;
        }

        template <typename TIter1, typename TIterEnd1>
        constexpr auto _insertAtUncounted(usize i, TIter1 begin, TIterEnd1 end)
            -> usize
        {
            usize rotateSize = _getCount() - i;
            _insertBackUncounted(begin, end);
            _rotateRangeBack(i, rotateSize);

            return i;
        }

        template <typename T1>
        constexpr auto _insertBack(T1&& el) -> usize
        {
            _ensureCapFor(1);
            _constructAt(_getCount(), fwd(el));
            _setcount(_getCount() + 1);

            return _getCount() - 1;
        }

        template <typename TIter1>
        constexpr auto _insertBackCounted(TIter1 it, usize count) -> usize
        {
            usize i = _getCount();

            if (count == 0)
                return i;

            _ensureCapFor(count);

            for (usize i = 0; i < count; i++)
            {
                _constructAt(i + i, fwd(*it++));
            }

            _setcount(_getCount() + count);
            return i;
        }

        template <typename TIter1, typename TIterEnd1>
        constexpr auto _insertBackUncounted(TIter1 begin, TIterEnd1 end) -> usize
        {
            usize i = _getCount();
            for (auto&& el : Range(begin, end))
            {
                _ensureCapFor(1);
                _constructAt(_getCount(), fwd(el));
                _setcount(_getCount() + 1);
            }

            return i;
        }

    private:
        constexpr auto _updateIterDebugId() -> void;
        constexpr auto _validateIndex(isize i) const -> bool;
        constexpr auto _calcCapGrowth(usize required) const -> usize;

        constexpr auto _ensureCapFor(usize count)
        {
            _updateIterDebugId();

            // We have enough capacity.
            if (_capacity() - _getCount() >= count)
                return;

            usize newCap = _calcCapGrowth(count);
            TElem* newArr = _AllocMem(newCap);

            _moveRangeTo(0, newArr);
            _deallocMem(_Data());
            _Data(newArr);
            _capacity(newCap);
        }

    private:
        constexpr auto _constructAt(usize i, auto&&... args) -> void;
        constexpr auto _destructAt(usize i) -> void;
        constexpr auto _destructRange(usize i, usize count) -> void;
        constexpr auto _moveRangeFront(usize i, usize count) -> void;
        constexpr auto _moveRangeBack(usize i, usize count) -> void;
        constexpr auto _moveRangeTo(usize i, TElem* dest) -> void;
        constexpr auto _rotateRangeBack(usize i, usize count) -> void;

        template <typename TIter1, typename TIterEnd1>
        static constexpr auto _canGetRangeSize() -> bool
        {
            return RFwdIterPair<TIter1, TIterEnd1>;
        }

        template <typename TIter1, typename TIterEnd1>
        static constexpr auto _getRangeSize(TIter it, TIterEnd1 itEnd) -> usize
        {
            if constexpr (RJumpIterPair<TIter1, TIterEnd1>)
            {
                return -it.compare(itEnd);
            }

            usize count = 0;
            for (; it.equals(itEnd); it.next())
                count++;

            return count;
        }

    private:
        constexpr auto _getCount() -> usize
        {
            return _storage.count();
        }

    private:
        _TStorage _storage;
        // _TAlloc _alloc;
    };
}
