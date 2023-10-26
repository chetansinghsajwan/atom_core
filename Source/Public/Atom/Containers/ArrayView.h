#pragma once
#include "Atom/Range.h"

namespace Atom
{
    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    class _ArrayViewStorage
    {
    public:
        using TElem = T;

    public:
        constexpr _ArrayViewStorage(const T* arr, usize count):
            _data{ arr }, _count{ count }
        {}

    public:
        constexpr auto data() const -> const T*
        {
            return _data;
        }

        constexpr auto setData(T* data)
        {
            _data = data;
        }

        constexpr auto count() const -> usize
        {
            return _count;
        }

        constexpr auto setCount(usize count)
        {
            _count = count;
        }

    protected:
        const T* _data;
        usize _count;
    };

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    class ArrayView: public ArrayRangeTrait<ArrayView<T>>
    {
        friend class ArrayRangeTraitImpl<ArrayView<T>>;

    private:
        using This = ArrayView;
        using _Storage = _ArrayViewStorage<T>;

    public:
        using TElem = T;

    public:
        /// ----------------------------------------------------------------------------------------
        /// # Default Constructor
        /// ----------------------------------------------------------------------------------------
        constexpr ArrayView():
            _storage(nullptr, 0)
        {}

        /// ----------------------------------------------------------------------------------------
        /// # Trivial Copy Constructor
        /// ----------------------------------------------------------------------------------------
        constexpr ArrayView(const This& that) = default;

        /// ----------------------------------------------------------------------------------------
        /// # Trivial Copy Operator
        /// ----------------------------------------------------------------------------------------
        constexpr ArrayView& operator=(const This& that) = default;

        /// ----------------------------------------------------------------------------------------
        /// # Trivial Move Constructor
        /// ----------------------------------------------------------------------------------------
        constexpr ArrayView(This&& that) = default;

        /// ----------------------------------------------------------------------------------------
        /// # Trivial Move Operator
        /// ----------------------------------------------------------------------------------------
        constexpr ArrayView& operator=(This&& that) = default;

        /// ----------------------------------------------------------------------------------------
        /// # Array Constructor
        /// ----------------------------------------------------------------------------------------
        template <usize count>
        constexpr ArrayView(const T (&arr)[count]):
            _storage(arr, count)
        {}

        /// ----------------------------------------------------------------------------------------
        /// # Array Operator
        /// ----------------------------------------------------------------------------------------
        template <usize count>
        constexpr ArrayView& operator=(const T (&arr)[count])
        {
            _storage.setData(arr);
            _storage.setCount(count);
        }

        /// ----------------------------------------------------------------------------------------
        /// # Range Constructor
        /// ----------------------------------------------------------------------------------------
        template <typename TRange>
        constexpr ArrayView(const TRange& range)
            requires(RArrayRangeOf<TRange, T>)
            :
            _storage{ range.data(), range.count() }
        {}

        /// ----------------------------------------------------------------------------------------
        /// # Range Operator
        /// ----------------------------------------------------------------------------------------
        template <typename TRange>
        constexpr ArrayView& operator=(const TRange& range)
            requires(RArrayRangeOf<TRange, T>)
        {
            _storage.setData(range.data());
            _storage.setCount(range.count());
        }

        /// ----------------------------------------------------------------------------------------
        /// # Trivial Destructor
        /// ----------------------------------------------------------------------------------------
        constexpr ~ArrayView() = default;

    private:
        _Storage _storage;
    };

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    class ArrayRangeTraitImpl<ArrayView<T>>
    {
        using _Array = ArrayView<T>;
        using _ArrayStorage = _ArrayViewStorage<T>;

    public:
        using TElem = T;
        using TIter = ArrayIter<T>;
        using TIterEnd = TIter;

    public:
        constexpr auto data() const -> const T*
        {
            return _storage().data();
        }

        constexpr auto count() const -> usize
        {
            return _storage().count();
        }

        constexpr auto iter() const -> TIter
        {
            return TIter{ _storage().data() };
        }

        constexpr auto iterEnd() const -> TIterEnd
        {
            return TIterEnd{ _storage().data() + _storage().count() };
        }

    private:
        constexpr auto _storage() const -> const _ArrayStorage&
        {
            return reinterpret_cast<const _Array&>(*this)._storage;
        }
    };
}
