#pragma once
#include "Atom/Range.h"

namespace Atom
{
    template <typename T>
    class ArrView;

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <tname T>
    class _ArrViewStorage
    {
        friend class ArrRangeTraitImpl<ArrView<T>>;

    public:
        using TElem = T;

    public:
        constexpr ctor _ArrViewStorage():
            _data{ nullptr }, _count{ 0 } { }

        constexpr ctor _ArrViewStorage(const T* arr, usize count):
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
    template <tname T>
    class ArrView:
        public _ArrViewStorage<T>,
        public ArrRangeTrait<ArrView<T>>
    {
        using _Storage = _ArrViewStorage<T>;

    public:
        /// ----------------------------------------------------------------------------------------
        /// # Default Constructor
        /// ----------------------------------------------------------------------------------------
        constexpr ctor ArrView() = default;

        /// ----------------------------------------------------------------------------------------
        /// # Range Constructor
        /// ----------------------------------------------------------------------------------------
        template <tname TRange>
        constexpr ctor ArrView(const TRange& range)
            requires(RArrRangeOf<TRange, T>):
            _Storage{ range.Data(), range.Count() } { }

        /// ----------------------------------------------------------------------------------------
        /// # Range Operator
        /// ----------------------------------------------------------------------------------------
        template <tname TRange>
        constexpr fn op=(const TRange& range) -> ArrView&
            requires(RArrRangeOf<TRange, T>)
        {
            *this = ArrView{ range.Data(), range.Count() };
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
            return reinterpret_cast<const _Storage&>(*this);
        }
    };
}