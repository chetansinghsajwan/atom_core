export module atom.core:ranges.range_literatoral;

import :core;

namespace atom
{
    export template <typename value_type>
    class range_literatoral
    {
    public:
        template <usize count>
        constexpr range_literatoral(const value_type (&arr)[count])
            : _arr(arr)
            , _count(count)
        {}

        constexpr range_literatoral(initializer_list<value_type> list)
            : _arr(list.begin())
            , _count(list.size())
        {}

    public:
        constexpr auto get_data() const -> const value_type*
        {
            return _arr;
        }

        constexpr auto get_count() const -> usize
        {
            return _count;
        }

    private:
        const value_type* _arr;
        usize _count;
    };
}
