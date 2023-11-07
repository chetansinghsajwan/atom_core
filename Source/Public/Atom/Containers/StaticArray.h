#pragma once
#include "Atom/Range/MutArrayRangeExtensions.h"

namespace Atom
{
    template <typename TElem_, usize count_>
    class BasicStaticArray
    {
    public:
        using TElem = TElem_;
        using TIter = ArrayIter<TElem>;
        using TIterEnd = TIter;
        using TMutIter = MutArrayIter<TElem>;
        using TMutIterEnd = TMutIter;

    public:
        constexpr BasicStaticArray() = default;

        template <usize n>
        constexpr BasicStaticArray(const TElem (&arr)[n])
            requires(n <= count_):
            _arr{ arr }
        {}

        template <typename... TArgs>
        constexpr BasicStaticArray(TArgs&&... args)
            requires(RConvertibleTo<TArgs, TElem> and ...) and (sizeof...(TArgs) <= count_.val()):
            _arr{ 0 } { }

    public:
        constexpr auto data() const -> MemPtr<const TElem>
        {
            return _arr;
        }

        constexpr auto mutData() -> MemPtr<TElem>
        {
            return _arr;
        }

        constexpr auto count() const -> usize
        {
            return count_;
        }

        constexpr auto iter() const -> TIter
        {
            return TIter{ _arr };
        }

        constexpr auto iterEnd() const -> TIterEnd
        {
            return TIterEnd{ _arr + count_ };
        }

        constexpr auto mutIter() -> TMutIter
        {
            return TMutIter{ _arr };
        }

        constexpr auto mutIterEnd() -> TMutIterEnd
        {
            return TMutIterEnd{ _arr + count_ };
        }

    private:
        TElem _arr[count_.val()];
    };

    template <typename TElem, usize count>
    class StaticArray: public MutArrayRangeExtensions<BasicStaticArray<TElem, count>>
    {
        using Base = MutArrayRangeExtensions<BasicStaticArray<TElem, count>>;

    public:
        using Base::Base;
        using Base::operator=;
    };
}
