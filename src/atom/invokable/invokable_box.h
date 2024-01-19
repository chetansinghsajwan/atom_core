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

    template <typename result_type, typename... arg_types>
    class _invoker
    {
    public:
        template <typename tinvokable>
        auto set()
            requires(rinvokable<tinvokable, result_type(arg_types...)>)
        {
            _impl = [](mut_mem_ptr<void> obj, result_type& result, targs&&... args) {
                tinvokable& invokable = *static_cast<tinvokable*>(obj.unwrap());
                new (&result) result_type(invokable(forward<targs>(args)...));
            };
        }

        auto invoke(mut_mem_ptr<void> invokable, targs&&... args) -> result_type
        {
            result_type result;
            _impl(invokable, result, forward<targs>(args)...);

            return result;
        }

    protected:
        void (*_impl)(mut_mem_ptr<void> invokable, result_type& result, targs&&... args);
    };

    template <typename... targs>
    class _invoker<void, targs...>
    {
    public:
        template <rinvokable<void(targs...)> tinvokable>
        auto set()
        {
            _impl = [](void* obj, targs&&... args) {
                tinvokable& invokable = *reinterpret_cast<tinvokable*>(obj);
                invokable(fwd(args)...);
            };
        }

        auto invoke(mut_mem_ptr<void> invokable, arg_types&&... args) -> result_type
        {
            result_type result;
            _impl(invokable, result, forward<arg_types>(args)...);

            return result;
        }

    protected:
        void (*_impl)(mut_mem_ptr<void> invokable, result_type& result, arg_types&&... args);
    };

    template <typename... arg_types>
    class _invoker<void, arg_types...>
    {
    public:
        template <rinvokable<void(arg_types...)> tinvokable>
        auto set()
        {
            _impl = [](mut_mem_ptr<void> obj, arg_types&&... args) {
                tinvokable& invokable = *reinterpret_cast<tinvokable*>(obj.unwrap());
                invokable(forward<arg_types>(args)...);
            };
        }

        auto invoke(mut_mem_ptr<void> invokable, arg_types&&... args)
        {
            _impl(invokable, forward<arg_types>(args)...);
        }

    protected:
        void (*_impl)(mut_mem_ptr<void> invokable, arg_types&&... args);
    };

    /// --------------------------------------------------------------------------------------------
    /// invokable_box declaration.
    /// --------------------------------------------------------------------------------------------
    template <typename>
    class invokable_box;

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename result_type, typename... arg_types>
    class invokable_box<result_type(arg_types...)>
        : public object_box<true, true, true, 50, default_mem_allocator>
        , public _invokable_box_identifier
    {
    private:
        using _box_type = copy_move_box<void, true, 50>;
        using _invoker_type = _invoker<result_type, targs...>;

    public:
        /// ----------------------------------------------------------------------------------------
        /// default_constructor.
        /// ----------------------------------------------------------------------------------------
        constexpr invokable_box() {}

        /// ----------------------------------------------------------------------------------------
        /// null_constructor.
        /// ----------------------------------------------------------------------------------------
        invokable_box(null_type null)
            : _box()
        {}

        /// ----------------------------------------------------------------------------------------
        /// null_assignment_operator.
        /// ----------------------------------------------------------------------------------------
        auto operator=(null_type null) -> invokable_box&
        {
            _box.destroy();
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// null_equality_operator.
        /// ----------------------------------------------------------------------------------------
        auto eq(null_type null) const -> bool
        {
            return not _box.has_val();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename tinvokable>
            requires rinvokable<tinvokable, result_type(targs...)>
        invokable_box(tinvokable&& invokable)
            requires(not rderived_from<tinvokable, _invokable_box_id>)
            : _box_type(forward<tinvokable>(invokable))
        {
            _set_invoker<tinvokable>();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename tinvokable>
        invokable_box& operator=(tinvokable&& invokable)
            requires rinvokable<tinvokable, result_type(targs...)>
                     and (not rderived_from<tinvokable, _invokable_box_id>)
        {
            _box.operator=(forward<tinvokable>(invokable));
            _set_invoker<tinvokable>();
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        invokable_box(const invokable_box& other)
            : _box(other)
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
            : _box(mov(other._box))
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
        auto invoke(arg_types&&... args) -> result_type
        {
            contracts::expects(_box.has_val(), "invokable_target is null.");

            return _invoker.invoke(object_box::_get_object(), forward<arg_types>(args)...);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        auto operator()(arg_types&&... args) -> result_type
        {
            return invoke(forward<arg_types>(args)...);
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
            requires(rinvokable<tinvokable, result_type(arg_types...)>)
        {
            _invoker.template set<tinvokable>();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        auto _invoke_invokable(arg_types&&... args) -> result_type
        {
            return _invoker.invoke(forward<arg_types>(args)...);
        }

    private:
        _invoker_type _invoker;
        _box_type _box;
    };
}
