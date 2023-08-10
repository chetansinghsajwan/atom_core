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
        constexpr _ArrViewStorage()
            : _data{ nullptr }
            , _count{ 0 }
        {}

        constexpr _ArrViewStorage(const T* arr, usize count)
            : _data{ arr }
            , _count{ count }
        {}

    public:
        constexpr auto _Data() const -> const T*
        {
            return _data;
        }

        constexpr auto _Count() const -> usize
        {
            return _count;
        }

    public:
        constexpr auto getData() const -> const T*
        {
            return _data;
        }

        constexpr auto getData() -> T*
        {
            return _data;
        }

        constexpr auto setData(T* data)
        {
            _data = data;
        }

        constexpr auto getCount() const -> usize
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
    class ArrView
        : public _ArrViewStorage<T>
        , public ArrRangeTrait<ArrView<T>>
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
        constexpr ArrView(const T (&range)[size])
            : _Storage()
        {}

        /// ----------------------------------------------------------------------------------------
        /// # Range Constructor
        /// ----------------------------------------------------------------------------------------
        template <typename TRange>
        constexpr ArrView(const TRange& range)
            requires(RArrRangeOf<TRange, T>)
            : _Storage{ range.data(), range.count() }
        {}

        /// ----------------------------------------------------------------------------------------
        /// # Range Operator
        /// ----------------------------------------------------------------------------------------
        template <typename TRange>
        constexpr auto operator=(const TRange& range) -> ArrView& requires(RArrRangeOf<TRange, T>) {
            *this = ArrView{ range.data(), range.count() };
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
        constexpr auto getData() const -> const T*
        {
            return _storage().getData();
        }

        constexpr auto getCount() const -> usize
        {
            return _storage().getCount();
        }

    private:
        constexpr auto _storage() const -> const _Storage&
        {
            return reinterpret_cast<const _Storage&>(*this);
        }
    };
}
