#pragma once
// #include "atom/core/_std.h"
#include "atom/core/core.h"
// #include "atom/core/types.h"
#include "atom/core/range/array_iter.h"
#include "atom/core/range/range_literal.h"
#include "atom/core/range/iter_requirements.h"
#include "atom/core/range/range_requirements.h"
#include "atom/core/range/range_extensions.h"

namespace atom
{
    template <typename in_iter_t, typename in_iter_end_t>
    class _range_from_iter_pair: public range_extensions
    {
    public:
        using value_t = typename in_iter_t::value_t;
        using iter_t = in_iter_t;
        using iter_end_t = in_iter_end_t;

    public:
        constexpr _range_from_iter_pair(iter_t it, iter_end_t it_end)
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

        constexpr auto get_data() const -> const value_t*
            requires is_array_iter_pair<iter_t, iter_end_t>
        {
            return &_it.value();
        }

        constexpr auto get_count() const -> usize
            requires is_jump_iter_pair<iter_t, iter_end_t>
        {
            return _it_end.compare(_it);
        }

    private:
        iter_t _it;
        iter_end_t _it_end;
    };

    template <typename in_mut_iter_t, typename in_mut_iter_end_t>
    class _mut_range_from_iter_pair
        : public _range_from_iter_pair<in_mut_iter_t, in_mut_iter_end_t>
    {
        using base_t = _range_from_iter_pair<in_mut_iter_t, in_mut_iter_end_t>;

    public:
        using mut_iter_t = in_mut_iter_t;
        using mut_iter_end_t = in_mut_iter_end_t;

    public:
        constexpr _mut_range_from_iter_pair(mut_iter_t it, mut_iter_end_t it_end)
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
    template <typename value_t>
    constexpr auto range_from(std::initializer_list<value_t> list)
    {
        return _range_from_iter_pair(array_iter(list.begin()), array_iter(list.end()));
    }

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename value_t>
    constexpr auto range_from(const value_t* begin, const value_t* end)
    {
        return _range_from_iter_pair(array_iter(begin), array_iter(end));
    }

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename value_t>
    constexpr auto range_from(value_t* begin, value_t* end)
    {
        return _mut_range_from_iter_pair(mut_array_iter(begin), mut_array_iter(end));
    }

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename value_t>
    constexpr auto range_from(const value_t* begin, usize count)
    {
        return _range_from_iter_pair(array_iter(begin), array_iter(begin + count));
    }

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename value_t>
    constexpr auto range_from(value_t* begin, usize count)
    {
        return _mut_range_from_iter_pair(mut_array_iter(begin), mut_array_iter(begin + count));
    }

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename value_t, usize count>
    constexpr auto range_from(const value_t (&arr)[count])
    {
        return _range_from_iter_pair(array_iter(ptr(arr)), array_iter(ptr(arr) + count));
    }

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename value_t, usize count>
    constexpr auto range_from(value_t (&arr)[count])
    {
        return _mut_range_from_iter_pair(
            mut_array_iter(mut_ptr(arr)), mut_array_iter(mut_ptr(arr) + count));
    }

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    constexpr auto range_from(const char* str)
    {
        return _range_from_iter_pair(array_iter(str), array_iter(str + _range_find_str_len(str)));
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
        requires is_iter_pair<iter_t, iter_end_t>
    {
        if constexpr (is_mut_iter<iter_t>)
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
    template <typename value_t>
    constexpr auto range_from_literal(range_literal<value_t> lit)
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
