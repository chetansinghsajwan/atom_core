#pragma once
#include "Atom/Range/ArrayIter.h"
#include "Atom/Range/MutArrayRangeExtensions.h"
#include "Atom/Range/RangeReq.h"
#include "Atom/TTI.h"

#include <cstring>

namespace Atom
{
    template <typename TIter_, typename TIterEnd_>
    class _BasicRangeFromIterPair
    {
    public:
        using TElem = typename TIter_::TElem;
        using TIter = TIter_;
        using TIterEnd = TIterEnd_;

    public:
        constexpr _BasicRangeFromIterPair(TIter it, TIterEnd itEnd):
            _it{ mov(it) }, _itEnd{ mov(itEnd) }
        {}

    public:
        constexpr auto iter() const -> TIter
        {
            return _it;
        }

        constexpr auto iterEnd() const -> TIter
        {
            return _itEnd;
        }

        constexpr auto data() const -> const TElem*
            requires RArrayIterPair<TIter, TIterEnd>
        {
            return &_it.value();
        }

        constexpr auto count() const -> usize
            requires RJumpIterPair<TIter, TIterEnd>
        {
            return _itEnd.compare(_it).template to<usize>();
        }

    private:
        TIter _it;
        TIterEnd _itEnd;
    };

    template <typename TMutIter_, typename TMutIterEnd_>
    class _BasicMutRangeFromIterPair: public _BasicRangeFromIterPair<TMutIter_, TMutIterEnd_>
    {
        using Base = _BasicRangeFromIterPair<TMutIter_, TMutIterEnd_>;

    public:
        using TMutIter = TMutIter_;
        using TMutIterEnd = TMutIterEnd_;

    public:
        constexpr _BasicMutRangeFromIterPair(TMutIter it, TMutIterEnd itEnd):
            Base(mov(it), mov(itEnd))
        {}

    public:
        constexpr auto mutIter() -> TMutIter
        {
            return this->iter();
        }

        constexpr auto mutIterEnd() -> TMutIterEnd
        {
            return this->iterEnd();
        }
    };

    template <typename TIter, typename TIterEnd>
    class _RangeFromIterExtended
    {
    private:
        template <typename T_>
        class _TypeContainer
        {
        public:
            using T = TTI::TRemoveCVRef<T_>;
        };

        static consteval auto _Get()
        {
            using TRange = _BasicRangeFromIterPair<TIter, TIterEnd>;

            if constexpr (RArrayIterPair<TIter, TIterEnd>)
                return _TypeContainer<ArrayRangeExtensions<TRange>>();

            else if constexpr (RJumpIterPair<TIter, TIterEnd>)
                return _TypeContainer<JumpRangeExtensions<TRange>>();

            else if constexpr (RBidiIterPair<TIter, TIterEnd>)
                return _TypeContainer<BidiRangeExtensions<TRange>>();

            else if constexpr (RFwdIterPair<TIter, TIterEnd>)
                return _TypeContainer<FwdRangeExtensions<TRange>>();

            else if constexpr (RIterPair<TIter, TIterEnd>)
                return _TypeContainer<RangeExtensions<TRange>>();
        }

    public:
        using T = typename decltype(_Get())::T;
    };

    template <typename TIter, typename TIterEnd>
    class _MutRangeFromIterExtended
    {
    private:
        template <typename T_>
        class _TypeContainer
        {
        public:
            using T = TTI::TRemoveCVRef<T_>;
        };

        static consteval auto _Get()
        {
            using TRange = _BasicMutRangeFromIterPair<TIter, TIterEnd>;

            if constexpr (RArrayIterPair<TIter, TIterEnd>)
                return _TypeContainer<MutArrayRangeExtensions<TRange>>();

            else if constexpr (RJumpIterPair<TIter, TIterEnd>)
                return _TypeContainer<MutJumpRangeExtensions<TRange>>();

            else if constexpr (RBidiIterPair<TIter, TIterEnd>)
                return _TypeContainer<MutBidiRangeExtensions<TRange>>();

            else if constexpr (RFwdIterPair<TIter, TIterEnd>)
                return _TypeContainer<MutFwdRangeExtensions<TRange>>();

            else if constexpr (RIterPair<TIter, TIterEnd>)
                return _TypeContainer<MutRangeExtensions<TRange>>();
        }

    public:
        using T = typename decltype(_Get())::T;
    };

    template <typename TIter, typename TIterEnd>
    class _RangeFromIterPair: public _RangeFromIterExtended<TIter, TIterEnd>::T
    {
        using Base = _RangeFromIterExtended<TIter, TIterEnd>::T;

    public:
        constexpr _RangeFromIterPair(TIter it, TIterEnd itEnd):
            Base(mov(it), mov(itEnd))
        {}
    };

    template <typename TMutIter, typename TMutIterEnd>
    class _MutRangeFromIterPair: public _MutRangeFromIterExtended<TMutIter, TMutIterEnd>::T
    {
        using Base = _MutRangeFromIterExtended<TMutIter, TMutIterEnd>::T;

    public:
        constexpr _MutRangeFromIterPair(TMutIter it, TMutIterEnd itEnd):
            Base(mov(it), mov(itEnd))
        {}
    };

    /// --------------------------------------------------------------------------------------------
    /// # To Do
    ///
    /// - Review this implementation after implementing character encoding.
    /// --------------------------------------------------------------------------------------------
    constexpr auto _RangeFindStrLen(const char* str) -> usize
    {
        if (std::is_constant_evaluated())
        {
            usize len = 0;
            while (*str != '\0')
            {
                str++;
                len++;
            }

            return len;
        }

        return usize(std::strlen(str));
    }
}

namespace Atom
{
    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    constexpr auto MakeRange(std::initializer_list<T> list)
    {
        return _RangeFromIterPair(ArrayIter(list.begin()), ArrayIter(list.end()));
    }

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    constexpr auto MakeRange(const T* begin, const T* end)
    {
        return _RangeFromIterPair(ArrayIter(begin), ArrayIter(end));
    }

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    constexpr auto MakeRange(T* begin, T* end)
    {
        return _MutRangeFromIterPair(MutArrayIter(begin), MutArrayIter(end));
    }

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    constexpr auto MakeRange(const T* begin, usize count)
    {
        return _RangeFromIterPair(ArrayIter(begin), ArrayIter(begin + count));
    }

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    constexpr auto MakeRange(T* begin, usize count)
    {
        return _MutRangeFromIterPair(MutArrayIter(begin), MutArrayIter(begin + count));
    }

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename T, usize count>
    constexpr auto MakeRange(const T (&arr)[count])
    {
        return _RangeFromIterPair(ArrayIter(arr), ArrayIter(arr + count));
    }

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename T, usize count>
    constexpr auto MakeRange(T (&arr)[count])
    {
        return _MutRangeFromIterPair(MutArrayIter(arr), MutArrayIter(arr + count));
    }

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    constexpr auto MakeRange(const char* str)
    {
        return _RangeFromIterPair(ArrayIter(str), ArrayIter(str + _RangeFindStrLen(str)));
    }

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    constexpr auto MakeRange(char* str)
    {
        return _MutRangeFromIterPair(MutArrayIter(str), MutArrayIter(str + _RangeFindStrLen(str)));
    }

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename TIter, typename TIterEnd>
    constexpr auto MakeRange(TIter it, TIterEnd itEnd)
        requires RIterPair<TIter, TIterEnd>
    {
        if constexpr (RMutIter<TIter>)
        {
            return _MutRangeFromIterPair(mov(it), mov(itEnd));
        }
        else
        {
            return _RangeFromIterPair(mov(it), mov(itEnd));
        }
    }
}
