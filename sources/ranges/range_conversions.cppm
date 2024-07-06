export module atom_core:ranges.range_conversions;

import std;
import :core;
import :ranges.array_iterator;
import :ranges.iterator_requirements;
import :ranges.range_requirements;

namespace atom::ranges
{
    template <typename in_iterator_type, typename in_iterator_end_type>
    class _range_from_iterator_pair
    {
    public:
        using value_type = typename in_iterator_type::value_type;
        using iterator_type = in_iterator_type;
        using iterator_end_type = in_iterator_end_type;

    public:
        constexpr _range_from_iterator_pair(iterator_type it, iterator_end_type it_end)
            : _it{ move(it) }
            , _it_end{ move(it_end) }
        {}

    public:
        constexpr auto get_iterator() const -> iterator_type
        {
            return _it;
        }

        constexpr auto get_iterator_end() const -> iterator_type
        {
            return _it_end;
        }

        constexpr auto get_data() const -> const value_type*
            requires is_array_iterator_pair<iterator_type, iterator_end_type>
        {
            return &*_it;
        }

        constexpr auto get_count() const -> usize
            requires is_random_access_iterator_pair<iterator_type, iterator_end_type>
        {
            return _it_end - _it;
        }

    private:
        iterator_type _it;
        iterator_end_type _it_end;
    };

    template <typename in_mut_iterator_type, typename in_mut_iterator_end_type>
    class _mut_range_from_iterator_pair
        : public _range_from_iterator_pair<in_mut_iterator_type, in_mut_iterator_end_type>
    {
        using base_type = _range_from_iterator_pair<in_mut_iterator_type, in_mut_iterator_end_type>;

    public:
        using mut_iterator_type = in_mut_iterator_type;
        using mut_iterator_end_type = in_mut_iterator_end_type;

    public:
        constexpr _mut_range_from_iterator_pair(mut_iterator_type it, mut_iterator_end_type it_end)
            : base_type(move(it), move(it_end))
        {}

    public:
        constexpr auto get_iterator() -> mut_iterator_type
        {
            return this->get_iterator();
        }

        constexpr auto get_iterator_end() -> mut_iterator_end_type
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
        return _range_from_iterator_pair(array_iterator(list.begin()), array_iterator(list.end()));
    }

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename value_type>
    constexpr auto from(const value_type* begin, const value_type* end)
    {
        return _range_from_iterator_pair(array_iterator(begin), array_iterator(end));
    }

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename value_type>
    constexpr auto from(value_type* begin, value_type* end)
    {
        return _mut_range_from_iterator_pair(mut_array_iterator(begin), mut_array_iterator(end));
    }

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename value_type>
    constexpr auto from(const value_type* begin, usize count)
    {
        return _range_from_iterator_pair(array_iterator(begin), array_iterator(begin + count));
    }

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename value_type>
    constexpr auto from(value_type* begin, usize count)
    {
        return _mut_range_from_iterator_pair(
            mut_array_iterator(begin), mut_array_iterator(begin + count));
    }

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename value_type, usize count>
    constexpr auto from(const value_type (&arr)[count])
    {
        return _range_from_iterator_pair(
            array_iterator(ptr(arr)), array_iterator(ptr(arr) + count));
    }

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename value_type, usize count>
    constexpr auto from(value_type (&arr)[count])
    {
        return _mut_range_from_iterator_pair(
            mut_array_iterator(mut_ptr(arr)), mut_array_iterator(mut_ptr(arr) + count));
    }

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    constexpr auto from(const char* str)
    {
        return _range_from_iterator_pair(
            array_iterator(str), array_iterator(str + _range_find_str_len(str) + 1));
    }

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    constexpr auto from(char* str)
    {
        return _mut_range_from_iterator_pair(
            mut_array_iterator(str), mut_array_iterator(str + _range_find_str_len(str) + 1));
    }

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename iterator_type, typename iterator_end_type>
    constexpr auto from(iterator_type it, iterator_end_type it_end)
        requires is_iterator_pair<iterator_type, iterator_end_type>
    {
        if constexpr (is_mut_iterator<iterator_type>)
        {
            return _mut_range_from_iterator_pair(move(it), move(it_end));
        }
        else
        {
            return _range_from_iterator_pair(move(it), move(it_end));
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
