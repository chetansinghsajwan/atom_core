export module atom.core:range.make_range;
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
import :mem_ptr;

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
        constexpr auto iter() const -> iter_type
        {
            return _it;
        }

        constexpr auto iter_end() const -> iter_type
        {
            return _it_end;
        }

        constexpr auto data() const -> mem_ptr<elem_type>
            requires rarray_iter_pair<iter_type, iter_end_type>
        {
            return &_it.value();
        }

        constexpr auto count() const -> usize
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
        constexpr auto mut_iter() -> mut_iter_type
        {
            return this->iter();
        }

        constexpr auto mut_iter_end() -> mut_iter_end_type
        {
            return this->iter_end();
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
            using type = tti::remove_cvref_type<t_>;
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
        using type = typename decltype(_get())::type;
    };

    template <typename iter_type, typename iter_end_type>
    class _mut_range_from_iter_extended
    {
    private:
        template <typename t_>
        class _type_container
        {
        public:
            using type = tti::remove_cvref_type<t_>;
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
        using type = typename decltype(_get())::type;
    };

    template <typename iter_type, typename iter_end_type>
    class _range_from_iter_pair: public _range_from_iter_extended<iter_type, iter_end_type>::type
    {
        using base_type = _range_from_iter_extended<iter_type, iter_end_type>::type;

    public:
        constexpr _range_from_iter_pair(iter_type it, iter_end_type it_end)
            : base_type(move(it), move(it_end))
        {}
    };

    template <typename mut_iter_type, typename mut_iter_end_type>
    class _mut_range_from_iter_pair
        : public _mut_range_from_iter_extended<mut_iter_type, mut_iter_end_type>::type
    {
        using base_type = _mut_range_from_iter_extended<mut_iter_type, mut_iter_end_type>::type;

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
    constexpr auto _range_find_str_len(mem_ptr<uchar> str) -> usize
    {
        if (std::is_constant_evaluated())
        {
            usize len = 0;
            while (str.get() != '\0')
            {
                str++;
                len++;
            }

            return len;
        }

        return usize(std::strlen(_to_std_char_ptr(str.to_unwrapped())));
    }
}

export namespace atom
{
    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename type>
    constexpr auto make_range(std::initializer_list<type> list)
    {
        return _range_from_iter_pair(
            array_iter(mem_ptr<type>(list.begin())), array_iter(mem_ptr<type>(list.end())));
    }

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename type>
    constexpr auto make_range(mem_ptr<type> begin, mem_ptr<type> end)
    {
        return _range_from_iter_pair(array_iter(begin), array_iter(end));
    }

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename type>
    constexpr auto make_range(mut_mem_ptr<type> begin, mut_mem_ptr<type> end)
    {
        return _mut_range_from_iter_pair(mut_array_iter(begin), mut_array_iter(end));
    }

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename type>
    constexpr auto make_range(mem_ptr<type> begin, usize count)
    {
        return _range_from_iter_pair(array_iter(begin), array_iter(begin + count));
    }

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename type>
    constexpr auto make_range(mut_mem_ptr<type> begin, usize count)
    {
        return _mut_range_from_iter_pair(mut_array_iter(begin), mut_array_iter(begin + count));
    }

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename type, usize count>
    constexpr auto make_range(const type (&arr)[count])
    {
        return _range_from_iter_pair(array_iter(mem_ptr(arr)), array_iter(mem_ptr(arr) + count));
    }

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename type, usize count>
    constexpr auto make_range(type (&arr)[count])
    {
        return _mut_range_from_iter_pair(
            mut_array_iter(mut_mem_ptr(arr)), mut_array_iter(mut_mem_ptr(arr) + count));
    }

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    constexpr auto make_range(mem_ptr<uchar> str)
    {
        return _range_from_iter_pair(array_iter(str), array_iter(str + _range_find_str_len(str)));
    }

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    constexpr auto make_range(mut_mem_ptr<uchar> str)
    {
        return _mut_range_from_iter_pair(
            mut_array_iter(str), mut_array_iter(str + _range_find_str_len(str)));
    }

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename iter_type, typename iter_end_type>
    constexpr auto make_range(iter_type it, iter_end_type it_end)
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
        return make_range(lit.data(), lit.count());
    }

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename type>
    constexpr auto make_range(const type* begin, const type* end)
    {
        return make_range(mem_ptr(begin), mem_ptr(end));
    }

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename type>
    constexpr auto make_range(type* begin, type* end)
    {
        return make_range(mut_mem_ptr(begin), mut_mem_ptr(end));
    }

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename type>
    constexpr auto make_range(const type* begin, usize count)
    {
        return make_range(mem_ptr(begin), mem_ptr(begin + count.to_unwrapped()));
    }

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    constexpr auto make_range(const char* begin, const char* end)
    {
        const uchar* begin_ = static_cast<const uchar*>(static_cast<const void*>(begin));
        const uchar* end_ = static_cast<const uchar*>(static_cast<const void*>(end));
        return make_range(begin_, end_);
    }

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    constexpr auto make_range(const char* begin, _usize count)
    {
        return make_range(begin, begin + count);
    }

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <usize::unwrapped_type count>
    constexpr auto make_range(const char (&arr)[count])
    {
        return make_range(arr, arr + count);
    }
}
