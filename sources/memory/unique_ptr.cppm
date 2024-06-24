export module atom.core:unique_ptr;

import std;
import :types;
import :default_mem_allocator;
import :obj_helper;

export namespace atom
{
    template <typename value_type>
    class shared_ptr;

    template <typename value_type>
    class unique_ptr_default_destroyer
    {
        static_assert(type_info<value_type>::is_pure(),
            "unique_ptr_default_destroyer only supports pure types.");
        static_assert(not type_info<value_type>::is_void(),
            "unique_ptr_default_destroyer does not support void.");

    public:
        constexpr auto operator()(value_type* val)
        {
            obj_helper().destruct_as<value_type>(val);
            default_mem_allocator().dealloc(val);
        }
    };

    template <typename in_value_type,
        typename in_destroyer_type = unique_ptr_default_destroyer<in_value_type>>
    class unique_ptr
    {
        static_assert(type_info<in_value_type>::is_pure(), "unique_ptr only supports pure types.");
        static_assert(not type_info<in_value_type>::is_void(), "unique_ptr does not support void.");
        static_assert(type_info<in_destroyer_type>::is_pure);
        static_assert(not type_info<in_destroyer_type>::is_void);

    private:
        template <typename other_value_type, typename other_destroyer_type>
        friend class unique_ptr;

    public:
        /// ----------------------------------------------------------------------------------------
        /// value_type of value `this_type` holds.
        /// ----------------------------------------------------------------------------------------
        using value_type = in_value_type;

        /// ----------------------------------------------------------------------------------------
        /// value_type of destroyer used to destroy value and dealloc memory.
        /// ----------------------------------------------------------------------------------------
        using destroyer_type = in_destroyer_type;

    public:
        /// ----------------------------------------------------------------------------------------
        /// # default constructor
        /// ----------------------------------------------------------------------------------------
        constexpr unique_ptr(destroyer_type destroyer = destroyer_type())
            : _ptr(nullptr)
            , _destroyer(move(destroyer))
        {}

        /// ----------------------------------------------------------------------------------------
        /// # copy constructor
        /// ----------------------------------------------------------------------------------------
        constexpr unique_ptr(const unique_ptr& that) = delete;

        /// ----------------------------------------------------------------------------------------
        /// # copy operator
        /// ----------------------------------------------------------------------------------------
        constexpr unique_ptr& operator=(const unique_ptr& that) = delete;

        /// ----------------------------------------------------------------------------------------
        /// # move constructor
        /// ----------------------------------------------------------------------------------------
        constexpr unique_ptr(unique_ptr&& that)
            : _ptr(that._ptr)
            , _destroyer(move(that._destroyer))
        {
            that._set_ptr(nullptr);
        }

        /// ----------------------------------------------------------------------------------------
        /// # move operator
        /// ----------------------------------------------------------------------------------------
        constexpr unique_ptr& operator=(unique_ptr&& that)
        {
            _move(move(that));
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # template move constructor
        /// ----------------------------------------------------------------------------------------
        template <typename other_value_type>
        constexpr unique_ptr(unique_ptr<other_value_type, destroyer_type>&& that)
            requires(type_info<other_value_type>::template is_same_or_derived_from<value_type>)
            : _ptr(that._ptr)
            , _destroyer(move(that._destroyer))
        {
            that._set_ptr(nullptr);
        }

        /// ----------------------------------------------------------------------------------------
        /// # template move operator
        /// ----------------------------------------------------------------------------------------
        template <typename other_value_type>
        constexpr unique_ptr& operator=(unique_ptr<other_value_type, destroyer_type>&& that)
            requires(type_info<other_value_type>::template is_same_or_derived_from<value_type>)
        {
            _move(move(that));
        }

        /// ----------------------------------------------------------------------------------------
        /// # value constructor
        /// ----------------------------------------------------------------------------------------
        constexpr explicit unique_ptr(value_type* ptr, destroyer_type destroyer = destroyer_type())
            : _ptr(ptr)
            , _destroyer(move(destroyer))
        {}

        /// ----------------------------------------------------------------------------------------
        /// # destructor
        /// ----------------------------------------------------------------------------------------
        constexpr ~unique_ptr()
        {
            _check_and_destroy_value();
        }

    public:
        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto set(value_type* ptr)
        {
            _check_and_destroy_value();
            _set_ptr(ptr);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto set(value_type* ptr, destroyer_type destroyer = destroyer_type())
        {
            _check_and_destroy_value();

            _set_ptr(ptr);
            _destroyer = move(destroyer);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto set(std::nullptr_t)
        {
            _check_and_destroy_value();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto release() -> value_type*
        {
            return _release_value();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto destroy()
        {
            _check_and_destroy_value();
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

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename new_value_type = value_type>
        constexpr auto to_shared() -> shared_ptr<new_value_type>
            requires(type_info<new_value_type>::template is_same_or_derived_from<value_type>)
        {
            return _to_shared();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename allocator_type, typename new_value_type = value_type>
        constexpr auto to_shared_with_alloc(
            allocator_type allocator = allocator_type()) -> shared_ptr<new_value_type>
            requires(type_info<new_value_type>::template is_same_or_derived_from<value_type>)
        {
            return _to_shared();
        }

    private:
        template <typename other_value_type>
        constexpr auto _move(unique_ptr<other_value_type, destroyer_type>&& that)
        {
            _check_and_destroy_value();

            _set_ptr(that._get_mut_ptr());
            _destroyer = move(that._destroyer);
            that._set_ptr(nullptr);
        }

        constexpr auto _check_and_destroy_value()
        {
            if (_get_ptr() != nullptr)
            {
                _destroy_value();
                _set_ptr(nullptr);
            }
        }

        constexpr auto _release_value() -> value_type*
        {
            value_type* ptr = _get_mut_ptr();
            _set_ptr(nullptr);
            return ptr;
        }

        constexpr auto _destroy_value()
        {
            _destroyer(_get_mut_ptr());
        }

        constexpr auto _get_ptr() const -> const value_type*
        {
            return _ptr;
        }

        constexpr auto _get_mut_ptr() -> value_type*
        {
            return _ptr;
        }

        constexpr auto _set_ptr(value_type* ptr)
        {
            return _ptr = ptr;
        }

        template <typename allocator_type, typename other_value_type>
        constexpr auto _to_shared(allocator_type allocator) -> shared_ptr<other_value_type>;

    private:
        destroyer_type _destroyer;
        value_type* _ptr;
    };

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename value_type, typename... arg_types>
    auto make_unique(arg_types&&... args) -> unique_ptr<value_type>
    {
        return make_unique_with_alloc<value_type, default_mem_allocator>(
            default_mem_allocator(), forward<arg_types>(args)...);
    }

    /// --------------------------------------------------------------------------------------------
    /// # todo: fix this implementation, maybe store allocator in destroyer.
    /// --------------------------------------------------------------------------------------------
    template <typename value_type, typename allocator_type, typename... arg_types>
    auto make_unique_with_alloc(
        allocator_type allocator, arg_types&&... args) -> unique_ptr<value_type>
    {
        value_type* mem = (value_type*)allocator.alloc(sizeof(value_type));
        obj_helper().construct_as<value_type>(mem, forward<arg_types>(args)...);
        return unique_ptr<value_type>(mem);
    }
}
