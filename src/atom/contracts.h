#pragma once
// #include "atom/core.h"
// #include "atom/core/source_line_info.h"

// #include "contracts_decl.h"

// #include "fmt/format.h"

// #include <iostream>

namespace atom
{
    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    enum class contract_type
    {
        pre_condition,
        assertion,
        post_condition,
        debug_pre_condition,
        debug_assertion,
        debug_post_condition
    };

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    class contract_violation
    {
    public:
        contract_type type;
        std::string_view msg;
        source_line_info src;
    };

    constexpr auto _contract_type_to_string(contract_type type) -> std::string_view
    {
        switch (type)
        {
            case contract_type::pre_condition:        return "pre_condition";
            case contract_type::assertion:            return "assertion";
            case contract_type::post_condition:       return "post_condition";
            case contract_type::debug_pre_condition:  return "debug_pre_condition";
            case contract_type::debug_assertion:      return "debug_assertion";
            case contract_type::debug_post_condition: return "debug_post_condition";
            default:                                  return "[invalid_value]";
        }
    }

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    class contract_violation_exception: public std::exception
    {
    public:
        contract_violation_exception(contract_violation violation)
            : violation{ violation }
        {
            _what = fmt::format("contracts {} violation:"
                                "\n\twith msg: {}"
                                "\n\tat: {}: {}: {}"
                                "\n\tfunc: {}",
                _contract_type_to_string(violation.type), violation.msg, violation.src.file_name,
                violation.src.line.unwrap(), violation.src.column.unwrap(),
                violation.src.func_name);
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
                throw contract_violation_exception{ violation };
            }
            else
            {
                std::cout << "contracts " << _contract_type_to_string(violation.type)
                          << " violation:"
                          << "\n\twith msg: " << violation.msg << "'"
                          << "\n\tat: " << violation.src.file_name << ":"
                          << violation.src.line.unwrap() << ":" << violation.src.column.unwrap()
                          << ": " << violation.src.func_name << std::endl;

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
        static default_contract_violation_handler _default_handler;
        static contract_violation_handler* _handler;
    };

    inline default_contract_violation_handler contract_violation_handler_manager::_default_handler =
        default_contract_violation_handler();
    inline contract_violation_handler* contract_violation_handler_manager::_handler =
        &_default_handler;

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    extern "C++" auto _contract_check_impl(
        _contract_type type, std::source_location src, std::string_view msg) -> void
    {
        source_line_info src_info{ .line = src.line(),
            .column = src.column(),
            .func_name = src.function_name(),
            .file_name = src.file_name() };

        contract_violation violation{ .type = contract_type(type), .msg = msg, .src = src_info };

        contract_violation_handler_manager::get_handler().handle(violation);
    }

    extern "C++" auto _panic(std::source_location src, std::string_view msg) -> void
    {
        std::cerr << msg << std::endl;
        std::terminate();
    }
}
