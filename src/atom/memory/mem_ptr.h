#pragma once
#include "atom/contracts.h"
#include "atom/memory/ptr.h"

namespace atom
{
    using memunit = byte;

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename tbase, typename this_final_type>
    class mem_ptr_functions: public tbase
    {
        using this_type = mem_ptr_functions<tbase, this_final_type>;
        using base_type = tbase;

    public:
        using value_type = typename base_type::value_type;

    public:
        template <typename type>
        constexpr mem_ptr_functions(ptr<type> that)
            : base_type(that)
        {}

        template <typename type>
        constexpr mem_ptr_functions(mut_ptr<type> that)
            : base_type(that)
        {}

        using base_type::base_type;
        using base_type::operator=;

    public:
        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto next() const -> this_final_type
        {
            return _make(_arithmetic(_ptr) + 1);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto prev() const -> this_final_type
        {
            return _make(_arithmetic(_ptr) - 1);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto next(usize i) const -> this_final_type
        {
            return _make(_arithmetic(_ptr) + i.unwrap());
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto prev(usize i) const -> this_final_type
        {
            return _make(_arithmetic(_ptr) - i.unwrap());
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto move(isize i) const -> this_final_type
        {
            return _make(_arithmetic(_ptr) + i.unwrap());
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto move(usize i) const -> this_final_type
        {
            return _make(_arithmetic(_ptr) + i.unwrap());
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto sub(this_type that) const -> this_final_type
        {
            return _make(_arithmetic(_ptr) - that.unwrap());
        }

    public:
        constexpr auto operator[](isize n) const
        {
            return base_type::unwrap()[n.unwrap()];
        }

        constexpr auto operator[](usize n) const
        {
            return base_type::unwrap()[n.unwrap()];
        }

        template <typename tint>
        constexpr auto operator+(tint n) const -> this_final_type
            requires rint<tint> or _rint<tint>
        {
            return _make(_arithmetic(_ptr) + _unwrap_int(n));
        }

        template <typename tint>
        constexpr auto operator-(tint n) const -> this_final_type
            requires rint<tint> or _rint<tint>
        {
            return _make(_arithmetic(_ptr) - _unwrap_int(n));
        }

        constexpr auto operator++(int) -> this_final_type
        {
            return _make(_arithmetic(_ptr) + 1);
        }

        constexpr auto operator--(int) -> this_final_type
        {
            return _make(_arithmetic(_ptr) - 1);
        }

        template <typename tint>
        constexpr auto operator+=(tint n) -> this_final_type&
            requires rint<tint> or _rint<tint>
        {
            _ptr = _arithmetic(_ptr) + _unwrap_int(n);
            return _mut_this();
        }

        template <typename tint>
        constexpr auto operator-=(tint n) -> this_final_type&
            requires rint<tint> or _rint<tint>
        {
            _ptr = _arithmetic(_ptr) - _unwrap_int(n);
            return _mut_this();
        }

        constexpr auto operator-(this_type ptr) const -> isize
        {
            return _arithmetic(_ptr) - _arithmetic(ptr._ptr);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto gt(this_type ptr) const -> bool
        {
            return _ptr > ptr._ptr;
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto ge(this_type ptr) const -> bool
        {
            return _ptr >= ptr._ptr;
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto lt(this_type ptr) const -> bool
        {
            return _ptr < ptr._ptr;
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto le(this_type ptr) const -> bool
        {
            return _ptr <= ptr._ptr;
        }

    private:
        static constexpr auto _arithmetic(const value_type* ptr)
        {
            if constexpr (rsame_as<value_type, void>)
            {
                return static_cast<const byte*>(ptr);
            }
            else
            {
                return ptr;
            }
        }

        static constexpr auto _arithmetic(value_type* ptr)
        {
            if constexpr (rsame_as<value_type, void>)
            {
                return static_cast<byte*>(ptr);
            }
            else
            {
                return ptr;
            }
        }

        static constexpr auto _make(const value_type* ptr) -> this_final_type
        {
            return this_final_type(const_cast<value_type*>(ptr));
        }

        constexpr auto _this() const -> const this_final_type&
        {
            return static_cast<const this_final_type&>(*this);
        }

        constexpr auto _mut_this() -> this_final_type&
        {
            return static_cast<this_final_type&>(*this);
        }

    protected:
        using base_type::_ptr;
    };

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename type>
    class mem_ptr: public mem_ptr_functions<ptr<type>, mem_ptr<type>>
    {
        using this_type = mem_ptr<type>;
        using base_type = mem_ptr_functions<ptr<type>, this_type>;

    public:
        constexpr mem_ptr(const ptr<type>& that)
            : base_type(that)
        {}

        using base_type::base_type;
        using base_type::operator=;
    };

    template <typename type>
    mem_ptr(const type* ptr) -> mem_ptr<type>;

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename type>
    class mut_mem_ptr: public mem_ptr_functions<mut_ptr<type>, mut_mem_ptr<type>>
    {
        using this_type = mut_mem_ptr<type>;
        using base_type = mem_ptr_functions<mut_ptr<type>, this_type>;

    public:
        constexpr mut_mem_ptr(const mut_ptr<type>& that)
            : base_type(that)
        {}

        using base_type::base_type;
        using base_type::operator=;
    };

    template <typename type>
    mut_mem_ptr(const type* ptr) -> mut_mem_ptr<type>;
}
