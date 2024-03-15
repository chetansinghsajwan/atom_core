#pragma once
#include "atom/core/core.h"
#include "atom/core/memory/unique_ptr.h"
#include "atom/core/memory/obj_helper.h"
#include "atom/core/typeinfo.h"
#include "atom/core/memory/default_mem_allocator.h"

/// ------------------------------------------------------------------------------------------------
/// implementations
/// ------------------------------------------------------------------------------------------------
namespace atom
{
    template <typename value_t>
    class ebo_helper
    {
    public:
        constexpr ebo_helper(value_t val)
            : _val(move(val))
        {}

    public:
        constexpr auto get() const -> const value_t&
        {
            return _val;
        }

        constexpr auto get_mut() -> value_t&
        {
            return _val;
        }

    private:
        value_t _val;
    };

    template <typename value_t>
        requires typeinfo::is_empty<value_t>
    class ebo_helper<value_t>: private value_t
    {
    public:
        constexpr ebo_helper(value_t val)
            : value_t(move(val))
        {}

    public:
        constexpr auto get() const -> const value_t&
        {
            return static_cast<const value_t&>(*this);
        }

        constexpr auto get_mut() -> value_t&
        {
            return static_cast<value_t&>(*this);
        }
    };

    class _shared_ptr_state
    {
    public:
        virtual auto destroy(void* ptr) -> void = 0;

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

    template <typename value_t, typename destroyer_t, typename allocator_t>
    class _default_shared_ptr_state
        : public _shared_ptr_state
        , private ebo_helper<destroyer_t>
        , private ebo_helper<allocator_t>
    {
    private:
        using destroyer_helper_t = ebo_helper<destroyer_t>;
        using allocator_helper_t = ebo_helper<allocator_t>;

    public:
        constexpr _default_shared_ptr_state(destroyer_t destroyer, allocator_t allocator)
            : destroyer_helper_t(move(destroyer))
            , allocator_helper_t(move(allocator))
        {}

    public:
        virtual auto destroy(void* ptr) -> void override final
        {
            destroyer_helper_t::get_mut()(static_cast<value_t*>(ptr));
        }

        virtual auto dealloc_self() -> void override final
        {
            allocator_helper_t::get_mut().dealloc(this);
        }
    };
}

/// ------------------------------------------------------------------------------------------------
/// apis
/// ------------------------------------------------------------------------------------------------
namespace atom
{
    class shared_ptr_default_allocator: public default_mem_allocator
    {};

    class _shared_ptr_private_ctor
    {};

    template <typename value_t>
    class shared_ptr_default_destroyer
    {
        ATOM_STATIC_ASSERTS(
            typeinfo::is_pure<value_t>, "shared_ptr_default_destroyer only supports pure types.");
        ATOM_STATIC_ASSERTS(
            not typeinfo::is_void<value_t>, "shared_ptr_default_destroyer does not support void.");

    public:
        constexpr auto operator()(value_t* val)
        {
            obj_helper().destruct_as<value_t>(val);
            default_mem_allocator().dealloc(val);
        }
    };

    template <typename in_value_t>
    class shared_ptr
    {
        ATOM_STATIC_ASSERTS(typeinfo::is_pure<in_value_t>, "shared_ptr only supports pure types.");
        ATOM_STATIC_ASSERTS(not typeinfo::is_void<in_value_t>, "shared_ptr does not support void.");

    private:
        using this_t = shared_ptr;

    public:
        /// ----------------------------------------------------------------------------------------
        /// value_t of value `this_t` holds.
        /// ----------------------------------------------------------------------------------------
        using value_t = in_value_t;

    private:
        template <typename value_t>
        friend class shared_ptr;

        template <typename value_t, typename allocator_t, typename... arg_ts>
        friend auto make_shared_with_alloc(allocator_t alloc, arg_ts&&... args)
            -> shared_ptr<value_t>;

    public:
        /// ----------------------------------------------------------------------------------------
        /// # default constructor
        /// ----------------------------------------------------------------------------------------
        constexpr shared_ptr()
            : _ptr(nullptr)
            , _state(nullptr)
        {}

        /// ----------------------------------------------------------------------------------------
        /// # copy constructor
        /// ----------------------------------------------------------------------------------------
        constexpr shared_ptr(const shared_ptr& that) = default;

        /// ----------------------------------------------------------------------------------------
        /// # template copy constructor
        /// ----------------------------------------------------------------------------------------
        template <typename that_t>
        constexpr shared_ptr(const shared_ptr<that_t>& that)
            requires is_same_or_derived_from<that_t, value_t>
            : _ptr(that._ptr)
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
        template <typename other_value_t>
        constexpr shared_ptr& operator=(const shared_ptr<other_value_t>& that)
            requires is_same_or_derived_from<other_value_t, value_t>
        {
            _check_and_release();

            _ptr = that._ptr;
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
        template <typename that_t>
        constexpr shared_ptr(shared_ptr<that_t>&& that)
            requires is_same_or_derived_from<that_t, value_t>
            : _ptr(that._ptr)
            , _state(that._state)
        {
            that._ptr = nullptr;
            that._state = nullptr;
        }

        /// ----------------------------------------------------------------------------------------
        /// # template move operator
        /// ----------------------------------------------------------------------------------------
        template <typename that_t>
        constexpr shared_ptr& operator=(shared_ptr<that_t>&& that)
            requires is_same_or_derived_from<that_t, value_t>
        {
            _check_and_release();

            _ptr = that._ptr;
            _state = that._state;

            that._ptr = nullptr;
            that._state = nullptr;
        }

        /// ----------------------------------------------------------------------------------------
        /// # null constructor
        /// ----------------------------------------------------------------------------------------
        constexpr shared_ptr(nullptr_t)
            : this_t()
        {}

        /// ----------------------------------------------------------------------------------------
        /// # null operator
        /// ----------------------------------------------------------------------------------------
        constexpr shared_ptr& operator=(nullptr_t)
        {
            _check_and_release();

            _ptr = nullptr;
            _state = nullptr;
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # value constructor
        /// ----------------------------------------------------------------------------------------
        template <typename destroyer_t = shared_ptr_default_destroyer<value_t>,
            typename allocator_t = shared_ptr_default_allocator>
        constexpr explicit shared_ptr(value_t* ptr, destroyer_t destroyer = destroyer_t(),
            allocator_t allocator = allocator_t())
            : _ptr(ptr)
        {
            if (ptr != nullptr)
            {
                _state =
                    _create_state<destroyer_t, allocator_t>(move(destroyer), move(allocator));
            }
        }

        /// ----------------------------------------------------------------------------------------
        /// # value operator
        /// ----------------------------------------------------------------------------------------
        constexpr shared_ptr& operator=(value_t* ptr)
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
        constexpr shared_ptr(_shared_ptr_private_ctor, _shared_ptr_state* state, value_t* ptr)
            : _ptr(ptr)
            , _state(state)
        {}

    public:
        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename value_t,
            typename destroyer_t = shared_ptr_default_destroyer<value_t>,
            typename allocator_t = shared_ptr_default_allocator>
        constexpr auto set(value_t* ptr, destroyer_t destroyer = destroyer_t(),
            allocator_t allocator = allocator_t())
            requires is_same_or_derived_from<value_t, value_t>
        {
            if (_ptr != nullptr)
            {
                _release();

                if (ptr != nullptr)
                {
                    _ptr = ptr;
                    _create_state(move(destroyer), move(allocator));
                }
                else
                {
                    _ptr = nullptr;
                    _state = nullptr;
                }
            }
            else
            {
                if (ptr != nullptr)
                {
                    _ptr = ptr;
                    _create_state(move(destroyer), move(allocator));
                }
            }
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto set(nullptr_t)
        {
            _check_and_release();

            _ptr = nullptr;
            _state = nullptr;
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto release() -> value_t*
        {
            _check_and_release();

            value_t* ptr = _ptr;
            _ptr = nullptr;
            _state = nullptr;

            return ptr;
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto get_count() const -> usize
        {
            return _state == nullptr ? 0 : _state->get_count();
        }

        /// ----------------------------------------------------------------------------------------
        /// returns the underlying ptr.
        /// ----------------------------------------------------------------------------------------
        constexpr auto to_unwrapped() const -> const value_t*
        {
            return _ptr;
        }

        /// ----------------------------------------------------------------------------------------
        /// returns the underlying ptr.
        /// ----------------------------------------------------------------------------------------
        constexpr auto to_unwrapped() -> value_t*
        {
            return _ptr;
        }

    private:
        constexpr auto _release()
        {
            _state->decrease_count();

            if (_state->get_count() == 0)
            {
                _state->destroy(_ptr);
                _state->dealloc_self();
            }
        }

        constexpr auto _check_and_release()
        {
            if (_ptr != nullptr)
            {
                _release();
            }
        }

        template <typename destroyer_t, typename allocator_t>
        constexpr auto _create_state(destroyer_t destroyer, allocator_t allocator)
            -> _shared_ptr_state*
        {
            using state_t =
                _default_shared_ptr_state<value_t, destroyer_t, allocator_t>;

            state_t* state = static_cast<state_t*>(allocator.alloc(sizeof(state_t)));
            obj_helper().construct_as<state_t>(state, move(destroyer), move(allocator));
            return state;
        }

        constexpr auto _check_and_increase_shared_count()
        {
            if (_state != nullptr)
            {
                _state->increase_count();
            }
        }

    private:
        value_t* _ptr;
        _shared_ptr_state* _state;
    };

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename value_t, typename... arg_ts>
    auto make_shared(arg_ts&&... args) -> shared_ptr<value_t>
    {
        return make_shared_with_alloc<value_t>(
            shared_ptr_default_allocator(), forward<arg_ts>(args)...);
    }

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename value_t, typename allocator_t, typename... arg_ts>
    auto make_shared_with_alloc(allocator_t allocator, arg_ts&&... args)
        -> shared_ptr<value_t>
    {
        using state_t = _default_shared_ptr_state<value_t,
            shared_ptr_default_destroyer<value_t>, shared_ptr_default_allocator>;

        void* mem = allocator.alloc(sizeof(state_t) + sizeof(value_t));
        state_t* state = mem;
        value_t* value_ptr = static_cast<value_t*>(state + 1);

        obj_helper().construct(value_ptr, forward<arg_ts>(args)...);
        return shared_ptr<value_t>(_shared_ptr_private_ctor(), state, value_ptr);
    }
}

namespace atom
{
    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename value_t, typename destroyer_t>
    template <typename allocator_t, typename other_value_t>
    constexpr auto unique_ptr<value_t, destroyer_t>::_to_shared(allocator_t allocator)
        -> shared_ptr<other_value_t>
    {
        return make_shared_with_alloc(move(allocator), _ptr);
    }
}
