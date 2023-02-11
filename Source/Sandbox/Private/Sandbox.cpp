#include <iostream>
#include "Atom/Engine.h"

namespace Atom::Engine
{
    class Sandbox : public Application
    {
    public:
        Sandbox()
        {
            std::cout << "Sandbox" << std::endl;
        }
    };

    Application* CreateApplication()
    {
        return new Sandbox();
    }
}