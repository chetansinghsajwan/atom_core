export module atom.core:static_storage;
import :core;
import :mem_ptr;
import :type_list;
import :contracts_decl;

namespace atom
{
    export template <typename... types>
    class static_storage_for
    {
    private:
        using _types = type_list<types...>;

    public:
        alignas(_types::max_align.unwrap()) byte storage[_types::max_size.unwrap()];
    };

    export template <usize in_size>
    class static_storage
    {
    public:
        static consteval auto size() -> usize
        {
            return in_size;
        }

    public:
        constexpr auto mem(usize i = 0) const -> mem_ptr<void>
        {
            contracts::debug_expects(i < size());

            return mem_ptr(_storage + i.unwrap());
        }

        constexpr auto mut_mem(usize i = 0) -> mut_mem_ptr<void>
        {
            contracts::debug_expects(i < size());

            return mut_mem_ptr(_storage + i.unwrap());
        }

        constexpr auto ref(usize i) const -> const byte&
        {
            contracts::debug_expects(i < size());

            return _storage[i.unwrap()];
        }

        constexpr auto mut_ref(usize i) -> byte&
        {
            contracts::debug_expects(i < size());

            return _storage[i.unwrap()];
        }

        constexpr auto operator[](usize i) const -> const byte&
        {
            return ref(i);
        }

        constexpr auto operator[](usize i) -> byte&
        {
            return mut_ref(i);
        }

        constexpr auto is_eq(const static_storage& that) const -> bool
        {
            for (usize i = usize(0); i < size(); i++)
            {
                if (_storage[i.unwrap()] != that._storage[i.unwrap()])
                    return false;
            }

            return true;
        }

    public:
        byte _storage[size().unwrap()];
    };
}
