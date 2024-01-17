#pragma once
#include "atom/range.h"
#include "atom/text/basic_char.h"
#include "atom/text/basic_rune.h"

namespace atom
{
    template <typename tencoding_, typename tcontainer>
    class _string_impl: public tcontainer
    {
    public:
        using tencoding = tencoding_;

        using tchar = basic_char<tencoding>;
        using iter_type = typename tcontainer::iter_type;
        using iter_end_type = typename tcontainer::iter_end_type;
        // using mut_iter_type             = typename tcontainer::mut_iter_type;
        // using mut_iter_end_type          = typename tcontainer::mut_iter_end_type;

        using trune = basic_rune<tencoding>;
        // using trune_iter            = typename tcontainer::trune_iter;
        // using trune_iter_end         = typename tcontainer::trune_iter_end;
        // using tmut_rune_iter         = typename tcontainer::tmut_rune_iter;
        // using tmut_rune_iter_end      = typename tcontainer::tmut_rune_iter_end;

    public:
        using tcontainer::tcontainer;
        using tcontainer::operator=;

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <usize count>
        constexpr _string_impl(const tchar (&arr)[count])
            : tcontainer()
        {}

    public:
        constexpr auto to_std_char_ptr() const -> const char*
        {
            return _to_std_char_ptr(this->data().unwrap());
        }

        constexpr auto to_std_char_ptr() -> char*
        {
            return _to_std_char_ptr(this->mut_data().unwrap());
        }
    };
}
