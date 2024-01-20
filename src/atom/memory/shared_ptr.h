#pragma once
#include "atom/memory/obj_helper.h"
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

    template <typename tin_val>
    class shared_ptr: public mut_ptr<tin_val>
    {
        static_assert(tti::is_pure<tin_val>, "shared_ptr only supports pure types.");
        static_assert(not tti::is_void<tin_val>, "shared_ptr does not support void.");

    private:
        using this_type = shared_ptr<tin_val>;
        using base_type = mut_ptr<tin_val>;

    public:
        /// ----------------------------------------------------------------------------------------
        /// type of value `this_type` holds.
        /// ----------------------------------------------------------------------------------------
        using value_type = typename base_type::value_type;

    private:
        template <typename type>
        friend class shared_ptr;

        template <typename type, typename tallocator, typename... arg_types>
        friend auto make_shared_with_alloc(tallocator alloc, arg_types&&... args)
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
        template <typename tval1>
        constexpr shared_ptr& operator=(const shared_ptr<tval1>& that)
            requires rsame_or_derived_from<tval1, value_type>
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
        constexpr shared_ptr(std::nullptr_t)
            : this_type()
        {}

        /// ----------------------------------------------------------------------------------------
        /// # null operator
        /// ----------------------------------------------------------------------------------------
        constexpr shared_ptr& operator=(std::nullptr_t)
        {
            _check_and_release();

            _set_ptr(nullptr);
            _state = nullptr;
        }

        /// ----------------------------------------------------------------------------------------
        /// # value constructor
        /// ----------------------------------------------------------------------------------------
        template <typename tdestroyer = shared_ptr_default_destroyer<value_type>,
            typename tallocator = shared_ptr_default_allocator>
        constexpr explicit shared_ptr(mut_ptr<value_type> ptr, tdestroyer destroyer = tdestroyer(),
            tallocator allocator = tallocator())
            : base_type(ptr)
        {
            if (ptr.ne(nullptr))
            {
                _state = _create_state<tdestroyer, tallocator>(mov(destroyer), mov(allocator));
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
        template <typename type, typename tdestroyer = shared_ptr_default_destroyer<value_type>,
            typename tallocator = shared_ptr_default_allocator>
        constexpr auto set(mut_ptr<type> ptr, tdestroyer destroyer = tdestroyer(),
            tallocator allocator = tallocator())
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
        constexpr auto set(std::nullptr_t)
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
            return _state.eq(nullptr) ? 0 : _state.get().get_count();
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

        template <typename tdestroyer, typename tallocator>
        constexpr auto _create_state(tdestroyer destroyer, tallocator allocator)
            -> mut_ptr<_ishared_ptr_state>
        {
            using state = _shared_ptr_state<value_type, tdestroyer, tallocator>;

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
    template <typename type, typename tallocator, typename... arg_types>
    auto make_shared_with_alloc(tallocator allocator, arg_types&&... args) -> shared_ptr<type>

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
    template <typename value_type, typename tdestroyer>
    template <typename tallocator, typename tval1>
    constexpr auto unique_ptr<value_type, tdestroyer>::_to_shared(tallocator allocator)
        -> shared_ptr<tval1>
    {
        return make_shared_with_alloc(mov(allocator), _get_mut_ptr());
    }
}
