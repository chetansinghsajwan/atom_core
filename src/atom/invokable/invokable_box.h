#pragma once
#include "atom/exceptions.h"
#include "atom/memory/object_box.h"
#include "atom/string.h"
#include "atom/tti.h"

#include "atom/invokable/invokable.h"

namespace atom
{
    class _invokable_box_identifier
    {};

    template <typename result_type, typename... args_type>
    class _invoker
    {
    public:
        template <typename tinvokable>
        auto set()
            requires(rinvokable<tinvokable, result_type(args_type...)>)
        {
            _impl = [](mut_mem_ptr<void> obj, result_type& result, args_type&&... args) {
                tinvokable& invokable = *static_cast<tinvokable*>(obj.unwrap());
                new (&result) result_type(invokable(forward<args_type>(args)...));
            };
        }

        auto invoke(mut_mem_ptr<void> invokable, args_type&&... args) -> result_type
        {
            result_type result;
            _impl(invokable, result, forward<args_type>(args)...);

            return result;
        }

    protected:
        void (*_impl)(mut_mem_ptr<void> invokable, result_type& result, args_type&&... args);
    };

    template <typename... args_type>
    class _invoker<void, args_type...>
    {
    public:
        template <rinvokable<void(args_type...)> tinvokable>
        auto set()
        {
            _impl = [](mut_mem_ptr<void> obj, args_type&&... args) {
                tinvokable& invokable = *reinterpret_cast<tinvokable*>(obj.unwrap());
                invokable(forward<args_type>(args)...);
            };
        }

        auto invoke(mut_mem_ptr<void> invokable, args_type&&... args)
        {
            _impl(invokable, forward<args_type>(args)...);
        }

    protected:
        void (*_impl)(mut_mem_ptr<void> invokable, args_type&&... args);
    };

    /// --------------------------------------------------------------------------------------------
    /// invokable_box declaration.
    /// --------------------------------------------------------------------------------------------
    template <typename>
    class invokable_box;

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename result_type, typename... args_type>
    class invokable_box<result_type(args_type...)>
        : public object_box<true, true, true, 50, default_mem_allocator>
        , public _invokable_box_identifier
    {
    private:
        using _TBox = CopyMoveBox<void, true, 50>;
        using _TInvoker = _Invoker<TResult, TArgs...>;

    public:
        /// ----------------------------------------------------------------------------------------
        /// default_constructor.
        /// ----------------------------------------------------------------------------------------
        constexpr invokable_box() {}

        /// ----------------------------------------------------------------------------------------
        /// null_constructor.
        /// ----------------------------------------------------------------------------------------
        invokable_box(null_type null)
            : object_box(null)
        {}

        /// ----------------------------------------------------------------------------------------
        /// null_assignment_operator.
        /// ----------------------------------------------------------------------------------------
        auto operator=(null_type null) -> invokable_box&
        {
            object_box::operator=(null);
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// null_equality_operator.
        /// ----------------------------------------------------------------------------------------
        auto eq(null_type null) const -> bool
        {
            return object_box::eq(null);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename tinvokable>
            requires rinvokable<tinvokable, result_type(args_type...)>
        invokable_box(tinvokable&& invokable)
            requires(rnot_derived_from<tinvokable, _invokable_box_identifier>)
            : object_box(forward<tinvokable>(invokable))
        {
            _set_invoker<tinvokable>();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename tinvokable>
            requires rinvokable<tinvokable, result_type(args_type...)>
        auto operator=(tinvokable&& invokable) -> invokable_box& requires(
            rnot_derived_from<tinvokable, _invokable_box_identifier>) {
            object_box::operator=(forward<tinvokable>(invokable));
            _set_invoker<tinvokable>();
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        invokable_box(const invokable_box& other)
            : object_box(other)
        {}

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        auto operator=(const invokable_box& other) -> invokable_box&
        {
            object_box::operator=(other);
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        invokable_box(invokable_box&& other)
            : object_box(mov(other))
        {}

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        auto operator=(invokable_box&& other) -> invokable_box&
        {
            object_box::operator=(mov(other));
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        ~invokable_box() {}

    public:
        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        auto invoke(args_type&&... args) -> result_type
        {
            contracts::expects(object_box::_has_object(), "invokable_target is null.");

            return _invoker.invoke(object_box::_get_object(), forward<args_type>(args)...);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        auto operator()(args_type&&... args) -> result_type
        {
            return invoke(forward<args_type>(args)...);
        }

        template <typename type>
        auto get_invokable() -> mut_mem_ptr<type>
        {
            if (typeid(type) != get_invokable_type())
                return nullptr;

            return object_box::_get_object<type>();
        }

        auto get_invokable_type() const -> const type_info&
        {
            return object_box::_get_object_type();
        }

    protected:
        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename tinvokable>
        auto _set_invoker()
            requires(rinvokable<tinvokable, result_type(args_type...)>)
        {
            _invoker.template set<tinvokable>();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        auto _invoke_invokable(args_type&&... args) -> result_type
        {
            return _invoker.invoke(forward<args_type>(args)...);
        }

        using tinvoker = _invoker<result_type, args_type...>;

        tinvoker _invoker;
    };
}
