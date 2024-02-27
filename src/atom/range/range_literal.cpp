export module atom.core:range.range_literal;
import :core;
import :ptr;

namespace atom
{
    export template <typename elem_type>
    class range_literal
    {
    public:
        template <usize count>
        constexpr range_literal(const elem_type (&arr)[count])
            : _arr(arr)
            , _count(count)
        {}

        constexpr range_literal(initializer_list<elem_type> list)
            : _arr(list.begin())
            , _count(list.size())
        {}

    public:
        constexpr auto get_data() const -> const elem_type*
        {
            return _arr;
        }

        constexpr auto get_count() const -> usize
        {
            return _count;
        }

    private:
        const elem_type* _arr;
        usize _count;
    };

    export template <>
    class range_literal<uchar>: public range_literal<char>
    {
        using base_type = range_literal<char>;

    public:
        using base_type::base_type;
        using base_type::operator=;

    public:
        auto get_data() const -> const uchar*
        {
            return reinterpret_cast<const uchar*>(base_type::get_data());
        }
    };
}
