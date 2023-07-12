#include "Atom/Engine/Application.h"

using namespace Atom;

fn main(int argc, char** args) -> int
{
    Engine::Application* app = Engine::CreateApplication();
    app->Run();

    delete app;
}