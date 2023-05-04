#include "Atom/String/StringView.h"

using namespace Atom;

struct Console
{
    void Out(StringView str)
    {
        std::wcout << std::wstring_view((const wchar_t*)str.Data(), str.Count()) << std::endl;
    }
};

static Console Console;

int main()
{
    return 0;
}