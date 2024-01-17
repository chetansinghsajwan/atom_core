#pragma once
#include "ArrayIter.h"
#include "RangeReq.h"
#include "StdIterWrapForAtomIter.h"

// #include <algorithm>

namespace Atom
{
    template <typename TRange>
    class _RangeExtensionsImpl
    {
    protected:
        using _TImpl = TRange;

    public:
        using TElem = typename _TImpl::TElem;
        using TIter = typename _TImpl::TIter;
        using TIterEnd = typename _TImpl::TIterEnd;

    public:
        constexpr _RangeExtensionsImpl(TRange& range)
            : _range_(range)
        {}

    public:
        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto iter() const -> TIter
        {
            return _range().iter();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto iterEnd() const -> TIterEnd
        {
            return _range().iterEnd();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename TElem1>
        constexpr auto findElem(const TElem1& el) const -> TIter
        {
            StdIterWrapForAtomIter stdIter{ iter() };
            StdIterWrapForAtomIter stdIterEnd{ iterEnd() };
            return std::find(stdIter, stdIterEnd, el).iter;
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename TRange1>
        constexpr auto findRange(const TRange1& range) const -> TIter
        {
            StdIterWrapForAtomIter stdIter{ iter() };
            StdIterWrapForAtomIter stdIterEnd{ iterEnd() };
            StdIterWrapForAtomIter stdIter1{ range.iter() };
            StdIterWrapForAtomIter stdIterEnd1{ range.iterEnd() };
            return std::find(stdIter, stdIterEnd, stdIter1, stdIterEnd1);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename TRange1>
        auto compare(const TRange1& range) const -> i8
        {
            StdIterWrapForAtomIter stdIter{ iter() };
            StdIterWrapForAtomIter stdIterEnd{ iterEnd() };
            StdIterWrapForAtomIter stdIter1{ range.iter() };
            StdIterWrapForAtomIter stdIterEnd1{ range.iterEnd() };

            return std::equal(stdIter, stdIterEnd, stdIter1, stdIterEnd1);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto getCount() const -> usize
        {
            if constexpr (RJumpIterPair<TIter, TIterEnd>)
            {
                return iterEnd() - iter();
            }

            usize count = 0;
            for (auto it = iter(); it != iterEnd(); it++)
                count++;

            return count;
        }

    protected:
        constexpr auto _range() const -> const TRange&
        {
            return _range_;
        }

        constexpr auto _range() -> TRange&
        {
            return _range_;
        }

    private:
        TRange& _range_;
    };

    template <typename TRange, typename _TRangeExtensionsImpl = void>
    class RangeExtensions: public TRange
    {
        using This = RangeExtensions;
        using Base = TRange;

    protected:
        using _TImpl = _TRangeExtensionsImpl;

    public:
        using TElem = typename _TImpl::TElem;
        using TIter = typename _TImpl::TIter;
        using TIterEnd = typename _TImpl::TIterEnd;

    public:
        using Base::Base;
        using Base::operator=;

    public:
        ////////////////////////////////////////////////////////////////////////////////////////////////
        ////
        //// Iteration
        ////
        ////////////////////////////////////////////////////////////////////////////////////////////////

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto iter() const -> TIter
        {
            return _impl().iter();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto iterEnd() const -> TIterEnd
        {
            return _impl().iterEnd();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto begin() const
        {
            return StdIterWrapForAtomIter{ _impl().iter() };
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto end() const
        {
            return StdIterWrapForAtomIter{ _impl().iterEnd() };
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////
        ////
        //// Search
        ////
        ////////////////////////////////////////////////////////////////////////////////////////////////

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename TElem1>
        constexpr auto find(const TElem1& el) const -> TIter
            requires(REqualityComparableWith<TElem, TElem1>)
        {
            return _impl().findElem(el);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename TRange1>
        constexpr auto findRange(const TRange1& range) const -> TIter
            requires(RFwdRange<TRange1>)
                    and (REqualityComparableWith<TElem, typename TRange1::TElem>)
        {
            return _impl().findRange(range);
        }

        template <typename TRange1>
        auto countAny(const TRange1& range) const -> usize
        {
            usize count = 0;
            for (auto it = iter(); it.compare(iterEnd()) != 0; it++)
                for (const auto& el : range)
                {
                    if (*it == el)
                        count++;
                }

            return count;
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename TElem1>
        constexpr auto contains(const TElem1& el) const -> bool
            requires(REqualityComparableWith<TElem, TElem1>)
        {
            return _impl().findElem(el).compare(_impl().iterEnd()) != 0;
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename TRange1>
        constexpr auto contains(const TRange1& range) const -> bool
            requires(RFwdRange<TRange1>)
                    and (REqualityComparableWith<TElem, typename TRange1::TElem>)
        {
            return _impl().findRange(range).compare(_impl().iterEnd()) != 0;
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////
        ////
        //// Comparision
        ////
        ////////////////////////////////////////////////////////////////////////////////////////////////

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename TRange1>
        constexpr auto compare(const TRange1& range) const -> i8
            requires(RRange<TRange1>) and (REqualityComparableWith<TElem, typename TRange1::TElem>)
        {
            return _impl().compare(range);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename TRange1>
        constexpr auto eq(const TRange1& range) const -> bool
            requires(RRange<TRange1>) and (REqualityComparableWith<TElem, typename TRange1::TElem>)
        {
            return _impl().compare(range) == 0;
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename TRange1>
        constexpr auto ne(const TRange1& range) const -> bool
            requires(RRange<TRange1>) and (REqualityComparableWith<TElem, typename TRange1::TElem>)
        {
            return not eq(range);
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////
        ////
        //// Helpers
        ////
        //// # To Do
        //// - Add range helper functions.
        //// -------------------------------------------------------------------------------------------

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto canGetCount() const -> bool
        {
            return RFwdIterPair<TIter, TIterEnd>;
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto getCount() const -> usize
        {
            return _impl().getCount();
        }

    protected:
        constexpr auto _impl() const -> const _TImpl
        {
            return _TImpl(const_cast<This&>(*this));
        }

        constexpr auto _impl() -> _TImpl
        {
            return _TImpl(*this);
        }
    };

    template <typename TRange>
    class RangeExtensions<TRange, void>
        : public RangeExtensions<TRange, _RangeExtensionsImpl<TRange>>
    {
        using Base = RangeExtensions<TRange, _RangeExtensionsImpl<TRange>>;

    public:
        using Base::Base;
        using Base::operator=;
    };
}
