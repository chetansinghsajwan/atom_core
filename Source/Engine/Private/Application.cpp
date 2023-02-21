#include "Atom/Engine/Application.h"

namespace Atom::Engine
{
    Application::Application() { }
    Application::~Application()
    {
        if (_window != nullptr)
        {
            WindowManger::CloseWindow(MOVE(_window));
        }
    }

    void Application::Run()
    {
        WindowProps windowProps
        {
            .windowName = "Sandbox",
            .dimensions = { 1920, 1080 }
        };

        _window = WindowManger::CreateWindow(windowProps);
        ATOM_DEBUG_EXPECTS(_window != nullptr);

        while (true)
        {
            _window->OnUpdate();
        }
    }
}