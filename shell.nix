with (import <nixpkgs> {});
mkShell{
    buildInputs = [
        cmake
        gnumake
        ninja
        clang_17
        clang-tools_17
        gcc13
    ];
}