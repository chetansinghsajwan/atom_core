#include "Atom/Engine/Application.h"

namespace Atom::Engine
{
    Application::Application()
    {
        WindowProps windowProps
        {
            .windowName = TEXT("Sandbox"),
            .windowSize = { 1920, 1080 }
        };

        _window = WindowManger::CreateWindow(windowProps);
        ATOM_DEBUG_EXPECTS(_window != nullptr);

        _window->OnEvent += [this](const SWindowEvent& event)
        {
            this->OnWindowEvent(event);
        };
    }

    Application::~Application()
    {
        if (_window != nullptr)
        {
            WindowManger::CloseWindow(_window);
        }
    }

    void Application::Run()
    {
        while (true)
        {
            _window->Update();
        }
    }

    void Application::OnWindowEvent(const SWindowEvent& event) { }
}