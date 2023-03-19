#include <iostream>
#include "Atom/Engine.h"
#include "Atom/Logging/SimpleLogger.h"
#include "Atom/Logging/LogTargets/ConsoleLogTarget.h"

using namespace Atom;
using namespace Atom::Engine;
using namespace Atom::Logging;

class Sandbox: public Application { };

namespace Atom::Engine
{
    extern Application* CreateApplication()
    {
        SharedPtr<ConsoleLogTarget> target = MakeShared<ConsoleLogTarget>();
        SimpleLoggerST logger(TEXT("Test"), target);
        logger.SetLogLevel(LogLevel::Debug);

        logger.LogWarn(TEXT("Just a warning"));

        return new Sandbox();
    }
}