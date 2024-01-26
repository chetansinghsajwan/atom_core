export module atom.core:_shared_ptr_state;
import :core;
import :tti;
import :ptr;

namespace atom
{
    template <typename type>
    class ebo_helper
    {
    public:
        constexpr ebo_helper(type val)
            : _val(move(val))
        {}

    public:
        constexpr auto get() const -> const type&
        {
            return _val;
        }

        constexpr auto get_mut() -> type&
        {
            return _val;
        }

    private:
        type _val;
    };

    template <typename type>
        requires tti::is_empty<type>
    class ebo_helper<type>: private type
    {
    public:
        constexpr ebo_helper(type val)
            : type(move(val))
        {}

    public:
        constexpr auto get() const -> const type&
        {
            return static_cast<const type&>(*this);
        }

        constexpr auto get_mut() -> type&
        {
            return static_cast<type&>(*this);
        }
    };

    class _ishared_ptr_state
    {
    public:
        virtual auto destroy(mut_ptr<void> ptr) -> void = 0;

        virtual auto dealloc_self() -> void = 0;

        constexpr auto increase_count()
        {
            _count++;
        }

        constexpr auto decrease_count()
        {
            _count--;
        }

        constexpr auto get_count() const -> usize
        {
            return _count;
        }

    private:
        usize _count;
    };

    template <typename value_type, typename destroyer_type, typename allocator_type>
    class _shared_ptr_state
        : public _ishared_ptr_state
        , private ebo_helper<destroyer_type>
        , private ebo_helper<allocator_type>
    {
    private:
        using destroyer_helper_type = ebo_helper<destroyer_type>;
        using allocator_helper_type = ebo_helper<allocator_type>;

    public:
        constexpr _shared_ptr_state(destroyer_type destroyer, allocator_type allocator)
            : destroyer_helper_type(move(destroyer))
            , allocator_helper_type(move(allocator))
        {}

    public:
        virtual auto destroy(mut_ptr<void> ptr) -> void override final
        {
            destroyer_helper_type::get_mut()(ptr.template as<value_type>());
        }

        virtual auto dealloc_self() -> void override final
        {
            allocator_helper_type::get_mut().dealloc(this);
        }
    };
}
