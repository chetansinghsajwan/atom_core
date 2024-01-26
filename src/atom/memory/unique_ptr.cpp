export module atom.core:unique_ptr;
import :core;
import :obj_helper;
import :tti;
import :ptr;
import :memory.default_mem_allocator;

namespace atom
{
    export template <typename type>
    class shared_ptr;

    export template <typename type>
    class unique_ptr_default_destroyer
    {
        static_assert(tti::is_pure<type>, "unique_ptr_default_destroyer only supports pure types.");
        static_assert(
            not tti::is_void<type>, "unique_ptr_default_destroyer does not support void.");

    public:
        constexpr auto operator()(mut_ptr<type> val)
        {
            obj_helper().destruct_as<type>(val);
            default_mem_allocator().dealloc(val);
        }
    };

    export template <typename in_value_type,
        typename in_destroyer_type = unique_ptr_default_destroyer<in_value_type>>
    class unique_ptr: public mut_ptr<in_value_type>
    {
        static_assert(tti::is_pure<in_value_type>, "unique_ptr only supports pure types.");
        static_assert(not tti::is_void<in_value_type>, "unique_ptr does not support void.");
        static_assert(tti::is_pure<in_destroyer_type>);
        static_assert(not tti::is_void<in_destroyer_type>);

    private:
        template <typename other_value_type, typename other_destroyer_type>
        friend class unique_ptr;

    private:
        using base_type = mut_ptr<in_value_type>;

    public:
        /// ----------------------------------------------------------------------------------------
        /// type of value `this_type` holds.
        /// ----------------------------------------------------------------------------------------
        using value_type = in_value_type;

        /// ----------------------------------------------------------------------------------------
        /// type of destroyer used to destroy value and dealloc memory.
        /// ----------------------------------------------------------------------------------------
        using destroyer_type = in_destroyer_type;

    public:
        /// ----------------------------------------------------------------------------------------
        /// # default constructor
        /// ----------------------------------------------------------------------------------------
        constexpr unique_ptr(destroyer_type destroyer = destroyer_type())
            : base_type()
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
            : base_type(that.unwrap())
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
            requires rsame_or_derived_from<other_value_type, value_type>
            : base_type(that.unwrap())
            , _destroyer(move(that._destroyer))
        {
            that._set_ptr(nullptr);
        }

        /// ----------------------------------------------------------------------------------------
        /// # template move operator
        /// ----------------------------------------------------------------------------------------
        template <typename other_value_type>
        constexpr unique_ptr& operator=(unique_ptr<other_value_type, destroyer_type>&& that)
            requires rsame_or_derived_from<other_value_type, value_type>
        {
            _move(move(that));
        }

        /// ----------------------------------------------------------------------------------------
        /// # value constructor
        /// ----------------------------------------------------------------------------------------
        constexpr explicit unique_ptr(
            mut_ptr<value_type> ptr, destroyer_type destroyer = destroyer_type())
            : base_type(ptr)
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
        constexpr auto set(mut_ptr<value_type> ptr)
        {
            _check_and_destroy_value();

            _set_ptr(ptr.unwrap());
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto set(mut_ptr<value_type> ptr, destroyer_type destroyer = destroyer_type())
        {
            _check_and_destroy_value();

            _set_ptr(ptr.unwrap());
            _destroyer = move(destroyer);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto set(nullptr_type)
        {
            _check_and_destroy_value();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto release() -> mut_ptr<value_type>
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
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename type = value_type>
        constexpr auto to_shared() -> shared_ptr<type>
            requires rsame_or_derived_from<type, value_type>
        {
            return _to_shared();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename allocator_type, typename type = value_type>
        constexpr auto to_shared_with_alloc(allocator_type allocator = allocator_type())
            -> shared_ptr<type>
            requires rsame_or_derived_from<type, value_type>
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
            return base_type::unwrap();
        }

        constexpr auto _get_mut_ptr() -> value_type*
        {
            return base_type::unwrap();
        }

        constexpr auto _set_ptr(value_type* ptr)
        {
            return base_type::set(ptr);
        }

        template <typename allocator_type, typename other_value_type>
        constexpr auto _to_shared(allocator_type allocator) -> shared_ptr<other_value_type>;

    private:
        destroyer_type _destroyer;
    };

    export template <typename type, typename... arg_types>
    auto make_unique(arg_types&&... args) -> unique_ptr<type>
    {
        // todo: implement this.
        return unique_ptr<type>(nullptr);
    }

    export template <typename type, typename allocator_type, typename... arg_types>
    auto make_unique_with_alloc(allocator_type allocator, arg_types&&... args) -> unique_ptr<type>
    {
        // todo: implement this.
        return unique_ptr<type>(nullptr);
    }
}
