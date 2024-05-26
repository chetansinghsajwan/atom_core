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

    template <typename result_t, typename... arg_ts>
    class _function_box_interface
    {
    public:
        virtual auto invoke(arg_ts... args) -> result_t = 0;
    };

    template <typename function_t, typename result_t, typename... arg_ts>
    class _function_box_wrapper: public _function_box_interface<result_t, arg_ts...>
    {
    public:
        _function_box_wrapper(function_t&& function)
            : _function(function)
        {}

    public:
        virtual auto invoke(arg_ts... args) -> result_t override final
        {
            if constexpr (typeinfo<result_t>::is_void)
            {
                _function(forward<arg_ts>(args)...);
            }
            else
            {
                return _function(forward<arg_ts>(args)...);
            }
        }

    public:
        function_t _function;
    };

    /// --------------------------------------------------------------------------------------------
    /// stores an function value_t using boxing.
    /// --------------------------------------------------------------------------------------------
    template <typename result_t, typename... arg_ts>
    class _function_box_impl
    {
        using this_t = _function_box_impl<result_t, arg_ts...>;
        using _box_t = std::function<result_t(arg_ts...)>;
        // using _interface_t = _function_box_interface<result_t, arg_ts...>;
        // using _box_t = copy_move_box<_interface_t, true, 50>;

        // template <typename function_t>
        // using _wrapper_t = _function_box_wrapper<function_t, result_t, arg_ts...>;

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

        _function_box_impl(copy_tag, const this_t& that)
            : _box(that._box)
        {}

        auto copy_that(const this_t& that)
        {
            _box = that._box;
        }

        _function_box_impl(move_tag, this_t& that)
            : _box(atom::move(that._box))
        {}

        auto move_that(this_t& that)
        {
            _box = atom::move(that._box);
        }

        template <typename function_t>
        _function_box_impl(value_tag, function_t&& function)
            : _box(forward<function_t>(function))
        {}

        ~_function_box_impl() {}

    public:
        /// ----------------------------------------------------------------------------------------
        /// stores function.
        /// ----------------------------------------------------------------------------------------
        template <typename function_t>
        auto set_function(function_t&& function)
        {
            // _box.set_val(_wrapper_t<function_t>(forward<function_t>(function)));
            _box = forward<function_t>(function);
        }

        /// ----------------------------------------------------------------------------------------
        /// get function.
        /// ----------------------------------------------------------------------------------------
        template <typename function_t>
        auto get_function_as() -> function_t*
        {
            // if (typeid(function_t) != get_function_t())
            //     return nullptr;

            // return _box.template mut_mem_as<function_t>();
            return _box.template get_t<function_t>();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        auto get_function_t() const -> const std::type_info&
        {
            // return _box.val_t();
            return _box.target_t();
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
        auto invoke_function(arg_ts&&... args) -> result_t
        {
            // return _box.get_mut().invoke(forward<arg_ts>(args)...);
            return _box(forward<arg_ts>(args)...);
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
        _box_t _box;
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
    /// stores an function value_t using boxing.
    /// --------------------------------------------------------------------------------------------
    export template <typename result_t, typename... arg_ts>
    class function_box<result_t(arg_ts...)>: public function_box_tag
    {
        using _impl_t = _function_box_impl<result_t, arg_ts...>;

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
            : _impl(typename _impl_t::copy_tag(), that._impl)
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
            : _impl(typename _impl_t::move_tag(), that._impl)
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
        template <typename function_t>
        function_box(function_t&& function)
            requires(typeinfo<function_t>::template is_function<result_t(arg_ts...)>)
                    and (typeinfo<function_t>::template is_not_derived_from<function_box_tag>)
            : _impl(typename _impl_t::value_tag(), forward<function_t>(function))
        {}

        /// ----------------------------------------------------------------------------------------
        /// # value operator
        /// ----------------------------------------------------------------------------------------
        template <typename function_t>
        function_box& operator=(function_t&& function)
            requires typeinfo<function_t>::template
        is_function<result_t(arg_ts...)>
            and (typeinfo<function_t>::template is_not_derived_from<function_box_tag>)
        {
            _impl.set_function(forward<function_t>(function));
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
        template <typename function_t>
        auto set(function_t&& function)
        {
            _impl.set_function(forward<function_t>(function));
        }

        /// ----------------------------------------------------------------------------------------
        /// returns stored function as a `mut_ptr` to `value_t`.
        /// ----------------------------------------------------------------------------------------
        template <typename value_t>
        auto get_as() -> value_t*
        {
            return _impl.template get_function_as<value_t>();
        }

        /// ----------------------------------------------------------------------------------------
        /// returns the typeid for the stored function.
        /// ----------------------------------------------------------------------------------------
        auto get_t() const -> const std::type_info&
        {
            return _impl.get_function_t();
        }

        /// ----------------------------------------------------------------------------------------
        /// invokes the stored function.
        /// ----------------------------------------------------------------------------------------
        auto invoke(arg_ts&&... args) -> result_t
        {
            contract_expects(has(), "no function is present.");

            return _impl.invoke_function(forward<arg_ts>(args)...);
        }

        /// ----------------------------------------------------------------------------------------
        /// invokes the stored function if any.
        /// ----------------------------------------------------------------------------------------
        auto invoke_try(result_t* out, arg_ts&&... args) -> result_t
        {
            if (not _impl.has_function())
                return;

            *out = _impl.invoke_function(forward<arg_ts>(args)...);
        }

        /// ----------------------------------------------------------------------------------------
        /// returns `invoke(args...)`.
        /// ----------------------------------------------------------------------------------------
        auto operator()(arg_ts&&... args) -> result_t
        {
            return invoke(forward<arg_ts>(args)...);
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
        _impl_t _impl;
    };
}
