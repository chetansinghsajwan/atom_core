#pragma once
#include "atom/core/_std.h"
#include "atom/core/_fmt.h"
#include "atom/core/contracts_decl.h"
#include "atom/core/core.h"
#include "atom/core/core/source_location.h"
#include "atom/core/core/build_config.h"

namespace atom
{
    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    class contract_violation
    {
    public:
        contract_violation(contract_type type, std::string_view msg, source_location source)
            : type(type)
            , msg(msg)
            , source(source)
        {}

    public:
        contract_type type;
        std::string_view msg;
        source_location source;
    };

    constexpr auto _contract_type_to_string(contract_type type) -> std::string_view
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
                _contract_type_to_string(violation.type), violation.msg, violation.source.file_name,
                violation.source.line, violation.source.column, violation.source.func_name);
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
                          << "\n\tat: " << violation.source.file_name << ":"
                          << violation.source.line << ":" << violation.source.column << ": "
                          << violation.source.func_name << std::endl;

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
    inline auto _handle_contract_violation_impl(contract_type type, std::string_view expr,
        source_location source, std::string_view msg) -> void
    {
        contract_violation violation(type, msg, source);
        contract_violation_handler_manager::get_handler().handle(violation);
    }

    inline auto _panic(source_location source, std::string_view msg) -> void
    {
        std::cerr << msg << std::endl;
        std::terminate();
    }
}
