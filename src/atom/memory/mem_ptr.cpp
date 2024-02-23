export module atom.core:mem_ptr;
import :core;
import :ptr;
import :requirements;

namespace atom
{
    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename in_base_type, typename final_type>
    class mem_ptr_functions: public in_base_type
    {
        using this_type = mem_ptr_functions<in_base_type, final_type>;
        using base_type = in_base_type;

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
        constexpr auto next() const -> final_type
        {
            return _make(_arithmetic(_ptr) + 1);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto prev() const -> final_type
        {
            return _make(_arithmetic(_ptr) - 1);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto next(usize i) const -> final_type
        {
            return _make(_arithmetic(_ptr) + i.to_unwrapped());
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto prev(usize i) const -> final_type
        {
            return _make(_arithmetic(_ptr) - i.to_unwrapped());
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto mov(isize i) const -> final_type
        {
            return _make(_arithmetic(_ptr) + i.to_unwrapped());
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto mov(usize i) const -> final_type
        {
            return _make(_arithmetic(_ptr) + i.to_unwrapped());
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto sub(this_type that) const -> final_type
        {
            return _make(_arithmetic(_ptr) - that.to_unwrapped());
        }

    public:
        constexpr auto operator[](isize n) const
        {
            return base_type::to_unwrapped()[n.to_unwrapped()];
        }

        constexpr auto operator[](usize n) const
        {
            return base_type::to_unwrapped()[n.to_unwrapped()];
        }

        constexpr auto operator+(isize n) const -> final_type
        {
            return _make(_arithmetic(_ptr) + n.to_unwrapped());
        }

        constexpr auto operator+(usize n) const -> final_type
        {
            return _make(_arithmetic(_ptr) + n.to_unwrapped());
        }

        constexpr auto operator-(isize n) const -> final_type
        {
            return _make(_arithmetic(_ptr) - n.to_unwrapped());
        }

        constexpr auto operator-(usize n) const -> final_type
        {
            return _make(_arithmetic(_ptr) - n.to_unwrapped());
        }

        constexpr auto operator++(int) -> final_type
        {
            auto tmp = _make(_ptr);
            _ptr = _arithmetic(_ptr) + 1;
            return tmp;
        }

        constexpr auto operator--(int) -> final_type
        {
            auto tmp = _make(_ptr);
            _ptr = _arithmetic(_ptr) - 1;
            return tmp;
        }

        constexpr auto operator+=(isize n) -> final_type&
        {
            _ptr = _arithmetic(_ptr) + n.to_unwrapped();
            return _mut_this();
        }

        constexpr auto operator+=(usize n) -> final_type&
        {
            _ptr = _arithmetic(_ptr) + n.to_unwrapped();
            return _mut_this();
        }

        constexpr auto operator-=(isize n) -> final_type&
        {
            _ptr = _arithmetic(_ptr) - n.to_unwrapped();
            return _mut_this();
        }

        constexpr auto operator-=(usize n) -> final_type&
        {
            _ptr = _arithmetic(_ptr) - n.to_unwrapped();
            return _mut_this();
        }

        constexpr auto operator-(this_type ptr) const -> isize
        {
            return _arithmetic(_ptr) - _arithmetic(ptr._ptr);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto is_gt(this_type ptr) const -> bool
        {
            return _ptr > ptr._ptr;
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto is_ge(this_type ptr) const -> bool
        {
            return _ptr >= ptr._ptr;
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto is_lt(this_type ptr) const -> bool
        {
            return _ptr < ptr._ptr;
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto is_le(this_type ptr) const -> bool
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

        static constexpr auto _make(const value_type* ptr) -> final_type
        {
            return final_type(const_cast<value_type*>(ptr));
        }

        constexpr auto _this() const -> const final_type&
        {
            return static_cast<const final_type&>(*this);
        }

        constexpr auto _mut_this() -> final_type&
        {
            return static_cast<final_type&>(*this);
        }

    protected:
        using base_type::_ptr;
    };

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    export template <typename type>
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

    export template <typename type>
    mem_ptr(const type* ptr) -> mem_ptr<type>;

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    export template <typename type>
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

    export template <typename type>
    mut_mem_ptr(const type* ptr) -> mut_mem_ptr<type>;
}
