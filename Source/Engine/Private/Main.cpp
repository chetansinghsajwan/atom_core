#include "Atom/Engine/Application.h"

using namespace Atom;

int main(int argc, char** args)
{
    Engine::Application* app = Engine::CreateApplication();
    app->Run();

    delete app;
}