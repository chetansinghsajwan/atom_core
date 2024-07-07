export module atom_core:ranges.range_conversions;

import std;
import :core;
import :ranges.array_iterator;
import :ranges.iterator_concepts;
import :ranges.range_concepts;

namespace atom::ranges
{
    template <typename in_const_iterator_type, typename in_const_iterator_end_type>
    class _const_range_from_iterator_pair
    {
    public:
        using value_type = typename in_const_iterator_type::value_type;
        using const_iterator_type = in_const_iterator_type;
        using const_iterator_end_type = in_const_iterator_end_type;

    public:
        constexpr _const_range_from_iterator_pair(
            const_iterator_type it, const_iterator_end_type it_end)
            : _it{ move(it) }
            , _it_end{ move(it_end) }
        {}

    public:
        constexpr auto get_iterator() const -> const_iterator_type
        {
            return _it;
        }

        constexpr auto get_iterator_end() const -> const_iterator_type
        {
            return _it_end;
        }

        constexpr auto get_data() const
            -> const value_type* requires(
                const_array_iterator_pair_concept<const_iterator_type, const_iterator_end_type>) {
            return &*_it;
        }

        constexpr auto get_count() const -> usize
            requires(const_random_access_iterator_pair_concept<const_iterator_type,
                const_iterator_end_type>)
        {
            return _it_end - _it;
        }

    private:
        const_iterator_type _it;
        const_iterator_end_type _it_end;
    };

    template <typename in_iterator_type, typename in_iterator_end_type>
    class _range_from_iterator_pair
        : public _const_range_from_iterator_pair<in_iterator_type, in_iterator_end_type>
    {
        using base_type = _const_range_from_iterator_pair<in_iterator_type, in_iterator_end_type>;

    public:
        using iterator_type = in_iterator_type;
        using iterator_end_type = in_iterator_end_type;

    public:
        constexpr _range_from_iterator_pair(iterator_type it, iterator_end_type it_end)
            : base_type(move(it), move(it_end))
        {}

    public:
        constexpr auto get_iterator() -> iterator_type
        {
            return this->get_iterator();
        }

        constexpr auto get_iterator_end() -> iterator_end_type
        {
            return this->get_iterator_end();
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

            return len + 1;
        }

        return std::strlen(str);
    }
}

export namespace atom::ranges
{
    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename value_type>
    constexpr auto from(std::initializer_list<value_type> list)
    {
        return _const_range_from_iterator_pair(
            array_iterator(list.begin()), array_iterator(list.end()));
    }

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename value_type>
    constexpr auto from(const value_type* begin, const value_type* end)
    {
        return _const_range_from_iterator_pair(array_iterator(begin), array_iterator(end));
    }

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename value_type>
    constexpr auto from(value_type* begin, value_type* end)
    {
        return _range_from_iterator_pair(mut_array_iterator(begin), mut_array_iterator(end));
    }

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename value_type>
    constexpr auto from(const value_type* begin, usize count)
    {
        return _const_range_from_iterator_pair(
            array_iterator(begin), array_iterator(begin + count));
    }

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename value_type>
    constexpr auto from(value_type* begin, usize count)
    {
        return _range_from_iterator_pair(
            mut_array_iterator(begin), mut_array_iterator(begin + count));
    }

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename value_type, usize count>
    constexpr auto from(const value_type (&arr)[count])
    {
        return _const_range_from_iterator_pair(
            array_iterator(ptr(arr)), array_iterator(ptr(arr) + count));
    }

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename value_type, usize count>
    constexpr auto from(value_type (&arr)[count])
    {
        return _range_from_iterator_pair(
            mut_array_iterator(mut_ptr(arr)), mut_array_iterator(mut_ptr(arr) + count));
    }

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    constexpr auto from(const char* str)
    {
        return _const_range_from_iterator_pair(
            array_iterator(str), array_iterator(str + _range_find_str_len(str) + 1));
    }

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    constexpr auto from(char* str)
    {
        return _range_from_iterator_pair(
            mut_array_iterator(str), mut_array_iterator(str + _range_find_str_len(str) + 1));
    }

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename iterator_type, typename iterator_end_type>
    constexpr auto from(iterator_type it, iterator_end_type it_end)
        requires(const_iterator_pair_concept<iterator_type, iterator_end_type>)
    {
        if constexpr (iterator_concept<iterator_type>)
        {
            return _range_from_iterator_pair(move(it), move(it_end));
        }
        else
        {
            return _const_range_from_iterator_pair(move(it), move(it_end));
        }
    }

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    constexpr auto from(std::string_view str)
    {
        return from(str.data(), str.size());
    }
}
