#pragma once
#include "_ConstArrImplHelper.decl.h"

namespace Atom
{
    template <typename T>
    struct _ArrViewImplBase
    {
    public:
        using TElem = T;

    public:
        constexpr _ArrViewImplBase() noexcept = default;

        constexpr _ArrViewImplBase(NullPtr) noexcept:
            _arr{ nullptr }, _count{ 0 } { }

        constexpr _ArrViewImplBase(const T* arr, usize count) noexcept:
            _arr{ arr }, _count{ count } { }

    public:
        constexpr auto _Data() const noexcept
            -> const T*
        {
            return _arr;
        }

        constexpr auto _Count() const noexcept
            -> usize
        {
            return _count;
        }

    protected:
        const T* _arr;
        usize _count;
    };

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    class ArrView: public _ConstArrImplHelper<_ArrViewImplBase<T>>
    {
        prim using _TBase = _ConstArrImplHelper<_ArrViewImplBase<T>>;
        prim using _ImplBase = _ArrViewImplBase<T>;

    public:
        /// ----------------------------------------------------------------------------------------
        /// DefCtor.
        /// ----------------------------------------------------------------------------------------
        constexpr ArrView() noexcept = default;

        /// ----------------------------------------------------------------------------------------
        /// NullCtor.
        /// ----------------------------------------------------------------------------------------
        constexpr ArrView(NullPtr) noexcept:
            _TBase{_ImplBase{ nullptr }} { }

        /// ----------------------------------------------------------------------------------------
        /// NullOper.
        /// ----------------------------------------------------------------------------------------
        constexpr ArrView& operator =(NullPtr) noexcept
        {
            *this = ArrView(nullptr);
        }

        /// ----------------------------------------------------------------------------------------
        /// ParamCtor.
        /// ----------------------------------------------------------------------------------------
        template <typename TRange>
        requires RArrRange<TRange, T>
        constexpr ArrView(const TRange& range) noexcept:
            _TBase{_ImplBase{ range.Data(), range.Count() }} { }

        /// ----------------------------------------------------------------------------------------
        /// ParamOper.
        /// ----------------------------------------------------------------------------------------
        template <typename TRange>
        requires RArrRange<TRange, T>
        constexpr ArrView& operator =(const TRange& range) noexcept
        {
            *this = ArrView{ range.Data(), range.Count() };
        }
    };
}