#pragma once
#include "Atom/String/String.h"

namespace Atom
{
    /// --------------------------------------------------------------------------------------------
    /// Converts objects to string.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    class StringConverter
    {
        static_assert(sizeof(T) == 0, "StringConverter for type is not defined.");
    };

    /// --------------------------------------------------------------------------------------------
    /// Requirements for objects which are convertible to string. This checks for valid 
    /// {StringConverter<T>} specialization.
    /// 
    /// @TPARAM[IN] T Type to convert to stirng.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    concept RStringConvertible = requires(T t)
    {
        StringConverter<T>::Convert(t);

        StringConverter<T>::Convert({}, t);
        StringConverter<T>::Convert({}, MOVE(t));
    };

    /// --------------------------------------------------------------------------------------------
    /// {StringConverter} specialization for {String}.
    /// --------------------------------------------------------------------------------------------
    template < >
    class StringConverter<String>
    {
        static constexpr String Convert(String&& str) noexcept
        {
            return MOVE(str);
        }

        static constexpr String Convert(const String& str) noexcept
        {
            return String{ str };
        }

        static constexpr void Convert(const String& str, RBackInsertable<Char> auto& out) noexcept
        {
            out.InsertBack(str);
        }
    };
}