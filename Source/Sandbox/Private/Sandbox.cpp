#include <iostream>
#include "Atom/Range.h"
#include "Atom/Engine.h"
#include "Atom/Logging/SimpleLogger.h"
#include "Atom/Logging/LogTargets/ConsoleLogTarget.h"

using namespace Atom;
using namespace Atom::Engine;
using namespace Atom::Logging;

class SandboxApplication: public Application { };

namespace Atom::Engine
{
    extern Application* CreateApplication()
    {
        SharedPtr<ConsoleLogTarget> target = MakeShared<ConsoleLogTarget>();

        // FIX: Fix this range usage in Clang.
        SimpleLoggerST logger(TEXT("Test"), Range{InitList<LogTargetPtr>{ target, target }});
        logger.SetLogLevel(ELogLevel::Debug);

        logger.LogWarn(TEXT("Just a warning"));

        return new SandboxApplication();
    }
}