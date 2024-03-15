#pragma once
#include "atom/core/_std.h"
#include "atom/core/core.h"
#include "atom/core/typeinfo.h"
#include "atom/core/range/array_iter.h"
#include "atom/core/range/range_literal.h"
#include "atom/core/range/iter_requirements.h"
#include "atom/core/range/range_requirements.h"
#include "atom/core/range/range_extensions.h"
#include "atom/core/range/fwd_range_extensions.h"
#include "atom/core/range/bidi_range_extensions.h"
#include "atom/core/range/jump_range_extensions.h"
#include "atom/core/range/array_range_extensions.h"
#include "atom/core/range/mut_range_extensions.h"
#include "atom/core/range/mut_fwd_range_extensions.h"
#include "atom/core/range/mut_bidi_range_extensions.h"
#include "atom/core/range/mut_jump_range_extensions.h"
#include "atom/core/range/mut_array_range_extensions.h"

namespace atom
{
    template <typename in_iter_t, typename in_iter_end_t>
    class _basic_range_from_iter_pair
    {
    public:
        using elem_t = typename in_iter_t::elem_t;
        using iter_t = in_iter_t;
        using iter_end_t = in_iter_end_t;

    public:
        constexpr _basic_range_from_iter_pair(iter_t it, iter_end_t it_end)
            : _it(move(it))
            , _it_end(move(it_end))
        {}

    public:
        constexpr auto get_iter() const -> iter_t
        {
            return _it;
        }

        constexpr auto get_iter_end() const -> iter_t
        {
            return _it_end;
        }

        constexpr auto get_data() const -> const elem_t*
            requires rarray_iter_pair<iter_t, iter_end_t>
        {
            return &_it.value();
        }

        constexpr auto get_count() const -> usize
            requires rjump_iter_pair<iter_t, iter_end_t>
        {
            return _it_end.compare(_it);
        }

    private:
        iter_t _it;
        iter_end_t _it_end;
    };

    template <typename in_mut_iter_t, typename in_mut_iter_end_t>
    class _basic_mut_range_from_iter_pair
        : public _basic_range_from_iter_pair<in_mut_iter_t, in_mut_iter_end_t>
    {
        using base_t = _basic_range_from_iter_pair<in_mut_iter_t, in_mut_iter_end_t>;

    public:
        using mut_iter_t = in_mut_iter_t;
        using mut_iter_end_t = in_mut_iter_end_t;

    public:
        constexpr _basic_mut_range_from_iter_pair(mut_iter_t it, mut_iter_end_t it_end)
            : base_t(move(it), move(it_end))
        {}

    public:
        constexpr auto get_mut_iter() -> mut_iter_t
        {
            return this->get_iter();
        }

        constexpr auto get_mut_iter_end() -> mut_iter_end_t
        {
            return this->get_iter_end();
        }
    };

    template <typename iter_t, typename iter_end_t>
    class _range_from_iter_extended
    {
    private:
        template <typename in_elem_t>
        class _type_container
        {
        public:
            using elem_t = typeinfo::remove_cvref_t<in_elem_t>;
        };

        static consteval auto _get()
        {
            using range_t = _basic_range_from_iter_pair<iter_t, iter_end_t>;

            if constexpr (rarray_iter_pair<iter_t, iter_end_t>)
                return _type_container<array_range_extensions<range_t>>();

            else if constexpr (rjump_iter_pair<iter_t, iter_end_t>)
                return _type_container<jump_range_extensions<range_t>>();

            else if constexpr (rbidi_iter_pair<iter_t, iter_end_t>)
                return _type_container<bidi_range_extensions<range_t>>();

            else if constexpr (rfwd_iter_pair<iter_t, iter_end_t>)
                return _type_container<fwd_range_extensions<range_t>>();

            else if constexpr (riter_pair<iter_t, iter_end_t>)
                return _type_container<range_extensions<range_t>>();
        }

    public:
        using elem_t = typename decltype(_get())::elem_t;
    };

    template <typename iter_t, typename iter_end_t>
    class _mut_range_from_iter_extended
    {
    private:
        template <typename in_elem_t>
        class _type_container
        {
        public:
            using elem_t = typeinfo::remove_cvref_t<in_elem_t>;
        };

        static consteval auto _get()
        {
            using range_t = _basic_mut_range_from_iter_pair<iter_t, iter_end_t>;

            if constexpr (rarray_iter_pair<iter_t, iter_end_t>)
                return _type_container<mut_array_range_extensions<range_t>>();

            else if constexpr (rjump_iter_pair<iter_t, iter_end_t>)
                return _type_container<mut_jump_range_extensions<range_t>>();

            else if constexpr (rbidi_iter_pair<iter_t, iter_end_t>)
                return _type_container<mut_bidi_range_extensions<range_t>>();

            else if constexpr (rfwd_iter_pair<iter_t, iter_end_t>)
                return _type_container<mut_fwd_range_extensions<range_t>>();

            else if constexpr (riter_pair<iter_t, iter_end_t>)
                return _type_container<mut_range_extensions<range_t>>();
        }

    public:
        using elem_t = typename decltype(_get())::elem_t;
    };

    template <typename iter_t, typename iter_end_t>
    class _range_from_iter_pair
        : public _range_from_iter_extended<iter_t, iter_end_t>::elem_t
    {
        using base_t = _range_from_iter_extended<iter_t, iter_end_t>::elem_t;

    public:
        constexpr _range_from_iter_pair(iter_t it, iter_end_t it_end)
            : base_t(move(it), move(it_end))
        {}
    };

    template <typename mut_iter_t, typename mut_iter_end_t>
    class _mut_range_from_iter_pair
        : public _mut_range_from_iter_extended<mut_iter_t, mut_iter_end_t>::elem_t
    {
        using base_t =
            _mut_range_from_iter_extended<mut_iter_t, mut_iter_end_t>::elem_t;

    public:
        constexpr _mut_range_from_iter_pair(mut_iter_t it, mut_iter_end_t it_end)
            : base_t(move(it), move(it_end))
        {}
    };

    /// --------------------------------------------------------------------------------------------
    /// # to do
    ///
    /// - review this implementation after implementing character encoding.
    /// --------------------------------------------------------------------------------------------
    constexpr auto _range_find_str_len(const char* str) -> usize
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

        return std::strlen(str);
    }
}

namespace atom
{
    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename elem_t>
    constexpr auto range_from(std::initializer_list<elem_t> list)
    {
        return _range_from_iter_pair(array_iter(list.begin()), array_iter(list.end()));
    }

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename elem_t>
    constexpr auto range_from(const elem_t* begin, const elem_t* end)
    {
        return _range_from_iter_pair(array_iter(begin), array_iter(end));
    }

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename elem_t>
    constexpr auto range_from(elem_t* begin, elem_t* end)
    {
        return _mut_range_from_iter_pair(mut_array_iter(begin), mut_array_iter(end));
    }

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename elem_t>
    constexpr auto range_from(const elem_t* begin, usize count)
    {
        return _range_from_iter_pair(array_iter(begin), array_iter(begin + count));
    }

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename elem_t>
    constexpr auto range_from(elem_t* begin, usize count)
    {
        return _mut_range_from_iter_pair(mut_array_iter(begin), mut_array_iter(begin + count));
    }

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename elem_t, usize count>
    constexpr auto range_from(const elem_t (&arr)[count])
    {
        return _range_from_iter_pair(array_iter(ptr(arr)), array_iter(ptr(arr) + count));
    }

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename elem_t, usize count>
    constexpr auto range_from(elem_t (&arr)[count])
    {
        return _mut_range_from_iter_pair(
            mut_array_iter(mut_ptr(arr)), mut_array_iter(mut_ptr(arr) + count));
    }

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    constexpr auto range_from(const char* str)
    {
        return _range_from_iter_pair(
            array_iter(str), array_iter(str + _range_find_str_len(str)));
    }

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    constexpr auto range_from(char* str)
    {
        return _mut_range_from_iter_pair(
            mut_array_iter(str), mut_array_iter(str + _range_find_str_len(str)));
    }

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename iter_t, typename iter_end_t>
    constexpr auto range_from(iter_t it, iter_end_t it_end)
        requires riter_pair<iter_t, iter_end_t>
    {
        if constexpr (rmut_iter<iter_t>)
        {
            return _mut_range_from_iter_pair(move(it), move(it_end));
        }
        else
        {
            return _range_from_iter_pair(move(it), move(it_end));
        }
    }

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename elem_t>
    constexpr auto range_from_literal(range_literal<elem_t> lit)
    {
        return range_from(lit.get_data(), lit.get_count());
    }

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    constexpr auto range_from(std::string_view str)
    {
        return range_from(str.data(), str.size());
    }
}
