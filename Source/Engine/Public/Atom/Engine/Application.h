#pragma once
#include "Atom/Core.h"

namespace Atom::Engine
{
    class ATOM_API Application
    {
    public:
        Application();
        virtual ~Application();

    public:
        virtual void Run()
        {
            while (true)
            {
            }
        }
    };

    extern Application* CreateApplication();
}