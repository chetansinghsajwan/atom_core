#pragma once
#include "_DynArrImpl.h"
#include "Atom/Invokable/Invokable.h"
#include "Atom/Memory/DefaultMemAllocator.h"
#include "Atom/Range.h"

namespace Atom
{
    /// --------------------------------------------------------------------------------------------
    /// # To Do
    /// - Write time complexities after writing implementation.
    /// - Add note for case, where element or elements to be inserted are from this array.
    /// --------------------------------------------------------------------------------------------
    template <typename T, typename TAlloc = DefaultMemAllocator>
    requires(not RRef<T>)
        and (not RIsVoid<T>)
    class DynArr:
        public MutArrRangeTrait<DynArr<T>>
    {
        friend class MutArrRangeTraitImpl<DynArr<T, TAlloc>>;

    private:
        using _Impl = _DynArrImpl<T, TAlloc>;

    public:
        using TElem = T;
        using TIter = ArrIter<T>;
        using TIterEnd = TIter;
        using TMutIter = ArrIter<T>;
        using TMutIterEnd = TMutIter;

    public:
        /// ----------------------------------------------------------------------------------------
        /// # Default Constructor
        /// ----------------------------------------------------------------------------------------
        constexpr DynArr():
            _impl{} { }

        /// ----------------------------------------------------------------------------------------
        /// # Copy Constructor
        /// ----------------------------------------------------------------------------------------
        constexpr DynArr(const DynArr& that):
            _impl{ that.iter(), that.iterEnd() }
        {
            _impl.insertRangeBack(that.iter(), that.iterEnd());
        }

        /// ----------------------------------------------------------------------------------------
        /// # Copy Operator
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator=(const DynArr& that) -> DynArr&
        {
            _impl.assignRange(that.iter(), that.iterEnd());
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # Move Constructor
        /// ----------------------------------------------------------------------------------------
        constexpr DynArr(DynArr&& that):
            _impl{ mov(that._impl) } {}

        /// ----------------------------------------------------------------------------------------
        /// # Move Operator
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator=(DynArr&& that) -> DynArr&
        {
            _impl.storage().move(mov(that));
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # Range Constructor
        /// ----------------------------------------------------------------------------------------
        template <typename TRange, typename TRangeUnqualified = TTI::TRemoveQuailfiersRef<TRange>>
        constexpr DynArr(TRange&& range)
            requires(RRangeOf<TRangeUnqualified, T>):
            _impl{ range.iter(), range.iterEnd() }
        {}

        /// ----------------------------------------------------------------------------------------
        /// # Range Operator
        /// ----------------------------------------------------------------------------------------
        template <typename TRange>
        constexpr DynArr& operator=(TRange&& range)
            requires(RRangeOf<TRange, T>)
        {
            _impl.assignRange(range.iter(), range.iterEnd());
        }

        /// ----------------------------------------------------------------------------------------
        /// # Destructor
        /// ----------------------------------------------------------------------------------------
        constexpr ~DynArr()
        {
            _impl.onDestruct();
        }

    //// -------------------------------------------------------------------------------------------
    //// Insert
    //// -------------------------------------------------------------------------------------------

    public:
        /// ----------------------------------------------------------------------------------------
        /// Constructs element at index `i` with `args`.
        /// 
        /// # Parameters
        /// - `i`: Index to insert element at.
        /// - `args...`: Args to construct element with.
        /// 
        /// # Time Complexity
        /// 
        /// # Iter Invalidation
        /// All iters are invalidated.
        /// ----------------------------------------------------------------------------------------
        template <typename... TArgs>
        constexpr auto emplaceAt(usize i, TArgs&&... args)
            requires(RConstructible<T, TArgs...>)
        {
            debug_expects(isIndexInRangeOrEnd(i), "Index is out of range.");

            _impl.emplaceAt(i, fwd(args)...);
        }

        /// ----------------------------------------------------------------------------------------
        /// Constructs element at pos `it` with `args`.
        /// 
        /// # Parameters
        /// - `it`: Pos to insert element at.
        /// - `args...`: Args to construct element with.
        /// 
        /// # Returns
        /// [`TMutIter`] to element inserted.
        /// 
        /// # Time Complexity
        /// 
        /// # Iter Invalidation
        /// All iters are invalidated.
        /// ----------------------------------------------------------------------------------------
        template <typename... TArgs>
        constexpr auto emplaceAt(TIter it, TArgs&&... args) -> TMutIter
            requires(RConstructible<T, TArgs...>)
        {
            debug_expects(isIterValid(it), "Invalid iter.");
            debug_expects(isIterInRangeOrEnd(it), "Iter is out of range.");

            usize i = indexForIter(it);
            _impl.emplaceAt(i, fwd(args)...);
            return _impl.mutIter(i);
        }

        /// ----------------------------------------------------------------------------------------
        /// Inserts elements at index `i`. Forwards each value returned by `range.iter()` to 
        /// constructor of element in the arr.
        /// 
        /// # Parameters
        /// - `i`: Index to insert elements at.
        /// - `range`: Range of elements to insert.
        /// 
        /// # Returns
        /// Count of elements inserted.
        /// 
        /// # Time Complexity
        /// 
        /// # Iter Invalidation
        /// All iters are invalidated.
        /// ----------------------------------------------------------------------------------------
        template <typename TRange>
        constexpr auto insertRangeAt(usize i, TRange&& range) -> usize
            requires(RRangeOf<TRange, T>)
                and (RConstructible<T, typename TRange::TElem>)
        {
            debug_expects(isIndexInRangeOrEnd(i), "Index is out of range.");

            usize count = _impl.insertRangeAt(i, range.iter(), range.iterEnd());
            return count;
        }

        /// ----------------------------------------------------------------------------------------
        /// Inserts elements at pos `it`. Forwards each value returned by `range.iter()` to 
        /// constructor of element in the arr.
        /// 
        /// # Parameters
        /// - `it`: Pos to insert elements at.
        /// - `range`: Range of elements to insert.
        /// 
        /// # Returns
        /// `MutRange` of elements inserted.
        /// 
        /// # Time Complexity
        /// 
        /// # Iter Invalidation
        /// All iters are invalidated.
        /// ----------------------------------------------------------------------------------------
        template <typename TRange>
        constexpr auto insertRangeAt(TIter it, TRange&& range) -> Range<TMutIter, TMutIterEnd>
            requires(RRangeOf<TRange, T>)
                and (RConstructible<T, typename TRange::TElem>)
        {
            debug_expects(isIterValid(it), "Invalid iter.");
            debug_expects(isIterInRangeOrEnd(it), "Iter is out of range.");

            usize i = indexForIter(it);
            usize count = _impl.insertRangeAt(i, range.iter(), range.iterEnd());
            return Range(_impl.mutIter(i), _impl.mutIter(i + count));
        }

        /// ----------------------------------------------------------------------------------------
        /// Constructs element at front with `args`.
        /// 
        /// # Parameters
        /// - `args...`: Args to construct element with.
        /// 
        /// # Time Complexity
        /// 
        /// # Iter Invalidation
        /// All iters are invalidated.
        /// ----------------------------------------------------------------------------------------
        template <typename... TArgs>
        constexpr auto emplaceFront(TArgs&&... args)
            requires(RConstructible<T, TArgs...>)
        {
            _impl.emplaceFront(fwd(args)...);
        }

        /// ----------------------------------------------------------------------------------------
        /// Inserts elements at front. Forwards each value returned by `range.iter()` to 
        /// constructor of element in the arr.
        /// 
        /// # Parameters
        /// - `range`: Range of elements to insert.
        /// 
        /// # Returns
        /// [`TMutIter`] to past the last inserted element.
        /// 
        /// # Time Complexity
        /// 
        /// # Iter Invalidation
        /// All iters are invalidated.
        /// ----------------------------------------------------------------------------------------
        template <typename TRange>
        constexpr auto insertRangeFront(TRange&& range) -> TMutIter
            requires(RRangeOf<TRange, T>)
                and (RConstructible<T, typename TRange::TElem>)
        {
            usize count = _impl.insertRangeFront(fwd(range));
            return _impl.mutIter(count);
        }

        /// ----------------------------------------------------------------------------------------
        /// Constructs element at back with `args`.
        /// 
        /// # Parameters
        /// - `args...`: Args to construct element with.
        /// 
        /// # Time Complexity
        /// 
        /// # Iter Invalidation
        /// All iters are invalidated.
        /// ----------------------------------------------------------------------------------------
        template <typename... TArgs>
        constexpr auto emplaceBack(TArgs&&... args)
            requires(RConstructible<T, TArgs...>)
        {
            _impl.emplaceBack(fwd(args)...);
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        template <typename T1>
        constexpr auto operator+=(T1&& el)
            requires(RConstructible<T, T1>)
        {
            _impl.emplaceBack(fwd(el));
        }

        /// ----------------------------------------------------------------------------------------
        /// Inserts elements at back. Forwards each value returned by `range.iter()` to 
        /// constructor of element in the arr.
        /// 
        /// # Parameters
        /// - `range`: Range of elements to insert.
        /// 
        /// # Returns
        /// [`TMutIter`] to the first inserted element.
        /// 
        /// # Time Complexity
        /// 
        /// # Iter Invalidation
        /// All iters are invalidated.
        /// ----------------------------------------------------------------------------------------
        template <typename TRange>
        constexpr auto insertRangeBack(TRange&& range) -> TMutIter
            requires(RRangeOf<TRange, T>)
                and (RConstructible<T, typename TRange::TElem>)
        {
            usize count = _impl.insertRangeBack(range.iter(), range.iterEnd());
            return _impl.mutIter(_impl.count() - count);
        }

        /// ----------------------------------------------------------------------------------------
        /// # To Do:
        /// - What happens when the elem type accepts range as parameter?
        /// ----------------------------------------------------------------------------------------
        template <typename TRange, typename TRangeUnqualified = TTI::TRemoveQuailfiersRef<TRange>>
        constexpr auto operator+=(TRange&& range)
            requires(RRangeOf<TRangeUnqualified, T>)
                and (RConstructible<T, typename TRangeUnqualified::TElem>)
        {
            _impl.insertRangeBack(mov(range.iter()), mov(range.iterEnd()));
        }

    //// -------------------------------------------------------------------------------------------
    //// Remove
    //// -------------------------------------------------------------------------------------------

    public:
        /// ----------------------------------------------------------------------------------------
        /// Removes element at index `i`.
        /// 
        /// # Parameters
        /// - `i`: Index to remove element at.
        /// ----------------------------------------------------------------------------------------
        constexpr auto removeAt(usize i)
        {
            debug_expects(isIndexInRange(i), "Index is out of range.");

            _impl.removeAt(i);
        }

        /// ----------------------------------------------------------------------------------------
        /// Removes element at pos `it`.
        /// 
        /// # Parameters
        /// - `it`: Pos to remove element at.
        /// 
        /// # Returns
        /// [TMutIter] to next element. If `it` was pointing to the last element, 
        /// returns [`iterEnd()`].
        /// ----------------------------------------------------------------------------------------
        constexpr auto removeAt(TIter it) -> TMutIter
        {
            debug_expects(isIterValid(it), "Invalid iter.");
            debug_expects(isIterInRange(it), "Iter is out of range.");

            usize i = indexForIter(it);
            _impl.removeAt(i);
            return _impl.mutIter(i);
        }

        /// ----------------------------------------------------------------------------------------
        /// Removes elements in range [[`from`, `to`]].
        /// 
        /// # Parameters
        /// - `from`: Start of range to remove elements at.
        /// - `to`: End of range to remove elements at.
        /// 
        /// # Returns
        /// `from`.
        /// 
        /// ## Explanation
        /// After removing `to - from` elements, next elements will be shifted back to index `from`.
        /// So the next element will be available at index `from`. If the last element of the arr
        /// was also removed, `from` will be equal to [`count()`].
        /// ----------------------------------------------------------------------------------------
        constexpr auto removeRange(usize from, usize to) -> usize
        {
            debug_expects(from <= to, "Invalid range.");
            debug_expects(isIndexInRange(to), "Index was out of range.");
            debug_ensures(fnret <= count(), "Invalid return value.");

            _impl.removeRange(from, to);
            return from;
        }

        /// ----------------------------------------------------------------------------------------
        /// Removes elements in range represented by `range`.
        /// 
        /// # Parameters
        /// - `it`: Start of range to remove elements at.
        /// - `itEnd`: End of range to remove elements at.
        /// 
        /// # Returns
        /// [`TMutIter`] to next element of the last removed element. If the last removed element 
        /// was also the last element of the arr, returns [`iterEnd()`].
        /// ----------------------------------------------------------------------------------------
        constexpr auto removeRange(TIter it, TIter itEnd) -> TMutIter
        {
            debug_expects(isIterValid(it), "Invalid iter.");
            debug_expects(isIterValid(itEnd), "Invalid iter.");
            debug_expects(isIterInRange(it), "Iter is out range.")
            debug_expects(isIterInRange(itEnd), "Iter is out range.")
            debug_expects(it.compare(itEnd) <= 0, "Invalid range.");
            debug_ensures(fnret.compare(iterEnd()) <= 0, "Invalid return value.")

            usize from = indexForIter(it);
            usize to = indexForIter(itEnd);
            _impl.removeRange(from, to);
            return _impl.mutIter(from);
        }

        /// ----------------------------------------------------------------------------------------
        /// Removes `count_` elements from front.
        /// ----------------------------------------------------------------------------------------
        constexpr auto removeFront(usize count_ = 1)
        {
            debug_expects(count_ <= count());

            _impl.removeFront(count_);
        }

        /// ----------------------------------------------------------------------------------------
        /// Removes `count_` elements from back.
        /// ----------------------------------------------------------------------------------------
        constexpr auto removeBack(usize count_ = 1)
        {
            debug_expects(count_ <= count());

            _impl.removeBack(count_);
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        template <typename TPred>
        constexpr auto removeIf(TPred&& pred) -> usize
            requires(RInvokable<TPred, bool(const TElem&)>)
        {
            usize removedCount = 0;
            for (usize i = 0; i < _impl.count(); i++)
            {
                if (pred(_impl.data()[i]))
                {
                    _impl.removeAt(i);
                    i--;
                    removedCount++;
                }
            }

            return removedCount;
        }

        /// ----------------------------------------------------------------------------------------
        /// Removes all elements.
        /// 
        /// # Note
        /// - Doesn't free storage.
        /// ----------------------------------------------------------------------------------------
        constexpr auto removeAll()
        {
            _impl.removeAll();
        }

    //// -------------------------------------------------------------------------------------------
    //// Memory
    //// -------------------------------------------------------------------------------------------

    public:
        /// ----------------------------------------------------------------------------------------
        /// Reserves memory for `count` elements. If there is already enough memory reserved,
        /// does nothing.
        /// ----------------------------------------------------------------------------------------
        constexpr auto reserve(usize count)
        {
            return _impl.reserve(count);
        }

        /// ----------------------------------------------------------------------------------------
        /// Reserves memory for `count` count more elements.
        /// ----------------------------------------------------------------------------------------
        constexpr auto reserveMore(usize count)
        {
            return _impl.reserveMore(count);
        }

        /// ----------------------------------------------------------------------------------------
        /// Releases unused memory.
        /// ----------------------------------------------------------------------------------------
        constexpr auto releaseMem()
        {
            return _impl.releaseMem();
        }

        /// ----------------------------------------------------------------------------------------
        /// Total count of places, with elements and reserved.
        /// ----------------------------------------------------------------------------------------
        constexpr auto capacity() const -> usize
        {
            return _impl.capacity();
        }

        /// ----------------------------------------------------------------------------------------
        /// Count of places reserved for elements.
        /// ----------------------------------------------------------------------------------------
        constexpr auto reservedCount() const -> usize
        {
            return _impl.capacity() - _impl.count();
        }

    public:
        /// ----------------------------------------------------------------------------------------
        /// Checks if index `i` is in range [`0`, [`count()`]).
        /// ----------------------------------------------------------------------------------------
        constexpr bool isIndexInRange(usize i) const
        {
            return _impl.isIndexInRange(i);
        }

        /// ----------------------------------------------------------------------------------------
        /// Checks if index `i` is in range [`0`, [`count()`]].
        /// ----------------------------------------------------------------------------------------
        constexpr bool isIndexInRangeOrEnd(usize i) const
        {
            return _impl.isIndexInRangeOrEnd(i);
        }

        /// ----------------------------------------------------------------------------------------
        /// Get index for iter `it`.
        /// ----------------------------------------------------------------------------------------
        constexpr auto indexForIter(TIter it) const -> usize
        {
            return _impl.indexForIter(it);
        }

        /// ----------------------------------------------------------------------------------------
        /// Checks for iter has been invalidated. This is done through a value which is changed 
        /// for the container every time iters are invalidated.
        /// 
        /// # To Do
        /// - Implement iter validation.
        /// ----------------------------------------------------------------------------------------
        constexpr bool isIterValid(TIter it) const
        {
            return _impl.isIterValid(it);
        }

        /// ----------------------------------------------------------------------------------------
        /// Checks if iter `pos` is in range [[`iter()`], [`iterEnd()`]).
        /// ----------------------------------------------------------------------------------------
        constexpr bool isIterInRange(TIter it) const
        {
            return _impl.isIterInRange(it);
        }

        /// ----------------------------------------------------------------------------------------
        /// Checks if iter `pos` is in range [[`iter()`], [`iterEnd()`]].
        /// ----------------------------------------------------------------------------------------
        constexpr bool isIterInRangeOrEnd(TIter it) const
        {
            return _impl.isIterInRangeOrEnd(it);
        }

    private:
        _Impl _impl;
    };

    template <typename T, typename TAlloc>
    class MutArrRangeTraitImpl<DynArr<T, TAlloc>>
    {
        using _Arr = DynArr<T, TAlloc>;
        using _ArrImpl = _DynArrImpl<T, TAlloc>;

    public:
        using TElem = T;
        using TIter = ArrIter<T>;
        using TIterEnd = TIter;
        using TMutIter = MutArrIter<T>;
        using TMutIterEnd = TMutIter;

    public:
        constexpr auto data() const -> const T*
        {
            return _impl().data();
        }

        constexpr auto mutData() -> T*
        {
            return _mutImpl().mutData();
        }

        constexpr auto count() const -> usize
        {
            return _impl().count();
        }

        constexpr auto iter() const -> TIter
        {
            return TIter{ _impl().data() };
        }

        constexpr auto iterEnd() const -> TIterEnd
        {
            return TIterEnd{ _impl().data() + _impl().count() };
        }

        constexpr auto mutIter() -> TMutIter
        {
            return TMutIter{ _mutImpl().mutData() };
        }

        constexpr auto mutIterEnd() -> TMutIterEnd
        {
            return TMutIterEnd{ _mutImpl().mutData() + _mutImpl().count() };
        }

    private:
        constexpr auto _impl() const -> const _ArrImpl&
        {
            return reinterpret_cast<const _Arr&>(*this)._impl;
        }

        constexpr auto _mutImpl() -> _ArrImpl&
        {
            return reinterpret_cast<_Arr&>(*this)._impl;
        }
    };
}
