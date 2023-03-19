#include "catch2/catch_all.hpp"
#include "Atom/Logging/Log.h"

using namespace Atom::Logging;

TEST_CASE("STATIC_LOG", "[Implementation]")
{
    BENCHMARK("LOG")
    {
        LOG_TRACE(TEXT("HOLLA! This is an error."));
        return 0;
    };

    BENCHMARK("STATIC_LOG - Optimized")
    {
        STATIC_LOG_TRACE(TEXT("HOLLA! This is trace."));
        return 0;
    };

    BENCHMARK("STATIC_LOG - NotOptimized")
    {
        STATIC_LOG_ERROR(TEXT("HOLLA! This is error."));
        return 0;
    };

    BENCHMARK("EMPTY")
    {
        return 0;
    };
}