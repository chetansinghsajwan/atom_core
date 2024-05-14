module;

#include "fmt/core.h"
#include "fmt/format.h"
#include "fmt/chrono.h"

export module fmt;

export namespace fmt
{
    using fmt::format_string;
    using fmt::format_context;
    using fmt::format_parse_context;
    using fmt::formatter;
    using fmt::format;
    using fmt::format_to;
    using fmt::format_error;
    using fmt::runtime;
    using fmt::string_view;
}
