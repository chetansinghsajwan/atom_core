#pragma once
#include "Atom/Contracts.h"
#include "Atom/Invokable/Invokable.h"
#include "Atom/Memory/DefaultMemAllocator.h"
#include "Atom/Range.h"
#include "_DynamicArrayImpl.h"

namespace Atom
{
    /// --------------------------------------------------------------------------------------------
    /// # To Do
    /// - Write time complexities after writing implementation.
    /// - Add note for case, where element or elements to be inserted are from this array.
    /// --------------------------------------------------------------------------------------------
    template <typename TElem_, typename TAlloc_>
    class BasicDynamicArray
    {
        static_assert(not TTI::IsRef<TElem_>, "DynamicArray doesn't supports ref types.");
        static_assert(not TTI::IsVoid<TElem_>, "DynamicArray doesn't support void.");

    private:
        using Impl = _DynamicArrayImpl<TElem_, TAlloc_>;

    public:
        using TElem = TElem_;
        using TAlloc = TAlloc_;
        using TIter = ArrayIter<TElem>;
        using TIterEnd = TIter;
        using TMutIter = MutArrayIter<TElem>;
        using TMutIterEnd = TMutIter;

    public:
        /// ----------------------------------------------------------------------------------------
        /// # Default Constructor
        /// ----------------------------------------------------------------------------------------
        constexpr BasicDynamicArray():
            _impl{}
        {}

        /// ----------------------------------------------------------------------------------------
        /// # Copy Constructor
        /// ----------------------------------------------------------------------------------------
        constexpr BasicDynamicArray(const BasicDynamicArray& that):
            _impl{ that.iter(), that.iterEnd() }
        {}

        /// ----------------------------------------------------------------------------------------
        /// # Copy Operator
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator=(const BasicDynamicArray& that) -> BasicDynamicArray&
        {
            _impl.assignRange(that.iter(), that.iterEnd());
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # Move Constructor
        /// ----------------------------------------------------------------------------------------
        constexpr BasicDynamicArray(BasicDynamicArray&& that):
            _impl{ mov(that._impl) }
        {}

        /// ----------------------------------------------------------------------------------------
        /// # Move Operator
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator=(BasicDynamicArray&& that) -> BasicDynamicArray&
        {
            _impl.storage().move(mov(that));
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # Range Constructor
        /// ----------------------------------------------------------------------------------------
        template <typename TRange, typename TRangeUnqualified = TTI::TRemoveQuailfiersRef<TRange>>
        constexpr BasicDynamicArray(TRange&& range)
            requires(RRangeOf<TRangeUnqualified, TElem>)
            :
            _impl{ range.iter(), range.iterEnd() }
        {}

        /// ----------------------------------------------------------------------------------------
        /// # Range Operator
        /// ----------------------------------------------------------------------------------------
        template <typename TRange>
        constexpr BasicDynamicArray& operator=(TRange&& range)
            requires(RRangeOf<TRange, TElem>)
        {
            _impl.assignRange(range.iter(), range.iterEnd());
        }

        /// ----------------------------------------------------------------------------------------
        /// # Destructor
        /// ----------------------------------------------------------------------------------------
        constexpr ~BasicDynamicArray()
        {
            _impl.onDestruct();
        }

    public:
        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto data() const -> MemPtr<TElem>
        {
            return _impl.data();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto mutData() -> MutMemPtr<TElem>
        {
            return _impl.mutData();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto count() const -> usize
        {
            return _impl.count();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto iter(usize i = 0) const -> TIter
        {
            return _impl.iter(i);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto iterEnd() const -> TIterEnd
        {
            return _impl.iterEnd();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto mutIter(usize i = 0) -> TMutIter
        {
            return _impl.mutIter(i);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto mutIterEnd() -> TMutIterEnd
        {
            return _impl.mutIterEnd();
        }

        /// ----------------------------------------------------------------------------------------
        /// Constructs element at index `i` with `args`.
        ///
        /// # Parameters
        ///
        /// - `i`: Index to insert element at.
        /// - `args...`: Args to construct element with.
        ///
        /// # Time Complexity
        ///
        /// # Iter Invalidation
        ///
        /// All iters are invalidated.
        /// ----------------------------------------------------------------------------------------
        template <typename... TArgs>
        constexpr auto emplaceAt(usize i, TArgs&&... args)
            requires(RConstructible<TElem, TArgs...>)
        {
            Contracts::DebugExpects(isIndexInRangeOrEnd(i), "Index is out of range.");

            _impl.emplaceAt(i, forward<TArgs>(args)...);
        }

        /// ----------------------------------------------------------------------------------------
        /// Constructs element at pos `it` with `args`.
        ///
        /// # Parameters
        ///
        /// - `it`: Pos to insert element at.
        /// - `args...`: Args to construct element with.
        ///
        /// # Returns
        ///
        /// [`TMutIter`] to element inserted.
        ///
        /// # Time Complexity
        ///
        /// # Iter Invalidation
        ///
        /// All iters are invalidated.
        /// ----------------------------------------------------------------------------------------
        template <typename... TArgs>
        constexpr auto emplaceAt(TIter it, TArgs&&... args) -> TMutIter
            requires(RConstructible<TElem, TArgs...>)
        {
            Contracts::DebugExpects(isIterValid(it), "Invalid iter.");
            Contracts::DebugExpects(isIterInRangeOrEnd(it), "Iter is out of range.");

            usize i = indexForIter(it);
            _impl.emplaceAt(i, forward<TArgs>(args)...);
            return _impl.mutIter(i);
        }

        /// ----------------------------------------------------------------------------------------
        /// Inserts elements at index `i`. Forwards each value returned by `range.iter()` to
        /// constructor of element in the arr.
        ///
        /// # Parameters
        ///
        /// - `i`: Index to insert elements at.
        /// - `range`: Range of elements to insert.
        ///
        /// # Returns
        ///
        /// Count of elements inserted.
        ///
        /// # Time Complexity
        ///
        /// # Iter Invalidation
        ///
        /// All iters are invalidated.
        /// ----------------------------------------------------------------------------------------
        template <typename TRange>
        constexpr auto insertRangeAt(usize i, TRange&& range) -> usize
            requires(RRangeOf<TRange, TElem>) and (RConstructible<TElem, typename TRange::TElem>)
        {
            Contracts::DebugExpects(isIndexInRangeOrEnd(i), "Index is out of range.");

            return _impl.insertRangeAt(i, range.iter(), range.iterEnd());
        }

        /// ----------------------------------------------------------------------------------------
        /// Inserts elements at pos `it`. Forwards each value returned by `range.iter()` to
        /// constructor of element in the arr.
        ///
        /// # Parameters
        ///
        /// - `it`: Pos to insert elements at.
        /// - `range`: Range of elements to insert.
        ///
        /// # Returns
        ///
        /// [`MutArrayRange`] of elements inserted.
        ///
        /// # Time Complexity
        ///
        /// # Iter Invalidation
        ///
        /// All iters are invalidated.
        /// ----------------------------------------------------------------------------------------
        template <typename TRange>
        constexpr auto insertRangeAt(TIter it, TRange&& range)
            requires(RRangeOf<TRange, TElem>) and (RConstructible<TElem, typename TRange::TElem>)
        {
            Contracts::DebugExpects(isIterValid(it), "Invalid iter.");
            Contracts::DebugExpects(isIterInRangeOrEnd(it), "Iter is out of range.");

            usize i = indexForIter(it);
            usize count = _impl.insertRangeAt(i, range.iter(), range.iterEnd());
            return MakeRange(_impl.mutIter(i), _impl.mutIter(i + count));
        }

        /// ----------------------------------------------------------------------------------------
        /// Constructs element at front with `args`.
        ///
        /// # Parameters
        ///
        /// - `args...`: Args to construct element with.
        ///
        /// # Time Complexity
        ///
        /// # Iter Invalidation
        ///
        /// All iters are invalidated.
        /// ----------------------------------------------------------------------------------------
        template <typename... TArgs>
        constexpr auto emplaceFront(TArgs&&... args)
            requires(RConstructible<TElem, TArgs...>)
        {
            _impl.emplaceFront(forward<TArgs>(args)...);
        }

        /// ----------------------------------------------------------------------------------------
        /// Inserts elements at front. Forwards each value returned by `range.iter()` to
        /// constructor of element in the arr.
        ///
        /// # Parameters
        ///
        /// - `range`: Range of elements to insert.
        ///
        /// # Returns
        ///
        /// [`TMutIter`] to past the last inserted element.
        ///
        /// # Time Complexity
        ///
        /// # Iter Invalidation
        ///
        /// All iters are invalidated.
        /// ----------------------------------------------------------------------------------------
        template <typename TRange>
        constexpr auto insertRangeFront(TRange&& range) -> TMutIter
            requires(RRangeOf<TRange, TElem>) and (RConstructible<TElem, typename TRange::TElem>)
        {
            usize count = _impl.insertRangeFront(forward<TRange&&>(range));
            return _impl.mutIter(count);
        }

        /// ----------------------------------------------------------------------------------------
        /// Constructs element at back with `args`.
        ///
        /// # Parameters
        ///
        /// - `args...`: Args to construct element with.
        ///
        /// # Time Complexity
        ///
        /// # Iter Invalidation
        ///
        /// All iters are invalidated.
        /// ----------------------------------------------------------------------------------------
        template <typename... TArgs>
        constexpr auto emplaceBack(TArgs&&... args)
            requires(RConstructible<TElem, TArgs...>)
        {
            _impl.emplaceBack(forward<TArgs>(args)...);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename TArg>
        constexpr auto operator+=(TArg&& el)
            requires(RConstructible<TElem, TArg>)
        {
            _impl.emplaceBack(forward<TArg>(el));
        }

        /// ----------------------------------------------------------------------------------------
        /// Inserts elements at back. Forwards each value returned by `range.iter()` to
        /// constructor of element in the arr.
        ///
        /// # Parameters
        ///
        /// - `range`: Range of elements to insert.
        ///
        /// # Returns
        ///
        /// [`TMutIter`] to the first inserted element.
        ///
        /// # Time Complexity
        ///
        /// # Iter Invalidation
        ///
        /// All iters are invalidated.
        /// ----------------------------------------------------------------------------------------
        template <typename TRange>
        constexpr auto insertRangeBack(TRange&& range) -> TMutIter
            requires(RRangeOf<TRange, TElem>) and (RConstructible<TElem, typename TRange::TElem>)
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
            requires(RRangeOf<TRangeUnqualified, TElem>)
                    and (RConstructible<TElem, typename TRangeUnqualified::TElem>)
        {
            _impl.insertRangeBack(mov(range.iter()), mov(range.iterEnd()));
        }

        /// ----------------------------------------------------------------------------------------
        /// Removes element at index `i`.
        ///
        /// # Parameters
        ///
        /// - `i`: Index to remove element at.
        /// ----------------------------------------------------------------------------------------
        constexpr auto removeAt(usize i)
        {
            Contracts::DebugExpects(isIndexInRange(i), "Index is out of range.");

            _impl.removeAt(i);
        }

        /// ----------------------------------------------------------------------------------------
        /// Removes element at pos `it`.
        ///
        /// # Parameters
        ///
        /// - `it`: Pos to remove element at.
        ///
        /// # Returns
        ///
        /// [TMutIter] to next element. If `it` was pointing to the last element, returns
        /// [`iterEnd()`].
        /// ----------------------------------------------------------------------------------------
        constexpr auto removeAt(TIter it) -> TMutIter
        {
            Contracts::DebugExpects(isIterValid(it), "Invalid iter.");
            Contracts::DebugExpects(isIterInRange(it), "Iter is out of range.");

            usize i = indexForIter(it);
            _impl.removeAt(i);
            return _impl.mutIter(i);
        }

        /// ----------------------------------------------------------------------------------------
        /// Removes elements in range [[`from`, `to`]].
        ///
        /// # Parameters
        ///
        /// - `from`: Start of range to remove elements at.
        /// - `to`: End of range to remove elements at.
        ///
        /// # Returns
        ///
        /// `from`.
        ///
        /// ## Explanation
        ///
        /// After removing `to - from` elements, next elements will be shifted back to index `from`.
        /// So the next element will be available at index `from`. If the last element of the arr
        /// was also removed, `from` will be equal to [`count()`].
        /// ----------------------------------------------------------------------------------------
        constexpr auto removeRange(usize from, usize to) -> usize
        {
            Contracts::DebugExpects(from <= to, "Invalid range.");
            Contracts::DebugExpects(isIndexInRange(to), "Index was out of range.");
            // TODO: what should we do about fnret?
            // Contracts::DebugEnsures(fnret <= count(), "Invalid return value.");

            _impl.removeRange(from, to);
            return from;
        }

        /// ----------------------------------------------------------------------------------------
        /// Removes elements in range represented by `range`.
        ///
        /// # Parameters
        ///
        /// - `it`: Start of range to remove elements at.
        /// - `itEnd`: End of range to remove elements at.
        ///
        /// # Returns
        ///
        /// [`TMutIter`] to next element of the last removed element. If the last removed element
        /// was also the last element of the arr, returns [`iterEnd()`].
        /// ----------------------------------------------------------------------------------------
        constexpr auto removeRange(TIter it, TIter itEnd) -> TMutIter
        {
            Contracts::DebugExpects(isIterValid(it), "Invalid iter.");
            Contracts::DebugExpects(isIterValid(itEnd), "Invalid iter.");
            Contracts::DebugExpects(isIterInRange(it), "Iter is out range.");
            Contracts::DebugExpects(isIterInRange(itEnd), "Iter is out range.");
            Contracts::DebugExpects(it.compare(itEnd) <= 0, "Invalid range.");

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
            Contracts::DebugExpects(count_ <= count());

            _impl.removeFront(count_);
        }

        /// ----------------------------------------------------------------------------------------
        /// Removes `count_` elements from back.
        /// ----------------------------------------------------------------------------------------
        constexpr auto removeBack(usize count_ = 1)
        {
            Contracts::DebugExpects(count_ <= count());

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
            return _impl.releaseUnusedMem();
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

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto alloc() const -> const TAlloc&
        {
            return _impl.alloc();
        }

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
        Impl _impl;
    };

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename TElem, typename TAlloc = DefaultMemAllocator>
    class DynamicArray: public MutArrayRangeExtensions<BasicDynamicArray<TElem, TAlloc>>
    {
        using Base = MutArrayRangeExtensions<BasicDynamicArray<TElem, TAlloc>>;

    public:
        using Base::Base;
        using Base::operator=;
    };
}
