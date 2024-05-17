#pragma once
#include "atom/core/core/build_config.h"
#include "atom/core/core/source_location.h"

namespace atom
{
    template <auto type, typename... arg_types>
    constexpr auto _contract_check(source_location src, bool assert, arg_types&&... args) -> void;

    inline auto _panic(source_location src, std::string_view msg, auto&&... args) -> void;

    constexpr auto _contract_type_to_string(auto type) -> std::string_view;

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    enum class contract_type
    {
        expects,
        asserts,
        ensures,
        debug_expects,
        debug_asserts,
        debug_ensures
    };

    /// ------------------------------------------------------------------------------------------------
    /// represents pre condition.
    /// ------------------------------------------------------------------------------------------------
    constexpr auto contract_expects(
        bool assert, std::string_view msg = "", source_location _src = source_location::current())
    {
        _contract_check<contract_type::expects>(_src, assert, msg);
    }

    /// ------------------------------------------------------------------------------------------------
    /// represents debug pre condition.
    /// ------------------------------------------------------------------------------------------------
    constexpr auto contract_debug_expects(
        bool assert, std::string_view msg = "", source_location _src = source_location::current())
    {
        _contract_check<contract_type::debug_expects>(_src, assert, msg);
    }

    /// ------------------------------------------------------------------------------------------------
    /// represents assertion.
    /// ------------------------------------------------------------------------------------------------
    constexpr auto contract_asserts(
        bool assert, std::string_view msg = "", source_location _src = source_location::current())
    {
        _contract_check<contract_type::asserts>(_src, assert, msg);
    }

    /// ------------------------------------------------------------------------------------------------
    /// represents debug assertion.
    /// ------------------------------------------------------------------------------------------------
    constexpr auto contract_debug_asserts(
        bool assert, std::string_view msg = "", source_location _src = source_location::current())
    {
        _contract_check<contract_type::debug_asserts>(_src, assert, msg);
    }

    /// ------------------------------------------------------------------------------------------------
    /// represents post condition.
    /// ------------------------------------------------------------------------------------------------
    constexpr auto contract_ensures(
        bool assert, std::string_view msg = "", source_location _src = source_location::current())
    {
        _contract_check<contract_type::ensures>(_src, assert, msg);
    }

    /// ------------------------------------------------------------------------------------------------
    /// represents debug post condition.
    /// ------------------------------------------------------------------------------------------------
    constexpr auto contract_debug_ensures(
        bool assert, std::string_view msg = "", source_location _src = source_location::current())
    {
        _contract_check<contract_type::debug_ensures>(_src, assert, msg);
    }

    /// ------------------------------------------------------------------------------------------------
    ///
    /// ------------------------------------------------------------------------------------------------
    template <typename... arg_types>
    constexpr auto contract_panic(
        std::string_view msg = "", source_location _src = source_location::current())
    {
        _panic(_src, msg);
    }

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    class contract_violation
    {
    public:
        contract_type type;
        std::string_view msg;
        source_location src;
    };

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    class contract_violation_exception: public std::exception
    {
    public:
        contract_violation_exception(contract_violation violation)
            : violation(violation)
        {
            _what = fmt::format("contracts {} violation:"
                                "\n\twith msg: {}"
                                "\n\tat: {}: {}: {}"
                                "\n\tfunc: {}",
                _contract_type_to_string(violation.type), violation.msg, violation.src.file_name,
                violation.src.line, violation.src.column, violation.src.func_name);
        }

    public:
        virtual auto what() const noexcept -> const char* override
        {
            return _what.data();
        }

    public:
        contract_violation violation;

    private:
        std::string _what;
    };

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    class contract_violation_handler
    {
    public:
        virtual auto handle(const contract_violation& violation) -> void = 0;
    };

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    class default_contract_violation_handler final: public contract_violation_handler
    {
    public:
        virtual auto handle(const contract_violation& violation) -> void override
        {
            if constexpr (build_config::is_mode_debug())
            {
                throw contract_violation_exception(violation);
            }
            else
            {
                std::cout << "contracts " << _contract_type_to_string(violation.type)
                          << " violation:"
                          << "\n\twith msg: " << violation.msg << "'"
                          << "\n\tat: " << violation.src.file_name << ":" << violation.src.line
                          << ":" << violation.src.column << ": " << violation.src.func_name
                          << std::endl;

                std::terminate();
            }
        }
    };

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    class contract_violation_handler_manager
    {
    public:
        static auto get_handler() -> contract_violation_handler&
        {
            return *_handler;
        }

        static auto set_handler(contract_violation_handler* handler)
        {
            _handler = handler;
            if (_handler == nullptr)
            {
                _handler = &_default_handler;
            }
        }

        static auto set_handler_to_default()
        {
            _handler = &_default_handler;
        }

    private:
        static inline default_contract_violation_handler _default_handler =
            default_contract_violation_handler();
        static inline contract_violation_handler* _handler = &_default_handler;
    };

    template <auto type, typename... arg_types>
    constexpr auto _contract_check(source_location src, bool assert, arg_types&&... args) -> void
    {
        if constexpr (build_config::is_mode_release())
        {
            if constexpr (type == contract_type::debug_expects
                          or type == contract_type::debug_asserts
                          or type == contract_type::debug_ensures)
                return;
        }

        if (assert)
            return;

        if (std::is_constant_evaluated())
            throw 0;

        std::string_view msg(std::forward<arg_types>(args)...);

        contract_violation violation{
            .type = contract_type(type),
            .msg = msg,
            .src = src,
        };

        contract_violation_handler_manager::get_handler().handle(violation);
    }

    inline auto _panic(source_location src, std::string_view msg, auto&&... args) -> void
    {
        std::cerr << msg << std::endl;
        std::terminate();
    }

    constexpr auto _contract_type_to_string(auto type) -> std::string_view
    {
        switch (type)
        {
            case contract_type::expects:       return "expects";
            case contract_type::asserts:       return "asserts";
            case contract_type::ensures:       return "ensures";
            case contract_type::debug_expects: return "debug_expects";
            case contract_type::debug_asserts: return "debug_asserts";
            case contract_type::debug_ensures: return "debug_ensures";
            default:                           return "[invalid_value]";
        }
    }
}
