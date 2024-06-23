export module atom.core:function_box;

import std;
import :core;
import :types;
import :box;
import :contracts;

/// ------------------------------------------------------------------------------------------------
/// implementations
/// ------------------------------------------------------------------------------------------------
namespace atom
{
    class function_box_tag
    {};

    template <typename result_type, typename... arg_types>
    class _function_box_interface
    {
    public:
        virtual auto invoke(arg_types... args) -> result_type = 0;
    };

    template <typename function_type, typename result_type, typename... arg_types>
    class _function_box_wrapper: public _function_box_interface<result_type, arg_types...>
    {
    public:
        _function_box_wrapper(function_type&& function)
            : _function(function)
        {}

    public:
        virtual auto invoke(arg_types... args) -> result_type override final
        {
            if constexpr (type_info<result_type>::is_void)
            {
                _function(forward<arg_types>(args)...);
            }
            else
            {
                return _function(forward<arg_types>(args)...);
            }
        }

    public:
        function_type _function;
    };

    /// --------------------------------------------------------------------------------------------
    /// stores an function value_type using boxing.
    /// --------------------------------------------------------------------------------------------
    template <typename result_type, typename... arg_types>
    class _function_box_impl
    {
        using this_type = _function_box_impl<result_type, arg_types...>;
        using _box_type = std::function<result_type(arg_types...)>;
        // using _interface_type = _function_box_interface<result_type, arg_types...>;
        // using _box_type = copy_move_box<_interface_type, true, 50>;

        // template <typename function_type>
        // using _wrapper_type = _function_box_wrapper<function_type, result_type, arg_types...>;

    public:
        class copy_tag
        {};

        class move_tag
        {};

        class value_tag
        {};

    public:
        _function_box_impl()
            : _box()
        {}

        _function_box_impl(copy_tag, const this_type& that)
            : _box(that._box)
        {}

        auto copy_that(const this_type& that)
        {
            _box = that._box;
        }

        _function_box_impl(move_tag, this_type& that)
            : _box(atom::move(that._box))
        {}

        auto move_that(this_type& that)
        {
            _box = atom::move(that._box);
        }

        template <typename function_type>
        _function_box_impl(value_tag, function_type&& function)
            : _box(forward<function_type>(function))
        {}

        ~_function_box_impl() {}

    public:
        /// ----------------------------------------------------------------------------------------
        /// stores function.
        /// ----------------------------------------------------------------------------------------
        template <typename function_type>
        auto set_function(function_type&& function)
        {
            // _box.set_val(_wrapper_type<function_type>(forward<function_type>(function)));
            _box = forward<function_type>(function);
        }

        /// ----------------------------------------------------------------------------------------
        /// get function.
        /// ----------------------------------------------------------------------------------------
        template <typename function_type>
        auto get_function_as() -> function_type*
        {
            // if (typeid(function_type) != get_function_type())
            //     return nullptr;

            // return _box.template mut_mem_as<function_type>();
            return _box.template get_type<function_type>();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        auto get_function_type() const -> const std::type_info&
        {
            // return _box.val_type();
            return _box.target_type();
        }

        /// ----------------------------------------------------------------------------------------
        /// get function.
        /// ----------------------------------------------------------------------------------------
        auto has_function() const -> bool
        {
            // return _box.has_val();
            return (bool)_box;
        }

        /// ----------------------------------------------------------------------------------------
        /// invokes stored function.
        /// ----------------------------------------------------------------------------------------
        auto invoke_function(arg_types&&... args) -> result_type
        {
            // return _box.get_mut().invoke(forward<arg_types>(args)...);
            return _box(forward<arg_types>(args)...);
        }

        /// ----------------------------------------------------------------------------------------
        /// destroys stored function if any.
        /// ----------------------------------------------------------------------------------------
        auto destroy_function()
        {
            // _box.destroy();
            _box = nullptr;
        }

    private:
        _box_type _box;
    };
}

/// ------------------------------------------------------------------------------------------------
/// apis
/// ------------------------------------------------------------------------------------------------
namespace atom
{
    /// --------------------------------------------------------------------------------------------
    /// [`function_box`] declaration.
    /// --------------------------------------------------------------------------------------------
    export template <typename signature>
    class function_box;

    /// --------------------------------------------------------------------------------------------
    /// stores an function value_type using boxing.
    /// --------------------------------------------------------------------------------------------
    export template <typename result_type, typename... arg_types>
    class function_box<result_type(arg_types...)>: public function_box_tag
    {
        using _impl_type = _function_box_impl<result_type, arg_types...>;

    public:
        /// ----------------------------------------------------------------------------------------
        /// # default constructor.
        /// ----------------------------------------------------------------------------------------
        constexpr function_box()
            : _impl()
        {}

        /// ----------------------------------------------------------------------------------------
        /// # copy constructor
        /// ----------------------------------------------------------------------------------------
        function_box(const function_box& that)
            : _impl(typename _impl_type::copy_tag(), that._impl)
        {}

        /// ----------------------------------------------------------------------------------------
        /// # copy operator
        /// ----------------------------------------------------------------------------------------
        auto operator=(const function_box& that) -> function_box&
        {
            _impl.copy_that(that._impl);
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # move constructor
        /// ----------------------------------------------------------------------------------------
        function_box(function_box&& that)
            : _impl(typename _impl_type::move_tag(), that._impl)
        {}

        /// ----------------------------------------------------------------------------------------
        /// # move operator
        /// ----------------------------------------------------------------------------------------
        auto operator=(function_box&& that) -> function_box&
        {
            _impl.move_that(that._impl);
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # null constructor.
        /// ----------------------------------------------------------------------------------------
        function_box(nullptr_t null)
            : _impl()
        {}

        /// ----------------------------------------------------------------------------------------
        /// # null operator.
        /// ----------------------------------------------------------------------------------------
        auto operator=(nullptr_t null) -> function_box&
        {
            _impl.destroy_function();
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # value constructor
        /// ----------------------------------------------------------------------------------------
        template <typename function_type>
        function_box(function_type&& function)
            requires(type_info<function_type>::template is_function<result_type(arg_types...)>)
                    and (type_info<function_type>::template is_not_derived_from<function_box_tag>)
            : _impl(typename _impl_type::value_tag(), forward<function_type>(function))
        {}

        /// ----------------------------------------------------------------------------------------
        /// # value operator
        /// ----------------------------------------------------------------------------------------
        template <typename function_type>
        function_box& operator=(function_type&& function)
            requires type_info<function_type>::template
        is_function<result_type(arg_types...)>
            and (type_info<function_type>::template is_not_derived_from<function_box_tag>)
        {
            _impl.set_function(forward<function_type>(function));
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # destructor
        /// ----------------------------------------------------------------------------------------
        ~function_box() {}

    public:
        /// ----------------------------------------------------------------------------------------
        /// stores the function.
        /// ----------------------------------------------------------------------------------------
        template <typename function_type>
        auto set(function_type&& function)
        {
            _impl.set_function(forward<function_type>(function));
        }

        /// ----------------------------------------------------------------------------------------
        /// returns stored function as a `mut_ptr` to `value_type`.
        /// ----------------------------------------------------------------------------------------
        template <typename value_type>
        auto get_as() -> value_type*
        {
            return _impl.template get_function_as<value_type>();
        }

        /// ----------------------------------------------------------------------------------------
        /// returns the typeid for the stored function.
        /// ----------------------------------------------------------------------------------------
        auto get_type() const -> const std::type_info&
        {
            return _impl.get_function_type();
        }

        /// ----------------------------------------------------------------------------------------
        /// invokes the stored function.
        /// ----------------------------------------------------------------------------------------
        auto invoke(arg_types&&... args) -> result_type
        {
            contract_expects(has(), "no function is present.");

            return _impl.invoke_function(forward<arg_types>(args)...);
        }

        /// ----------------------------------------------------------------------------------------
        /// invokes the stored function if any.
        /// ----------------------------------------------------------------------------------------
        auto invoke_typery(result_type* out, arg_types&&... args) -> result_type
        {
            if (not _impl.has_function())
                return;

            *out = _impl.invoke_function(forward<arg_types>(args)...);
        }

        /// ----------------------------------------------------------------------------------------
        /// returns `invoke(args...)`.
        /// ----------------------------------------------------------------------------------------
        auto operator()(arg_types&&... args) -> result_type
        {
            return invoke(forward<arg_types>(args)...);
        }

        /// ----------------------------------------------------------------------------------------
        /// returns `true` if this doesn't contain an function.
        /// ----------------------------------------------------------------------------------------
        auto has() const -> bool
        {
            return _impl.has_function();
        }

        /// ----------------------------------------------------------------------------------------
        /// destroys stored function if any.
        /// ----------------------------------------------------------------------------------------
        auto destroy()
        {
            return _impl.destroy_function();
        }

        /// ----------------------------------------------------------------------------------------
        /// returns `true` if this doesn't contain an function.
        /// ----------------------------------------------------------------------------------------
        auto operator==(nullptr_t null) const -> bool
        {
            return not _impl.has_function();
        }

    private:
        _impl_type _impl;
    };
}
