{
    description = "atom.core";

    inputs = {
        nixpkgs.url = "github:nixos/nixpkgs/nixos-unstable";
    };

    outputs = { self, nixpkgs }:
    let
        system = "x86_64-linux";
        pkgs = nixpkgs.legacyPackages.${system};
        stdenv = pkgs.llvmPackages_17.libcxxStdenv;
    in
    {
        devShells.${system}.default = stdenv.mkDerivation {
            name = "atom.core";
            buildInputs = with pkgs; [
                cmake-format
                coreutils
                git
                cmake
                ninja
            ];
        };
    };
}
