#include "Atom/String/StringView.h"
#include "Atom/Hash/Md5Hash.h"
#include "Atom/Hash/Md5HashGenerator.h"
#include "Atom/Hash/Md5HashParser.h"
#include "Atom/Hash/Md5HashStringifier.h"
#include "Atom/Hash/Sha1Hash.h"
#include "Atom/Hash/Sha1HashGenerator.h"
#include "Atom/Hash/Sha1HashParser.h"
#include "Atom/Hash/Sha1HashStringifier.h"

using namespace Atom;

struct Console
{
    void Out(StringView str)
    {
        std::wcout << std::wstring_view((const wchar_t*)str.Data(), str.Count()) << std::endl;
    }
};

static Console Console;

void TestMd5()
{
    Md5Hash{ 0 };
    Md5Hash{ 0,0 };
    Md5Hash{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0b0,0xb };

    Md5Hash hash0 = Md5HashGenerator()
        .ProcessBytes("Hello", 5)
        .Generate();

    Md5Hash hash1 = Md5HashParser()
        .Parse(TEXT("da39a3ee5e6b4b0d3255bfef95601890"));

    String str1 = Md5HashStringifier()
        .ToString(hash1);

    Console.Out(str1);
}

void TestSha1()
{
    Sha1Hash{ 0 };
    Sha1Hash{ 0,0 };
    Sha1Hash{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0b0,0xb };

    Sha1Hash hash0 = Sha1HashGenerator()
        .ProcessBytes("Hello", 5)
        .Generate();

    Sha1Hash hash1 = Sha1HashParser()
        .Parse(TEXT("da39a3ee5e6b4b0d3255bfef95601890afd80709"));

    String str1 = Sha1HashStringifier()
        .ToString(hash1);

    Console.Out(str1);
}

int main()
{
    // Console.Out(TEXT("Hello"));

    TestSha1();
    TestMd5();

    return 0;
}