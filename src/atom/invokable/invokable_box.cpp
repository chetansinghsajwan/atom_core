export module atom.core:invokable_box;
import :std;
import :box;
import :core;
import :ptr;
import :invokable;
import :contracts_decl;

namespace atom
{
    class invokable_box_type_id
    {};

    template <typename result_type, typename... arg_types>
    class _invokable_box_interface
    {
    public:
        virtual auto invoke(arg_types... args) -> result_type = 0;
    };

    template <typename invokable_type, typename result_type, typename... arg_types>
    class _invokable_box_wrapper: public _invokable_box_interface<result_type, arg_types...>
    {
    public:
        _invokable_box_wrapper(invokable_type&& invokable)
            : _invokable(invokable)
        {}

    public:
        virtual auto invoke(arg_types... args) -> result_type override final
        {
            if constexpr (rsame_as<result_type, void>)
            {
                _invokable(forward<arg_types>(args)...);
            }
            else
            {
                return _invokable(forward<arg_types>(args)...);
            }
        }

    public:
        invokable_type _invokable;
    };

    /// --------------------------------------------------------------------------------------------
    /// stores an invokable type using boxing.
    /// --------------------------------------------------------------------------------------------
    template <typename result_type, typename... arg_types>
    class _invokable_box_impl
    {
        using this_type = _invokable_box_impl<result_type, arg_types...>;
        using _box_type = std::function<result_type(arg_types...)>;
        // using _interface_type = _invokable_box_interface<result_type, arg_types...>;
        // using _box_type = copy_move_box<_interface_type, true, 50>;

        // template <typename invokable_type>
        // using _wrapper_type = _invokable_box_wrapper<invokable_type, result_type, arg_types...>;

    public:
        class copy_tag
        {};

        class move_tag
        {};

        class value_tag
        {};

    public:
        _invokable_box_impl()
            : _box()
        {}

        _invokable_box_impl(copy_tag, const this_type& that)
            : _box(that._box)
        {}

        auto copy_that(const this_type& that)
        {
            _box = that._box;
        }

        _invokable_box_impl(move_tag, this_type& that)
            : _box(atom::move(that._box))
        {}

        auto move_that(this_type& that)
        {
            _box = atom::move(that._box);
        }

        template <typename invokable_type>
        _invokable_box_impl(value_tag, invokable_type&& invokable)
            : _box(forward<invokable_type>(invokable))
        {}

        ~_invokable_box_impl() {}

    public:
        /// ----------------------------------------------------------------------------------------
        /// stores invokable.
        /// ----------------------------------------------------------------------------------------
        template <typename invokable_type>
        auto set_invokable(invokable_type&& invokable)
        {
            // _box.set_val(_wrapper_type<invokable_type>(forward<invokable_type>(invokable)));
            _box = forward<invokable_type>(invokable);
        }

        /// ----------------------------------------------------------------------------------------
        /// get invokable.
        /// ----------------------------------------------------------------------------------------
        template <typename invokable_type>
        auto get_invokable_as() -> mut_ptr<invokable_type>
        {
            // if (typeid(invokable_type) != get_invokable_type())
            //     return nullptr;

            // return _box.template mut_mem_as<invokable_type>();
            return _box.template get_type<invokable_type>();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        auto get_invokable_type() const -> const std::type_info&
        {
            // return _box.val_type();
            return _box.target_type();
        }

        /// ----------------------------------------------------------------------------------------
        /// get invokable.
        /// ----------------------------------------------------------------------------------------
        auto has_invokable() const -> bool
        {
            // return _box.has_val();
            return (bool)_box;
        }

        /// ----------------------------------------------------------------------------------------
        /// invokes stored invokable.
        /// ----------------------------------------------------------------------------------------
        auto invoke_invokable(arg_types&&... args) -> result_type
        {
            // return _box.get_mut().invoke(forward<arg_types>(args)...);
            return _box(forward<arg_types>(args)...);
        }

        /// ----------------------------------------------------------------------------------------
        /// destroys stored invokable if any.
        /// ----------------------------------------------------------------------------------------
        auto destroy_invokable()
        {
            // _box.destroy();
            _box = nullptr;
        }

    private:
        _box_type _box;
    };
}

export namespace atom
{
    /// --------------------------------------------------------------------------------------------
    /// [`invokable_box`] declaration.
    /// --------------------------------------------------------------------------------------------
    template <typename signature>
    class invokable_box;

    /// --------------------------------------------------------------------------------------------
    /// stores an invokable type using boxing.
    /// --------------------------------------------------------------------------------------------
    template <typename result_type, typename... arg_types>
    class invokable_box<result_type(arg_types...)>: public invokable_box_type_id
    {
        using _impl_type = _invokable_box_impl<result_type, arg_types...>;

    public:
        /// ----------------------------------------------------------------------------------------
        /// # default constructor.
        /// ----------------------------------------------------------------------------------------
        constexpr invokable_box()
            : _impl()
        {}

        /// ----------------------------------------------------------------------------------------
        /// # copy constructor
        /// ----------------------------------------------------------------------------------------
        invokable_box(const invokable_box& that)
            : _impl(typename _impl_type::copy_tag(), that._impl)
        {}

        /// ----------------------------------------------------------------------------------------
        /// # copy operator
        /// ----------------------------------------------------------------------------------------
        auto operator=(const invokable_box& that) -> invokable_box&
        {
            _impl.copy_that(that._impl);
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # move constructor
        /// ----------------------------------------------------------------------------------------
        invokable_box(invokable_box&& that)
            : _impl(typename _impl_type::move_tag(), that._impl)
        {}

        /// ----------------------------------------------------------------------------------------
        /// # move operator
        /// ----------------------------------------------------------------------------------------
        auto operator=(invokable_box&& that) -> invokable_box&
        {
            _impl.move_that(that._impl);
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # null constructor.
        /// ----------------------------------------------------------------------------------------
        invokable_box(nullptr_type null)
            : _impl()
        {}

        /// ----------------------------------------------------------------------------------------
        /// # null operator.
        /// ----------------------------------------------------------------------------------------
        auto operator=(nullptr_type null) -> invokable_box&
        {
            _impl.destroy_invokable();
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # value constructor
        /// ----------------------------------------------------------------------------------------
        template <typename invokable_type>
        invokable_box(invokable_type&& invokable)
            requires rinvokable<invokable_type, result_type(arg_types...)>
                     and (not rderived_from<invokable_type, invokable_box_type_id>)
            : _impl(typename _impl_type::value_tag(), forward<invokable_type>(invokable))
        {}

        /// ----------------------------------------------------------------------------------------
        /// # value operator
        /// ----------------------------------------------------------------------------------------
        template <typename invokable_type>
        invokable_box& operator=(invokable_type&& invokable)
            requires rinvokable<invokable_type, result_type(arg_types...)>
                     and (not rderived_from<invokable_type, invokable_box_type_id>)
        {
            _impl.set_invokable(forward<invokable_type>(invokable));
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # destructor
        /// ----------------------------------------------------------------------------------------
        ~invokable_box() {}

    public:
        /// ----------------------------------------------------------------------------------------
        /// stores the invokable.
        /// ----------------------------------------------------------------------------------------
        template <typename invokable_type>
        auto set(invokable_type&& invokable)
        {
            _impl.set_invokable(forward<invokable_type>(invokable));
        }

        /// ----------------------------------------------------------------------------------------
        /// returns stored invokable as a `mut_ptr` to `type`.
        /// ----------------------------------------------------------------------------------------
        template <typename type>
        auto get_as() -> mut_ptr<type>
        {
            return _impl.template get_invokable_as<type>();
        }

        /// ----------------------------------------------------------------------------------------
        /// returns the typeid for the stored invokable.
        /// ----------------------------------------------------------------------------------------
        auto get_type() const -> const std::type_info&
        {
            return _impl.get_invokable_type();
        }

        /// ----------------------------------------------------------------------------------------
        /// invokes the stored invokable.
        /// ----------------------------------------------------------------------------------------
        auto invoke(arg_types&&... args) -> result_type
        {
            contracts::expects(has(), "no invokable is present.");

            return _impl.invoke_invokable(forward<arg_types>(args)...);
        }

        /// ----------------------------------------------------------------------------------------
        /// invokes the stored invokable if any.
        /// ----------------------------------------------------------------------------------------
        auto invoke_try(mut_ptr<result_type> out, arg_types&&... args) -> result_type
        {
            if (not _impl.has_invokable())
                return;

            *out = _impl.invoke_invokable(forward<arg_types>(args)...);
        }

        /// ----------------------------------------------------------------------------------------
        /// returns `invoke(args...)`.
        /// ----------------------------------------------------------------------------------------
        auto operator()(arg_types&&... args) -> result_type
        {
            return invoke(forward<arg_types>(args)...);
        }

        /// ----------------------------------------------------------------------------------------
        /// returns `true` if this doesn't contain an invokable.
        /// ----------------------------------------------------------------------------------------
        auto has() const -> bool
        {
            return _impl.has_invokable();
        }

        /// ----------------------------------------------------------------------------------------
        /// destroys stored invokable if any.
        /// ----------------------------------------------------------------------------------------
        auto destroy()
        {
            return _impl.destroy_invokable();
        }

        /// ----------------------------------------------------------------------------------------
        /// returns `true` if this doesn't contain an invokable.
        /// ----------------------------------------------------------------------------------------
        auto is_eq(nullptr_type null) const -> bool
        {
            return not _impl.has_invokable();
        }

    private:
        _impl_type _impl;
    };
}
