#include "Atom/Engine/Application.h"

namespace Atom::Engine
{
    Application::Application()
    {
        WindowProps windowProps
        {
            .windowName = "Sandbox",
            .dimensions = { 1920, 1080 }
        };

        _window = WindowManger::CreateWindow(windowProps);
        ATOM_DEBUG_EXPECTS(_window != nullptr);
    }

    Application::~Application()
    {
        if (_window != nullptr)
        {
            WindowManger::CloseWindow(MOVE(_window));
        }
    }

    void Application::Run()
    {
        while (true)
        {
            _window->OnUpdate();
        }
    }
}