#pragma once
#include "atom/core/core.h"
#include "atom/core/memory/obj_helper.h"
#include "atom/core/types.h"
#include "atom/core/memory/default_mem_allocator.h"

namespace atom
{
    template <typename value_t>
    class shared_ptr;

    template <typename value_t>
    class unique_ptr_default_destroyer
    {
        ATOM_STATIC_ASSERTS(
            typeinfo<value_t>::is_pure, "unique_ptr_default_destroyer only supports pure types.");
        ATOM_STATIC_ASSERTS(
            not typeinfo<value_t>::is_void, "unique_ptr_default_destroyer does not support void.");

    public:
        constexpr auto operator()(value_t* val)
        {
            obj_helper().destruct_as<value_t>(val);
            default_mem_allocator().dealloc(val);
        }
    };

    template <typename in_value_t,
        typename in_destroyer_t = unique_ptr_default_destroyer<in_value_t>>
    class unique_ptr
    {
        ATOM_STATIC_ASSERTS(typeinfo<in_value_t>::is_pure, "unique_ptr only supports pure types.");
        ATOM_STATIC_ASSERTS(not typeinfo<in_value_t>::is_void, "unique_ptr does not support void.");
        ATOM_STATIC_ASSERTS(typeinfo<in_destroyer_t>::is_pure);
        ATOM_STATIC_ASSERTS(not typeinfo<in_destroyer_t>::is_void);

    private:
        template <typename other_value_t, typename other_destroyer_t>
        friend class unique_ptr;

    public:
        /// ----------------------------------------------------------------------------------------
        /// value_t of value `this_t` holds.
        /// ----------------------------------------------------------------------------------------
        using value_t = in_value_t;

        /// ----------------------------------------------------------------------------------------
        /// value_t of destroyer used to destroy value and dealloc memory.
        /// ----------------------------------------------------------------------------------------
        using destroyer_t = in_destroyer_t;

    public:
        /// ----------------------------------------------------------------------------------------
        /// # default constructor
        /// ----------------------------------------------------------------------------------------
        constexpr unique_ptr(destroyer_t destroyer = destroyer_t())
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
        template <typename other_value_t>
        constexpr unique_ptr(unique_ptr<other_value_t, destroyer_t>&& that)
            requires(typeinfo<other_value_t>::template is_same_or_derived_from<value_t>)
            : _ptr(that._ptr)
            , _destroyer(move(that._destroyer))
        {
            that._set_ptr(nullptr);
        }

        /// ----------------------------------------------------------------------------------------
        /// # template move operator
        /// ----------------------------------------------------------------------------------------
        template <typename other_value_t>
        constexpr unique_ptr& operator=(unique_ptr<other_value_t, destroyer_t>&& that)
            requires(typeinfo<other_value_t>::template is_same_or_derived_from<value_t>)
        {
            _move(move(that));
        }

        /// ----------------------------------------------------------------------------------------
        /// # value constructor
        /// ----------------------------------------------------------------------------------------
        constexpr explicit unique_ptr(value_t* ptr, destroyer_t destroyer = destroyer_t())
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
        constexpr auto set(value_t* ptr)
        {
            _check_and_destroy_value();
            _set_ptr(ptr);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto set(value_t* ptr, destroyer_t destroyer = destroyer_t())
        {
            _check_and_destroy_value();

            _set_ptr(ptr);
            _destroyer = move(destroyer);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto set(nullptr_t)
        {
            _check_and_destroy_value();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto release() -> value_t*
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

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename new_value_t = value_t>
        constexpr auto to_shared() -> shared_ptr<new_value_t>
            requires(typeinfo<new_value_t>::template is_same_or_derived_from<value_t>)
        {
            return _to_shared();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename allocator_t, typename new_value_t = value_t>
        constexpr auto to_shared_with_alloc(
            allocator_t allocator = allocator_t()) -> shared_ptr<new_value_t>
            requires(typeinfo<new_value_t>::template is_same_or_derived_from<value_t>)
        {
            return _to_shared();
        }

    private:
        template <typename other_value_t>
        constexpr auto _move(unique_ptr<other_value_t, destroyer_t>&& that)
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

        constexpr auto _release_value() -> value_t*
        {
            value_t* ptr = _get_mut_ptr();
            _set_ptr(nullptr);
            return ptr;
        }

        constexpr auto _destroy_value()
        {
            _destroyer(_get_mut_ptr());
        }

        constexpr auto _get_ptr() const -> const value_t*
        {
            return _ptr;
        }

        constexpr auto _get_mut_ptr() -> value_t*
        {
            return _ptr;
        }

        constexpr auto _set_ptr(value_t* ptr)
        {
            return _ptr = ptr;
        }

        template <typename allocator_t, typename other_value_t>
        constexpr auto _to_shared(allocator_t allocator) -> shared_ptr<other_value_t>;

    private:
        destroyer_t _destroyer;
        value_t* _ptr;
    };

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename value_t, typename... arg_ts>
    auto make_unique(arg_ts&&... args) -> unique_ptr<value_t>
    {
        return make_unique_with_alloc<value_t, default_mem_allocator>(
            default_mem_allocator(), forward<arg_ts>(args)...);
    }

    /// --------------------------------------------------------------------------------------------
    /// # todo: fix this implementation, maybe store allocator in destroyer.
    /// --------------------------------------------------------------------------------------------
    template <typename value_t, typename allocator_t, typename... arg_ts>
    auto make_unique_with_alloc(allocator_t allocator, arg_ts&&... args) -> unique_ptr<value_t>
    {
        value_t* mem = (value_t*)allocator.alloc(sizeof(value_t));
        obj_helper().construct_as<value_t>(mem, forward<arg_ts>(args)...);
        return unique_ptr<value_t>(mem);
    }
}
