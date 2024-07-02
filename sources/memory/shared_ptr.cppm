export module atom_core:shared_ptr;

import :core;
import :types;
import :unique_ptr;
import :obj_helper;
import :default_mem_allocator;

/// ------------------------------------------------------------------------------------------------
/// implementations
/// ------------------------------------------------------------------------------------------------
namespace atom
{
    template <typename value_type>
    class ebo_helper
    {
    public:
        constexpr ebo_helper(value_type val)
            : _val(move(val))
        {}

    public:
        constexpr auto get() const -> const value_type&
        {
            return _val;
        }

        constexpr auto get_mut() -> value_type&
        {
            return _val;
        }

    private:
        value_type _val;
    };

    template <typename value_type>
        requires(type_info<value_type>::is_empty())
    class ebo_helper<value_type>: private value_type
    {
    public:
        constexpr ebo_helper(value_type val)
            : value_type(move(val))
        {}

    public:
        constexpr auto get() const -> const value_type&
        {
            return static_cast<const value_type&>(*this);
        }

        constexpr auto get_mut() -> value_type&
        {
            return static_cast<value_type&>(*this);
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

    template <typename value_type, typename destroyer_type, typename allocator_type>
    class _default_shared_ptr_state
        : public _shared_ptr_state
        , private ebo_helper<destroyer_type>
        , private ebo_helper<allocator_type>
    {
    private:
        using destroyer_helper_type = ebo_helper<destroyer_type>;
        using allocator_helper_type = ebo_helper<allocator_type>;

    public:
        constexpr _default_shared_ptr_state(destroyer_type destroyer, allocator_type allocator)
            : destroyer_helper_type(move(destroyer))
            , allocator_helper_type(move(allocator))
        {}

    public:
        virtual auto destroy(void* ptr) -> void override final
        {
            destroyer_helper_type::get_mut()(static_cast<value_type*>(ptr));
        }

        virtual auto dealloc_self() -> void override final
        {
            allocator_helper_type::get_mut().dealloc(this);
        }
    };
}

/// ------------------------------------------------------------------------------------------------
/// apis
/// ------------------------------------------------------------------------------------------------
export namespace atom
{
    class shared_ptr_default_allocator: public default_mem_allocator
    {};

    class _shared_ptr_private_ctor
    {};

    template <typename value_type>
    class shared_ptr_default_destroyer
    {
        static_assert(type_info<value_type>::is_pure(),
            "shared_ptr_default_destroyer only supports pure types.");
        static_assert(not type_info<value_type>::is_void(),
            "shared_ptr_default_destroyer does not support void.");

    public:
        constexpr auto operator()(value_type* val)
        {
            obj_helper::destruct_as<value_type>(val);
            default_mem_allocator().dealloc(val);
        }
    };

    template <typename in_value_type>
    class shared_ptr
    {
        static_assert(type_info<in_value_type>::is_pure(), "shared_ptr only supports pure types.");
        static_assert(not type_info<in_value_type>::is_void(), "shared_ptr does not support void.");

    private:
        using this_type = shared_ptr;

    public:
        /// ----------------------------------------------------------------------------------------
        /// value_type of value `this_type` holds.
        /// ----------------------------------------------------------------------------------------
        using value_type = in_value_type;

    private:
        template <typename value_type>
        friend class shared_ptr;

        template <typename value_type, typename allocator_type, typename... arg_types>
        friend auto make_shared_with_alloc(
            allocator_type alloc, arg_types&&... args) -> shared_ptr<value_type>;

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
        template <typename that_type>
        constexpr shared_ptr(const shared_ptr<that_type>& that)
            requires(type_info<that_type>::template is_same_or_derived_from<value_type>())
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
        template <typename other_value_type>
        constexpr shared_ptr& operator=(const shared_ptr<other_value_type>& that)
            requires(type_info<other_value_type>::template is_same_or_derived_from<value_type>())
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
        template <typename that_type>
        constexpr shared_ptr(shared_ptr<that_type>&& that)
            requires(type_info<that_type>::template is_same_or_derived_from<value_type>())
            : _ptr(that._ptr)
            , _state(that._state)
        {
            that._ptr = nullptr;
            that._state = nullptr;
        }

        /// ----------------------------------------------------------------------------------------
        /// # template move operator
        /// ----------------------------------------------------------------------------------------
        template <typename that_type>
        constexpr shared_ptr& operator=(shared_ptr<that_type>&& that)
            requires(type_info<that_type>::template is_same_or_derived_from<value_type>())
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
            : this_type()
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
        template <typename destroyer_type = shared_ptr_default_destroyer<value_type>,
            typename allocator_type = shared_ptr_default_allocator>
        constexpr explicit shared_ptr(value_type* ptr, destroyer_type destroyer = destroyer_type(),
            allocator_type allocator = allocator_type())
            : _ptr(ptr)
        {
            if (ptr != nullptr)
            {
                _state =
                    _create_state<destroyer_type, allocator_type>(move(destroyer), move(allocator));
            }
        }

        /// ----------------------------------------------------------------------------------------
        /// # value operator
        /// ----------------------------------------------------------------------------------------
        constexpr shared_ptr& operator=(value_type* ptr)
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
        constexpr shared_ptr(_shared_ptr_private_ctor, _shared_ptr_state* state, value_type* ptr)
            : _ptr(ptr)
            , _state(state)
        {}

    public:
        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename value_type,
            typename destroyer_type = shared_ptr_default_destroyer<value_type>,
            typename allocator_type = shared_ptr_default_allocator>
        constexpr auto set(value_type* ptr, destroyer_type destroyer = destroyer_type(),
            allocator_type allocator = allocator_type())
            requires(type_info<value_type>::template is_same_or_derived_from<value_type>())
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
        constexpr auto release() -> value_type*
        {
            _check_and_release();

            value_type* ptr = _ptr;
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
        constexpr auto to_unwrapped() const -> const value_type*
        {
            return _ptr;
        }

        /// ----------------------------------------------------------------------------------------
        /// returns the underlying ptr.
        /// ----------------------------------------------------------------------------------------
        constexpr auto to_unwrapped() -> value_type*
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

        template <typename destroyer_type, typename allocator_type>
        constexpr auto _create_state(
            destroyer_type destroyer, allocator_type allocator) -> _shared_ptr_state*
        {
            using state_type =
                _default_shared_ptr_state<value_type, destroyer_type, allocator_type>;

            state_type* state = static_cast<state_type*>(allocator.alloc(sizeof(state_type)));
            obj_helper::construct_as<state_type>(state, move(destroyer), move(allocator));
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
        value_type* _ptr;
        _shared_ptr_state* _state;
    };

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename value_type, typename... arg_types>
    auto make_shared(arg_types&&... args) -> shared_ptr<value_type>
    {
        return make_shared_with_alloc<value_type>(
            shared_ptr_default_allocator(), forward<arg_types>(args)...);
    }

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename value_type, typename allocator_type, typename... arg_types>
    auto make_shared_with_alloc(
        allocator_type allocator, arg_types&&... args) -> shared_ptr<value_type>
    {
        using state_type = _default_shared_ptr_state<value_type,
            shared_ptr_default_destroyer<value_type>, shared_ptr_default_allocator>;

        void* mem = allocator.alloc(sizeof(state_type) + sizeof(value_type));
        state_type* state = mem;
        value_type* value_ptr = static_cast<value_type*>(state + 1);

        obj_helper::construct(value_ptr, forward<arg_types>(args)...);
        return shared_ptr<value_type>(_shared_ptr_private_ctor(), state, value_ptr);
    }
}

namespace atom
{
    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename value_type, typename destroyer_type>
    template <typename allocator_type, typename other_value_type>
    constexpr auto unique_ptr<value_type, destroyer_type>::_to_shared(
        allocator_type allocator) -> shared_ptr<other_value_type>
    {
        return make_shared_with_alloc(move(allocator), _ptr);
    }
}
