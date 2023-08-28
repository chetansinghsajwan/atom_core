{
    pkgs ? import<nixos> { },
    unstablePkgs ? import<nixos-unstable> { }
}:
pkgs.mkShell
{
    nativeBuildInputs = [
        pkgs.cmake
        pkgs.ninja
        pkgs.gnumake
        pkgs.clang_16
        pkgs.gcc

        pkgs.glfw
        unstablePkgs.fmt
        pkgs.catch2_3
    ];
}