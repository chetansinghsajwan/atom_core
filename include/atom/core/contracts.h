#pragma once
#include "atom/core/core/build_config.h"
#include "atom/core/core/source_location.h"

#include "fmt/format.h"

namespace atom
{
    /// --------------------------------------------------------------------------------------------
    /// type of contract.
    /// --------------------------------------------------------------------------------------------
    enum class contract_type
    {
        /// pre checks performed at function start.
        expects,

        /// assertion between pre checks and post checks.
        asserts,

        /// post checks performed before function ends.
        ensures,

        /// same as `expects` but only checked in debug mode.
        debug_expects,

        /// same as `asserts` but only checked in debug mode.
        debug_asserts,

        /// same as `ensures` but only checked in debug mode.
        debug_ensures
    };

    /// --------------------------------------------------------------------------------------------
    /// type containing information about the contract violation.
    /// --------------------------------------------------------------------------------------------
    class contract_violation
    {
    public:
        contract_violation(
            contract_type type, std::string_view expr, std::string_view msg, source_location source)
            : type(type)
            , expr(expr)
            , msg(msg)
            , source(source)
        {}

    public:
        contract_type type;
        std::string_view msg;
        std::string_view expr;
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
    /// exception type used to represent a contract violation.
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
    /// interface for handling contract violations.
    /// --------------------------------------------------------------------------------------------
    class contract_violation_handler
    {
    public:
        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        static auto get() -> contract_violation_handler*
        {
            return _handler;
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        static auto set(contract_violation_handler* handler) -> void
        {
            _handler = handler;
            if (_handler == nullptr)
            {
                _handler = _default_handler;
            }
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        static auto set_default() -> void
        {
            _handler = _default_handler;
        }

    public:
        /// ----------------------------------------------------------------------------------------
        /// function invoked to handle contract violation.
        /// ----------------------------------------------------------------------------------------
        virtual auto handle(const contract_violation& violation) -> void = 0;

    private:
        static contract_violation_handler* _default_handler;
        static contract_violation_handler* _handler;
    };

    /// --------------------------------------------------------------------------------------------
    /// this type is used by default to handle contract violations.
    /// --------------------------------------------------------------------------------------------
    class default_contract_violation_handler final: public contract_violation_handler
    {
    public:
        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
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

    static inline default_contract_violation_handler _default_handler_instance;
    inline contract_violation_handler* contract_violation_handler::_default_handler =
        &_default_handler_instance;
    inline contract_violation_handler* contract_violation_handler::_handler =
        &_default_handler_instance;

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename... arg_types>
    constexpr inline auto _handle_contract_violation(contract_type type, std::string_view expr,
        source_location source, std::string_view fmt = "", arg_types&&... args) -> void
    {
        if (std::is_constant_evaluated())
            throw 0;

        contract_violation violation(type, expr, fmt, source);
        contract_violation_handler::get()->handle(violation);
    }

    template <typename... arg_types>
    inline auto _panic(source_location source, std::string_view fmt = "", arg_types&&... args)
        -> void
    {
        std::cerr << fmt << std::endl;
        std::terminate();
    }
}

#define VA_ARGS(...) , ##__VA_ARGS__

#define _ATOM_CONTRACT_CHECK(CONTRACT_TYPE, ASSERTION, ...)                                        \
    if (not(ASSERTION))                                                                            \
    atom::_handle_contract_violation(atom::contract_type::CONTRACT_TYPE, #ASSERTION,               \
        atom::source_location::current() VA_ARGS(__VA_ARGS__))

#if defined(ATOM_MODE_DEBUG)
#    define _ATOM_CONTRACT_CHECK_DEBUG(...) _ATOM_CONTRACT_CHECK(__VA_ARGS__)
#else
#    define _ATOM_CONTRACT_CHECK_DEBUG(...)
#endif

#define ATOM_EXPECTS(...) _ATOM_CONTRACT_CHECK(expects, __VA_ARGS__)
#define ATOM_ASSERTS(...) _ATOM_CONTRACT_CHECK(asserts, __VA_ARGS__)
#define ATOM_ENSURES(...) _ATOM_CONTRACT_CHECK(ensures, __VA_ARGS__)
#define ATOM_DEBUG_EXPECTS(...) _ATOM_CONTRACT_CHECK_DEBUG(debug_expects, __VA_ARGS__)
#define ATOM_DEBUG_ASSERTS(...) _ATOM_CONTRACT_CHECK_DEBUG(debug_asserts, __VA_ARGS__)
#define ATOM_DEBUG_ENSURES(...) _ATOM_CONTRACT_CHECK_DEBUG(debug_ensures, __VA_ARGS__)
#define ATOM_STATIC_ASSERTS(...) static_assert(__VA_ARGS__)
#define ATOM_PANIC(...) atom::_panic(atom::source_location::current() VA_ARGS(__VA_ARGS__))
