#include "Atom/Engine/Application.h"

namespace Atom::Engine
{
    ctor Application::Application()
    {
        WindowProps windowProps
        {
            .windowName = TEXT("Sandbox"),
            .windowSize = { 1920, 1080 }
        };

        _window = WindowManger::CreateWindow(windowProps);
        ATOM_DEBUG_ASSERT(_window != nullptr);

        _window->OnEvent += [this](const SWindowEvent& event)
        {
            this->OnWindowEvent(event);
        };
    }

    Application::dtor Application()
    {
        if (_window != nullptr)
        {
            WindowManger::CloseWindow(_window);
        }
    }

    fn Application::Run() -> void
    {
        while (true)
        {
            _window->Update();
        }
    }

    fn Application::OnWindowEvent(const SWindowEvent& event) -> void { }
}