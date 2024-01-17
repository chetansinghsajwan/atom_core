#pragma once
#include "atom/core.h"

namespace atom
{
    template <typename type>
    class ebo_helper
    {
    public:
        constexpr ebo_helper(type val)
            : _val(mov(val))
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
            : type(mov(val))
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

    template <typename value_type, typename tdestroyer, typename tallocator>
    class _shared_ptr_state
        : public _ishared_ptr_state
        , private ebo_helper<tdestroyer>
        , private ebo_helper<tallocator>
    {
    private:
        using tdestroyer_helper = ebo_helper<tdestroyer>;
        using tallocator_helper = ebo_helper<tallocator>;

    public:
        constexpr _shared_ptr_state(tdestroyer destroyer, tallocator allocator)
            : tdestroyer_helper(mov(destroyer))
            , tallocator_helper(mov(allocator))
        {}

    public:
        virtual auto destroy(mut_ptr<void> ptr) -> void override final
        {
            tdestroyer_helper::get_mut()(ptr.template as<value_type>());
        }

        virtual auto dealloc_self() -> void override final
        {
            tallocator_helper::get_mut().dealloc(this);
        }
    };
}
