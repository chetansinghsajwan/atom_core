#pragma once
#include "Atom/Range.h"

namespace Atom
{
    template <typename T>
    class ArrView;

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    class _ArrViewStorage
    {
        friend class ArrRangeTraitImpl<ArrView<T>>;

    public:
        using TElem = T;

    public:
        constexpr _ArrViewStorage():
            _data{ nullptr }, _count{ 0 } { }

        constexpr _ArrViewStorage(const T* arr, usize count):
            _data{ arr }, _count{ count } { }

    public:
        constexpr fn _Data() const -> const T*
        {
            return _data;
        }

        constexpr fn _Count() const -> usize
        {
            return _count;
        }

    public:
        constexpr fn getData() const -> const T*
        {
            return _data;
        }

        constexpr fn getData() -> T*
        {
            return _data;
        }

        constexpr fn setData(T* data)
        {
            _data = data;
        }

        constexpr fn getCount() const -> usize
        {
            return _count;
        }

        constexpr fn setCount(usize count)
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
    class ArrView:
        public _ArrViewStorage<T>,
        public ArrRangeTrait<ArrView<T>>
    {
        using _Storage = _ArrViewStorage<T>;

    public:
        using TElem = T;

    public:
        /// ----------------------------------------------------------------------------------------
        /// # Default Constructor
        /// ----------------------------------------------------------------------------------------
        constexpr ArrView() = default;

        /// ----------------------------------------------------------------------------------------
        /// # Arr Constructor
        /// ----------------------------------------------------------------------------------------
        template <usize size>
        constexpr ArrView(const T(&range)[size]):
            _Storage() { }

        /// ----------------------------------------------------------------------------------------
        /// # Range Constructor
        /// ----------------------------------------------------------------------------------------
        template <typename TRange>
        constexpr ArrView(const TRange& range)
            requires(RArrRangeOf<TRange, T>):
            _Storage{ range.data(), range.count() } { }

        /// ----------------------------------------------------------------------------------------
        /// # Range Operator
        /// ----------------------------------------------------------------------------------------
        template <typename TRange>
        constexpr fn operator=(const TRange& range) -> ArrView&
            requires(RArrRangeOf<TRange, T>)
        {
            self = ArrView{ range.data(), range.count() };
        }
    };

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    class ArrRangeTraitImpl<ArrView<T>>
    {
        using _Storage = _ArrViewStorage<T>;

    public:
        using TElem = T;

    public:
        constexpr fn getData() const -> const T*
        {
            return _storage().getData();
        }

        constexpr fn getCount() const -> usize
        {
            return _storage().getCount();
        }

    private:
        constexpr fn _storage() const -> const _Storage&
        {
            return reinterpret_cast<const _Storage&>(self);
        }
    };
}