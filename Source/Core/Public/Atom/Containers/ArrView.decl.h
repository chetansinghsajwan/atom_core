#pragma once
#include "_ConstArrImplHelper.decl.h"

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
        friend class RangeTraitImpl<ArrView<T>>;

    public:
        using TElem = T;

    public:
        constexpr ctor _ArrViewStorage() = default;

        constexpr ctor _ArrViewStorage(NullPtr):
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
        public _ConstArrImplHelper<_ArrViewStorage<T>>,
        public RangeTrait<ArrView<T>>
    {
    private:
        using Base = _ConstArrImplHelper<_ArrViewStorage<T>>;
        using BaseImpl = _ArrViewStorage<T>;

    public:
        /// ----------------------------------------------------------------------------------------
        /// DefCtor.
        /// ----------------------------------------------------------------------------------------
        constexpr ctor ArrView() = default;

        /// ----------------------------------------------------------------------------------------
        /// NullCtor.
        /// ----------------------------------------------------------------------------------------
        constexpr ctor ArrView(NullPtr):
            Base{ BaseImpl{ nullptr } } { }

        /// ----------------------------------------------------------------------------------------
        /// NullOper.
        /// ----------------------------------------------------------------------------------------
        constexpr fn op=(NullPtr) -> ArrView&
        {
            *this = ArrView(nullptr);
        }

        /// ----------------------------------------------------------------------------------------
        /// ParamCtor.
        /// ----------------------------------------------------------------------------------------
        template <tname TRange>
        constexpr ctor ArrView(const TRange& range)
            requires(RArrRangeOf<TRange, T>):
            Base{ BaseImpl{ range.Data(), range.Count() } } { }

        /// ----------------------------------------------------------------------------------------
        /// ParamOper.
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
    class RangeTraitImpl<ArrView<T>>
    {
        using _Storage = _ArrViewStorage<T>;

    public:
        using TElem = T;
        using TIter = ArrIter<T>;
        using TIterEnd = TIter;

    public:
        constexpr fn iter() const -> TIter
        {
            return TIter{ _storage().getData() };
        }

        constexpr fn iterEnd() const -> TIterEnd
        {
            return TIterEnd{ _storage().getData() + _storage().getCount() };
        }

    private:
        constexpr fn _storage() const -> const _Storage&
        {
            return reinterpret_cast<const _Storage&>(*this);
        }
    };
}