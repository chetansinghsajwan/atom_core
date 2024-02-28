export module atom.core:range.range_from;
import :range.iter_requirements;
import :range.range_requirements;
import :range.range_extensions;
import :range.fwd_range_extensions;
import :range.bidi_range_extensions;
import :range.jump_range_extensions;
import :range.array_range_extensions;
import :range.mut_range_extensions;
import :range.mut_fwd_range_extensions;
import :range.mut_bidi_range_extensions;
import :range.mut_jump_range_extensions;
import :range.mut_array_range_extensions;
import :range.range_literal;
import :std;
import :core;
import :array_iter;
import :tti;
import :ptr;

namespace atom
{
    template <typename in_iter_type, typename in_iter_end_type>
    class _basic_range_from_iter_pair
    {
    public:
        using elem_type = typename in_iter_type::elem_type;
        using iter_type = in_iter_type;
        using iter_end_type = in_iter_end_type;

    public:
        constexpr _basic_range_from_iter_pair(iter_type it, iter_end_type it_end)
            : _it(move(it))
            , _it_end(move(it_end))
        {}

    public:
        constexpr auto get_iter() const -> iter_type
        {
            return _it;
        }

        constexpr auto get_iter_end() const -> iter_type
        {
            return _it_end;
        }

        constexpr auto get_data() const -> const elem_type*
            requires rarray_iter_pair<iter_type, iter_end_type>
        {
            return &_it.value();
        }

        constexpr auto get_count() const -> usize
            requires rjump_iter_pair<iter_type, iter_end_type>
        {
            return _it_end.compare(_it).template to<usize>();
        }

    private:
        iter_type _it;
        iter_end_type _it_end;
    };

    template <typename tmut_iter_, typename tmut_iter_end_>
    class _basic_mut_range_from_iter_pair
        : public _basic_range_from_iter_pair<tmut_iter_, tmut_iter_end_>
    {
        using base_type = _basic_range_from_iter_pair<tmut_iter_, tmut_iter_end_>;

    public:
        using mut_iter_type = tmut_iter_;
        using mut_iter_end_type = tmut_iter_end_;

    public:
        constexpr _basic_mut_range_from_iter_pair(mut_iter_type it, mut_iter_end_type it_end)
            : base_type(move(it), move(it_end))
        {}

    public:
        constexpr auto get_mut_iter() -> mut_iter_type
        {
            return this->get_iter();
        }

        constexpr auto get_mut_iter_end() -> mut_iter_end_type
        {
            return this->get_iter_end();
        }
    };

    template <typename iter_type, typename iter_end_type>
    class _range_from_iter_extended
    {
    private:
        template <typename t_>
        class _type_container
        {
        public:
            using elem_type = tti::remove_cvref_type<t_>;
        };

        static consteval auto _get()
        {
            using range_type = _basic_range_from_iter_pair<iter_type, iter_end_type>;

            if constexpr (rarray_iter_pair<iter_type, iter_end_type>)
                return _type_container<array_range_extensions<range_type>>();

            else if constexpr (rjump_iter_pair<iter_type, iter_end_type>)
                return _type_container<jump_range_extensions<range_type>>();

            else if constexpr (rbidi_iter_pair<iter_type, iter_end_type>)
                return _type_container<bidi_range_extensions<range_type>>();

            else if constexpr (rfwd_iter_pair<iter_type, iter_end_type>)
                return _type_container<fwd_range_extensions<range_type>>();

            else if constexpr (riter_pair<iter_type, iter_end_type>)
                return _type_container<range_extensions<range_type>>();
        }

    public:
        using elem_type = typename decltype(_get())::elem_type;
    };

    template <typename iter_type, typename iter_end_type>
    class _mut_range_from_iter_extended
    {
    private:
        template <typename t_>
        class _type_container
        {
        public:
            using elem_type = tti::remove_cvref_type<t_>;
        };

        static consteval auto _get()
        {
            using range_type = _basic_mut_range_from_iter_pair<iter_type, iter_end_type>;

            if constexpr (rarray_iter_pair<iter_type, iter_end_type>)
                return _type_container<mut_array_range_extensions<range_type>>();

            else if constexpr (rjump_iter_pair<iter_type, iter_end_type>)
                return _type_container<mut_jump_range_extensions<range_type>>();

            else if constexpr (rbidi_iter_pair<iter_type, iter_end_type>)
                return _type_container<mut_bidi_range_extensions<range_type>>();

            else if constexpr (rfwd_iter_pair<iter_type, iter_end_type>)
                return _type_container<mut_fwd_range_extensions<range_type>>();

            else if constexpr (riter_pair<iter_type, iter_end_type>)
                return _type_container<mut_range_extensions<range_type>>();
        }

    public:
        using elem_type = typename decltype(_get())::elem_type;
    };

    template <typename iter_type, typename iter_end_type>
    class _range_from_iter_pair
        : public _range_from_iter_extended<iter_type, iter_end_type>::elem_type
    {
        using base_type = _range_from_iter_extended<iter_type, iter_end_type>::elem_type;

    public:
        constexpr _range_from_iter_pair(iter_type it, iter_end_type it_end)
            : base_type(move(it), move(it_end))
        {}
    };

    template <typename mut_iter_type, typename mut_iter_end_type>
    class _mut_range_from_iter_pair
        : public _mut_range_from_iter_extended<mut_iter_type, mut_iter_end_type>::elem_type
    {
        using base_type =
            _mut_range_from_iter_extended<mut_iter_type, mut_iter_end_type>::elem_type;

    public:
        constexpr _mut_range_from_iter_pair(mut_iter_type it, mut_iter_end_type it_end)
            : base_type(move(it), move(it_end))
        {}
    };

    /// --------------------------------------------------------------------------------------------
    /// # to do
    ///
    /// - review this implementation after implementing character encoding.
    /// --------------------------------------------------------------------------------------------
    constexpr auto _range_find_str_len(const uchar* str) -> usize
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

        return usize(std::strlen(_to_std_char_ptr(str)));
    }
}

export namespace atom
{
    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename elem_type>
    constexpr auto range_from(std::initializer_list<elem_type> list)
    {
        return _range_from_iter_pair(array_iter(list.begin()), array_iter(list.end()));
    }

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename elem_type>
    constexpr auto range_from(const elem_type* begin, const elem_type* end)
    {
        return _range_from_iter_pair(array_iter(begin), array_iter(end));
    }

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename elem_type>
    constexpr auto range_from(elem_type* begin, elem_type* end)
    {
        return _mut_range_from_iter_pair(mut_array_iter(begin), mut_array_iter(end));
    }

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename elem_type>
    constexpr auto range_from(const elem_type* begin, usize count)
    {
        return _range_from_iter_pair(array_iter(begin), array_iter(begin + count));
    }

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename elem_type>
    constexpr auto range_from(elem_type* begin, usize count)
    {
        return _mut_range_from_iter_pair(mut_array_iter(begin), mut_array_iter(begin + count));
    }

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename elem_type, usize count>
    constexpr auto range_from(const elem_type (&arr)[count])
    {
        return _range_from_iter_pair(array_iter(ptr(arr)), array_iter(ptr(arr) + count));
    }

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename elem_type, usize count>
    constexpr auto range_from(elem_type (&arr)[count])
    {
        return _mut_range_from_iter_pair(
            mut_array_iter(mut_ptr(arr)), mut_array_iter(mut_ptr(arr) + count));
    }

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    constexpr auto range_from(const uchar* str)
    {
        return _range_from_iter_pair(
            array_iter(str), array_iter(str + _range_find_str_len(str).to_unwrapped()));
    }

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    constexpr auto range_from(uchar* str)
    {
        return _mut_range_from_iter_pair(
            mut_array_iter(str), mut_array_iter(str + _range_find_str_len(str).to_unwrapped()));
    }

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename iter_type, typename iter_end_type>
    constexpr auto range_from(iter_type it, iter_end_type it_end)
        requires riter_pair<iter_type, iter_end_type>
    {
        if constexpr (rmut_iter<iter_type>)
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
    template <typename elem_type>
    constexpr auto range_from_literal(range_literal<elem_type> lit)
    {
        return range_from(lit.get_data(), lit.get_count());
    }

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    constexpr auto range_from(const char* begin, const char* end)
    {
        const uchar* begin_ = static_cast<const uchar*>(static_cast<const void*>(begin));
        const uchar* end_ = static_cast<const uchar*>(static_cast<const void*>(end));
        return range_from(begin_, end_);
    }

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    constexpr auto range_from(const char* begin, _usize count)
    {
        return range_from(begin, begin + count);
    }

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    // template <usize::unwrapped_type count>
    // constexpr auto range_from(const char (&str)[count])
    // {
    //     return range_from(str, count);
    // }
}
