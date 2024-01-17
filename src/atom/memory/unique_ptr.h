#pragma once
#include "atom/core.h"
#include "atom/memory/obj_helper.h"
#include "atom/memory/shared_ptr_decl.h"
#include "atom/tti.h"

namespace atom
{
    template <typename type>
    class unique_ptr_default_destroyer
    {
        static_assert(tti::is_pure<type>, "unique_ptr_default_destroyer only supports pure types.");
        static_assert(not tti::is_void<type>, "unique_ptr_default_destroyer doesn't support void.");

    public:
        constexpr auto operator()(mut_ptr<type> val)
        {
            obj_helper().destruct_as<type>(val);
            default_mem_allocator().dealloc(val);
        }
    };

    template <typename tin_val, typename tin_destroyer = unique_ptr_default_destroyer<tin_val>>
    class unique_ptr: public mut_ptr<tin_val>
    {
        static_assert(tti::is_pure<tin_val>, "unique_ptr only supports pure types.");
        static_assert(not tti::is_void<tin_val>, "unique_ptr doesn't support void.");
        static_assert(tti::is_pure<tin_destroyer>);
        static_assert(not tti::is_void<tin_destroyer>);

    private:
        template <typename tval1, typename tdestroyer1>
        friend class unique_ptr;

    private:
        using base_type = mut_ptr<tin_val>;

    public:
        /// ----------------------------------------------------------------------------------------
        /// type of value `this_type` holds.
        /// ----------------------------------------------------------------------------------------
        using value_type = tin_val;

        /// ----------------------------------------------------------------------------------------
        /// type of destroyer used to destroy value and dealloc memory.
        /// ----------------------------------------------------------------------------------------
        using tdestroyer = tin_destroyer;

    public:
        /// ----------------------------------------------------------------------------------------
        /// # default constructor
        /// ----------------------------------------------------------------------------------------
        constexpr unique_ptr(tdestroyer destroyer = tdestroyer())
            : base_type()
            , _destroyer(mov(destroyer))
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
            , _destroyer(mov(that._destroyer))
        {
            that._set_ptr(nullptr);
        }

        /// ----------------------------------------------------------------------------------------
        /// # move operator
        /// ----------------------------------------------------------------------------------------
        constexpr unique_ptr& operator=(unique_ptr&& that)
        {
            _move(mov(that));
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # template move constructor
        /// ----------------------------------------------------------------------------------------
        template <typename tval1>
        constexpr unique_ptr(unique_ptr<tval1, tdestroyer>&& that)
            requires rsame_or_derived_from<tval1, value_type>
            : base_type(that.unwrap())
            , _destroyer(mov(that._destroyer))
        {
            that._set_ptr(nullptr);
        }

        /// ----------------------------------------------------------------------------------------
        /// # template move operator
        /// ----------------------------------------------------------------------------------------
        template <typename tval1>
        constexpr unique_ptr& operator=(unique_ptr<tval1, tdestroyer>&& that)
            requires rsame_or_derived_from<tval1, value_type>
        {
            _move(mov(that));
        }

        /// ----------------------------------------------------------------------------------------
        /// # value constructor
        /// ----------------------------------------------------------------------------------------
        constexpr explicit unique_ptr(mut_ptr<value_type> ptr, tdestroyer destroyer = tdestroyer())
            : base_type(ptr)
            , _destroyer(mov(destroyer))
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
        constexpr auto set(mut_ptr<value_type> ptr, tdestroyer destroyer = tdestroyer())
        {
            _check_and_destroy_value();

            _set_ptr(ptr.unwrap());
            _destroyer = mov(destroyer);
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
        template <typename tallocator, typename type = value_type>
        constexpr auto to_shared_with_alloc(tallocator allocator = tallocator()) -> shared_ptr<type>
            requires rsame_or_derived_from<type, value_type>
        {
            return _to_shared();
        }

    private:
        template <typename tval1>
        constexpr auto _move(unique_ptr<tval1, tdestroyer>&& that)
        {
            _check_and_destroy_value();

            _set_ptr(that._get_mut_ptr());
            _destroyer = mov(that._destroyer);
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

        template <typename tallocator, typename tval1>
        constexpr auto _to_shared(tallocator allocator) -> shared_ptr<tval1>;

    private:
        tdestroyer _destroyer;
    };

    template <typename type, typename... args_type>
    auto make_unique(args_type&&... args) -> unique_ptr<type>
    {
        return std::make_unique<type>(forward<args_type>(args)...);
    }

    template <typename type, typename tallocator, typename... args_type>
    auto make_unique_with_alloc(tallocator allocator, args_type&&... args) -> unique_ptr<type>
    {
        return std::make_unique<type>(forward<args_type>(args)...);
    }
}
