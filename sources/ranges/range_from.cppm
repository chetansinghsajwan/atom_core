export module atom.core:ranges.range_from;

import std;
import :core;
import :ranges.array_iterator;
import :ranges.range_literatoral;
import :ranges.iterator_requirements;
import :ranges.range_requirements;
import :ranges.range_extensions;

namespace atom
{
    template <typename in_iterator_t, typename in_iterator_end_t>
    class _range_from_iterator_pair: public range_extensions
    {
    public:
        using value_t = typename in_iterator_t::value_t;
        using iterator_t = in_iterator_t;
        using iterator_end_t = in_iterator_end_t;

    public:
        constexpr _range_from_iterator_pair(iterator_t it, iterator_end_t it_end)
            : _it(move(it))
            , _it_end(move(it_end))
        {}

    public:
        constexpr auto get_iterator() const -> iterator_t
        {
            return _it;
        }

        constexpr auto get_iterator_end() const -> iterator_t
        {
            return _it_end;
        }

        constexpr auto get_data() const -> const value_t*
            requires is_array_iterator_pair<iterator_t, iterator_end_t>
        {
            return &*_it;
        }

        constexpr auto get_count() const -> usize
            requires is_random_access_iterator_pair<iterator_t, iterator_end_t>
        {
            return _it_end - _it;
        }

    private:
        iterator_t _it;
        iterator_end_t _it_end;
    };

    template <typename in_mut_iterator_t, typename in_mut_iterator_end_t>
    class _mut_range_from_iterator_pair
        : public _range_from_iterator_pair<in_mut_iterator_t, in_mut_iterator_end_t>
    {
        using base_t = _range_from_iterator_pair<in_mut_iterator_t, in_mut_iterator_end_t>;

    public:
        using mut_iterator_t = in_mut_iterator_t;
        using mut_iterator_end_t = in_mut_iterator_end_t;

    public:
        constexpr _mut_range_from_iterator_pair(mut_iterator_t it, mut_iterator_end_t it_end)
            : base_t(move(it), move(it_end))
        {}

    public:
        constexpr auto get_mut_iterator() -> mut_iterator_t
        {
            return this->get_iterator();
        }

        constexpr auto get_mut_iterator_end() -> mut_iterator_end_t
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

            return len;
        }

        return std::strlen(str);
    }
}

export namespace atom
{
    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename value_t>
    constexpr auto range_from(std::initializer_list<value_t> list)
    {
        return _range_from_iterator_pair(array_iterator(list.begin()), array_iterator(list.end()));
    }

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename value_t>
    constexpr auto range_from(const value_t* begin, const value_t* end)
    {
        return _range_from_iterator_pair(array_iterator(begin), array_iterator(end));
    }

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename value_t>
    constexpr auto range_from(value_t* begin, value_t* end)
    {
        return _mut_range_from_iterator_pair(mut_array_iterator(begin), mut_array_iterator(end));
    }

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename value_t>
    constexpr auto range_from(const value_t* begin, usize count)
    {
        return _range_from_iterator_pair(array_iterator(begin), array_iterator(begin + count));
    }

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename value_t>
    constexpr auto range_from(value_t* begin, usize count)
    {
        return _mut_range_from_iterator_pair(mut_array_iterator(begin), mut_array_iterator(begin + count));
    }

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename value_t, usize count>
    constexpr auto range_from(const value_t (&arr)[count])
    {
        return _range_from_iterator_pair(array_iterator(ptr(arr)), array_iterator(ptr(arr) + count));
    }

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename value_t, usize count>
    constexpr auto range_from(value_t (&arr)[count])
    {
        return _mut_range_from_iterator_pair(
            mut_array_iterator(mut_ptr(arr)), mut_array_iterator(mut_ptr(arr) + count));
    }

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    constexpr auto range_from(const char* str)
    {
        return _range_from_iterator_pair(array_iterator(str), array_iterator(str + _range_find_str_len(str)));
    }

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    constexpr auto range_from(char* str)
    {
        return _mut_range_from_iterator_pair(
            mut_array_iterator(str), mut_array_iterator(str + _range_find_str_len(str)));
    }

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename iterator_t, typename iterator_end_t>
    constexpr auto range_from(iterator_t it, iterator_end_t it_end)
        requires is_iterator_pair<iterator_t, iterator_end_t>
    {
        if constexpr (is_mut_iterator<iterator_t>)
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
    template <typename value_t>
    constexpr auto range_from_literatoral(range_literatoral<value_t> lit)
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
