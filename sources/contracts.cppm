module;
#include "cpptrace.hpp"

export module atom.core:contracts;

import std;
import fmt;
import :core.build_config;
import :core.source_location;

namespace atom
{
    template <auto type>
    constexpr auto _contract_check(bool assert, std::string_view msg, source_location src) -> void;

    [[noreturn]]
    constexpr auto _panic(std::string_view msg, source_location src) -> void;

    constexpr auto _contract_violation_to_string(const auto& violation) -> std::string;
};

export namespace atom
{
    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    enum class contract_type
    {
        expects,
        asserts,
        ensures,
        panic,
        debug_expects,
        debug_asserts,
        debug_ensures,
        debug_panic,
    };

    /// ------------------------------------------------------------------------------------------------
    /// represents pre condition.
    /// ------------------------------------------------------------------------------------------------
    constexpr auto contract_expects(bool assert, std::string_view msg = "",
        source_location _src = source_location::current()) -> void
    {
        _contract_check<contract_type::expects>(assert, msg, _src);
    }

    /// ------------------------------------------------------------------------------------------------
    /// represents debug pre condition.
    /// ------------------------------------------------------------------------------------------------
    constexpr auto contract_debug_expects(bool assert, std::string_view msg = "",
        source_location _src = source_location::current()) -> void
    {
        _contract_check<contract_type::debug_expects>(assert, msg, _src);
    }

    /// ------------------------------------------------------------------------------------------------
    /// represents assertion.
    /// ------------------------------------------------------------------------------------------------
    constexpr auto contract_asserts(bool assert, std::string_view msg = "",
        source_location _src = source_location::current()) -> void
    {
        _contract_check<contract_type::asserts>(assert, msg, _src);
    }

    /// ------------------------------------------------------------------------------------------------
    /// represents debug assertion.
    /// ------------------------------------------------------------------------------------------------
    constexpr auto contract_debug_asserts(bool assert, std::string_view msg = "",
        source_location _src = source_location::current()) -> void
    {
        _contract_check<contract_type::debug_asserts>(assert, msg, _src);
    }

    /// ------------------------------------------------------------------------------------------------
    /// represents post condition.
    /// ------------------------------------------------------------------------------------------------
    constexpr auto contract_ensures(bool assert, std::string_view msg = "",
        source_location _src = source_location::current()) -> void
    {
        _contract_check<contract_type::ensures>(assert, msg, _src);
    }

    /// ------------------------------------------------------------------------------------------------
    /// represents debug post condition.
    /// ------------------------------------------------------------------------------------------------
    constexpr auto contract_debug_ensures(bool assert, std::string_view msg = "",
        source_location _src = source_location::current()) -> void
    {
        _contract_check<contract_type::debug_ensures>(assert, msg, _src);
    }

    /// ------------------------------------------------------------------------------------------------
    ///
    /// ------------------------------------------------------------------------------------------------
    template <typename... arg_types>
    [[noreturn]]
    constexpr auto contract_debug_panic(
        std::string_view msg = "", source_location _src = source_location::current()) -> void
        requires(build_config::is_mode_debug())
    {
        _panic(msg, _src);
    }

    /// ------------------------------------------------------------------------------------------------
    ///
    /// ------------------------------------------------------------------------------------------------
    template <typename... arg_types>
    constexpr auto contract_debug_panic(
        std::string_view msg = "", source_location _src = source_location::current()) -> void
        requires(not build_config::is_mode_debug())
    {}

    /// ------------------------------------------------------------------------------------------------
    ///
    /// ------------------------------------------------------------------------------------------------
    template <typename... arg_types>
    [[noreturn]]
    constexpr auto contract_panic(
        std::string_view msg = "", source_location _src = source_location::current()) -> void
    {
        _panic(msg, _src);
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
        cpptrace::stacktrace trace;
    };

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    class contract_violation_exception: public std::exception
    {
    public:
        contract_violation_exception(contract_violation violation)
            : violation(violation)
            , _what{ _contract_violation_to_string(violation) }
        {}

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
        static auto get() -> contract_violation_handler*
        {
            return _handler;
        }

        static auto set(contract_violation_handler* handler)
        {
            if (handler == nullptr)
            {
                handler = _default_handler;
            }

            _handler = handler;
        }

        static auto set_default()
        {
            _handler = _default_handler;
        }

    public:
        [[noreturn]]
        virtual auto handle(const contract_violation& violation) -> void = 0;

    private:
        static contract_violation_handler* _default_handler;
        static contract_violation_handler* _handler;
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
                std::string out = _contract_violation_to_string(violation);
                std::cerr << out << std::endl;
                std::terminate();
            }
        }
    };

    contract_violation_handler* contract_violation_handler::_default_handler =
        new default_contract_violation_handler{};

    contract_violation_handler* contract_violation_handler::_handler = _default_handler;
}

namespace atom
{
    template <auto type>
    constexpr auto _contract_check(bool assert, std::string_view msg, source_location src) -> void
    {
        if constexpr (not build_config::is_mode_debug())
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

        // 1 for this impl function and 1 for the api function.
        contract_violation violation{
            .type = type,
            .msg = msg,
            .src = src,
            .trace = cpptrace::stacktrace::current(2),
        };

        contract_violation_handler::get()->handle(violation);
    }

    constexpr auto _panic(std::string_view msg, source_location src) -> void
    {
        if (std::is_constant_evaluated())
            throw 0;

        // 1 for this impl function and 1 for the api function.
        contract_violation violation{
            .type = contract_type::panic,
            .msg = msg,
            .src = src,
            .trace = cpptrace::stacktrace::current(2),
        };

        contract_violation_handler::get()->handle(violation);
    }

    constexpr auto _contract_type_to_string(contract_type type) -> std::string_view
    {
        switch (type)
        {
            case contract_type::expects:       return "expects";
            case contract_type::asserts:       return "asserts";
            case contract_type::ensures:       return "ensures";
            case contract_type::panic:         return "panic";
            case contract_type::debug_expects: return "debug_expects";
            case contract_type::debug_asserts: return "debug_asserts";
            case contract_type::debug_ensures: return "debug_ensures";
            case contract_type::debug_panic:   return "debug_panic";
            default:                           return "[invalid_value]";
        }
    }

    constexpr auto _contract_violation_to_string(const auto& violation) -> std::string
    {
        std::string out = fmt::format("contracts {} violation:"
                                      "\n\twith msg: {}"
                                      "\n\tat: {}:{}:{}"
                                      "\n\tfunc: {}"
                                      "\n\ttrace: {}",
            _contract_type_to_string(violation.type), violation.msg, violation.src.file_name,
            violation.src.line, violation.src.column, violation.src.func_name,
            violation.trace.to_string());

        return out;
    }
}
