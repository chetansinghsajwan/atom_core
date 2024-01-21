#pragma once
// #include "atom/memory/obj_helper.h"
// #include "atom/memory/ptr.h"
#include "atom/memory/unique_ptr.h"
#include "atom/memory/_shared_ptr_state.h"

// #include "atom/tti.h"

namespace atom
{
    class shared_ptr_default_allocator: public default_mem_allocator
    {};

    class _shared_ptr_private_ctor
    {};

    template <typename type>
    class shared_ptr_default_destroyer
    {
        static_assert(tti::is_pure<type>, "shared_ptr_default_destroyer only supports pure types.");
        static_assert(
            not tti::is_void<type>, "shared_ptr_default_destroyer does not support void.");

    public:
        constexpr auto operator()(mut_ptr<type> val)
        {
            obj_helper().destruct_as<type>(val);
            default_mem_allocator().dealloc(val);
        }
    };

    template <typename in_value_type>
    class shared_ptr: public mut_ptr<in_value_type>
    {
        static_assert(tti::is_pure<in_value_type>, "shared_ptr only supports pure types.");
        static_assert(not tti::is_void<in_value_type>, "shared_ptr does not support void.");

    private:
        using this_type = shared_ptr<in_value_type>;
        using base_type = mut_ptr<in_value_type>;

    public:
        /// ----------------------------------------------------------------------------------------
        /// type of value `this_type` holds.
        /// ----------------------------------------------------------------------------------------
        using value_type = typename base_type::value_type;

    private:
        template <typename type>
        friend class shared_ptr;

        template <typename type, typename allocator_type, typename... arg_types>
        friend auto make_shared_with_alloc(allocator_type alloc, arg_types&&... args)
            -> shared_ptr<type>;

    public:
        /// ----------------------------------------------------------------------------------------
        /// # default constructor
        /// ----------------------------------------------------------------------------------------
        constexpr shared_ptr()
            : base_type()
            , _state(nullptr)
        {}

        /// ----------------------------------------------------------------------------------------
        /// # copy constructor
        /// ----------------------------------------------------------------------------------------
        constexpr shared_ptr(const shared_ptr& that) = default;

        /// ----------------------------------------------------------------------------------------
        /// # template copy constructor
        /// ----------------------------------------------------------------------------------------
        template <typename tthat>
        constexpr shared_ptr(const shared_ptr<tthat>& that)
            requires rsame_or_derived_from<tthat, value_type>
            : base_type(that.unwrap())
            , _state(that._state)
        {
            _check_and_increase_shared_count();
        }

        /// ----------------------------------------------------------------------------------------
        /// # copy operator
        /// ----------------------------------------------------------------------------------------
        constexpr shared_ptr& operator=(const shared_ptr& that) = default;

        /// ----------------------------------------------------------------------------------------
        /// # template copy operator
        /// ----------------------------------------------------------------------------------------
        template <typename other_value_type>
        constexpr shared_ptr& operator=(const shared_ptr<other_value_type>& that)
            requires rsame_or_derived_from<other_value_type, value_type>
        {
            _check_and_release();

            _set_ptr(that._get_mut_ptr());
            _state = that._state;

            _check_and_increase_shared_count();

            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # move constructor
        /// ----------------------------------------------------------------------------------------
        constexpr shared_ptr(shared_ptr&& that) = default;

        /// ----------------------------------------------------------------------------------------
        /// # move operator
        /// ----------------------------------------------------------------------------------------
        constexpr shared_ptr& operator=(shared_ptr&& that) = default;

        /// ----------------------------------------------------------------------------------------
        /// # template move constructor
        /// ----------------------------------------------------------------------------------------
        template <typename tthat>
        constexpr shared_ptr(shared_ptr<tthat>&& that)
            requires rsame_or_derived_from<tthat, value_type>
            : base_type(that.unwrap())
            , _state(that._state)
        {
            that._set_ptr(nullptr);
            that._state = nullptr;
        }

        /// ----------------------------------------------------------------------------------------
        /// # template move operator
        /// ----------------------------------------------------------------------------------------
        template <typename tthat>
        constexpr shared_ptr& operator=(shared_ptr<tthat>&& that)
            requires rsame_or_derived_from<tthat, value_type>
        {
            _check_and_release();

            _set_ptr(that._get_mut_ptr());
            _state = that._state;

            that._set_ptr(nullptr);
            that._state = nullptr;
        }

        /// ----------------------------------------------------------------------------------------
        /// # null constructor
        /// ----------------------------------------------------------------------------------------
        constexpr shared_ptr(nullptr_type)
            : this_type()
        {}

        /// ----------------------------------------------------------------------------------------
        /// # null operator
        /// ----------------------------------------------------------------------------------------
        constexpr shared_ptr& operator=(nullptr_type)
        {
            _check_and_release();

            _set_ptr(nullptr);
            _state = nullptr;
        }

        /// ----------------------------------------------------------------------------------------
        /// # value constructor
        /// ----------------------------------------------------------------------------------------
        template <typename destroyer_type = shared_ptr_default_destroyer<value_type>,
            typename allocator_type = shared_ptr_default_allocator>
        constexpr explicit shared_ptr(mut_ptr<value_type> ptr, destroyer_type destroyer = destroyer_type(),
            allocator_type allocator = allocator_type())
            : base_type(ptr)
        {
            if (not ptr.is_eq(nullptr))
            {
                _state = _create_state<destroyer_type, allocator_type>(mov(destroyer), mov(allocator));
            }
        }

        /// ----------------------------------------------------------------------------------------
        /// # value operator
        /// ----------------------------------------------------------------------------------------
        constexpr shared_ptr& operator=(mut_ptr<value_type> ptr)
        {
            set(ptr);
        }

        /// ----------------------------------------------------------------------------------------
        /// # destructor
        /// ----------------------------------------------------------------------------------------
        constexpr ~shared_ptr()
        {
            _check_and_release();
        }

    private:
        constexpr shared_ptr(
            _shared_ptr_private_ctor, mut_ptr<_ishared_ptr_state> state, mut_ptr<value_type> ptr)
            : base_type(ptr)
            , _state(state)
        {}

    public:
        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename type, typename destroyer_type = shared_ptr_default_destroyer<value_type>,
            typename allocator_type = shared_ptr_default_allocator>
        constexpr auto set(mut_ptr<type> ptr, destroyer_type destroyer = destroyer_type(),
            allocator_type allocator = allocator_type())
            requires rsame_or_derived_from<type, value_type>
        {
            if (_get_ptr() != nullptr)
            {
                _release();

                if (ptr != nullptr)
                {
                    _set_ptr(ptr);
                    _create_state(mov(destroyer), mov(allocator));
                }
                else
                {
                    _set_ptr(nullptr);
                    _state = nullptr;
                }
            }
            else
            {
                if (ptr != nullptr)
                {
                    _set_ptr(ptr);
                    _create_state(mov(destroyer), mov(allocator));
                }
            }
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto set(nullptr_type)
        {
            _check_and_release();

            _set_ptr(nullptr);
            _state = nullptr;
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto release() -> mut_ptr<value_type>
        {
            _check_and_release();

            mut_ptr<value_type> ptr = _get_mut_ptr();
            _set_ptr(nullptr);
            _state = nullptr;

            return ptr;
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto get_count() const -> usize
        {
            return _state.is_eq(nullptr) ? 0 : _state.get().get_count();
        }

    private:
        constexpr auto _release()
        {
            _state.get_mut().decrease_count();

            if (_state.get_mut().get_count() == 0)
            {
                _state.get_mut().destroy(_get_mut_ptr());
                _state.get_mut().dealloc_self();
            }
        }

        constexpr auto _check_and_release()
        {
            if (_get_ptr() != nullptr)
            {
                _release();
            }
        }

        template <typename destroyer_type, typename allocator_type>
        constexpr auto _create_state(destroyer_type destroyer, allocator_type allocator)
            -> mut_ptr<_ishared_ptr_state>
        {
            using state = _shared_ptr_state<value_type, destroyer_type, allocator_type>;

            mut_ptr<void> mem = allocator.alloc(sizeof(state));
            obj_helper().construct_as<state>(mem, mov(destroyer), mov(allocator));
            return mem.as<state>();
        }

        constexpr auto _check_and_increase_shared_count()
        {
            if (_state != nullptr)
            {
                _state.get_mut().increase_count();
            }
        }

        constexpr auto _set_ptr(mut_ptr<value_type> ptr)
        {
            base_type::set(ptr);
        }

        constexpr auto _get_ptr() const -> const value_type*
        {
            return base_type::unwrap();
        }

        constexpr auto _get_mut_ptr() -> value_type*
        {
            return base_type::unwrap();
        }

    private:
        mut_ptr<_ishared_ptr_state> _state;
    };

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename type, typename... arg_types>
    auto make_shared(arg_types&&... args) -> shared_ptr<type>
    {
        return make_shared_with_alloc<type>(
            shared_ptr_default_allocator(), forward<arg_types>(args)...);
    }

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename type, typename allocator_type, typename... arg_types>
    auto make_shared_with_alloc(allocator_type allocator, arg_types&&... args) -> shared_ptr<type>

    {
        using state = _shared_ptr_state<type, shared_ptr_default_destroyer<type>,
            shared_ptr_default_allocator>;

        mut_mem_ptr<void> mem = allocator.alloc(sizeof(state) + sizeof(type));
        mut_ptr<state> state_ptr = mem;
        mut_ptr<type> value_ptr = mem.next(sizeof(state)).as<type>();

        obj_helper().construct(value_ptr, forward<arg_types>(args)...);
        return shared_ptr<type>(_shared_ptr_private_ctor(), state_ptr, value_ptr);
    }
}

namespace atom
{
    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename value_type, typename destroyer_type>
    template <typename allocator_type, typename other_value_type>
    constexpr auto unique_ptr<value_type, destroyer_type>::_to_shared(allocator_type allocator)
        -> shared_ptr<other_value_type>
    {
        return make_shared_with_alloc(mov(allocator), _get_mut_ptr());
    }
}
