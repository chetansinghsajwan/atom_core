{
    description = "atom.core";

    inputs = {
        nixpkgs.url = "github:nixos/nixpkgs/nixos-unstable";

        cpptrace = {
            url = "github:jeremy-rifkin/cpptrace";
            flake = false;
        };

        libdwarf-lite = {
            url = "github:jeremy-rifkin/libdwarf-lite/v0.9.2";
            flake = false;
        };

        zstd = {
            url = "github:facebook/zstd/v1.5.5";
            flake = false;
        };
    };

    outputs = inputs:
    let
        system = "x86_64-linux";
        pkgs = inputs.nixpkgs.legacyPackages.${system};
        lib = pkgs.lib;
        stdenv = pkgs.llvmPackages_18.libcxxStdenv;

        cpptrace_pkg = stdenv.mkDerivation {

            name = "cpptrace";

            src = inputs.cpptrace;

            nativeBuildInputs = with pkgs; [
                cmake
                git
            ];

            configurePhase = ''
                cmake -S . -B build \
                    -D FETCHCONTENT_FULLY_DISCONNECTED:BOOL="ON" \
                    -D FETCHCONTENT_SOURCE_DIR_LIBDWARF:PATH="${inputs.libdwarf-lite}" \
                    -D FETCHCONTENT_SOURCE_DIR_ZSTD:PATH="${inputs.zstd}";
            '';

            buildPhase = ''
                cmake --build build;
            '';

            installPhase = ''
                cmake --install build --prefix $out;
            '';
        };

        clang_scan_deps_include_paths = lib.strings.concatStrings [
            " -I /nix/store/csml9b5w7z51yc7hxgd2ax4m6vj36iyq-libcxx-18.1.5-dev/include"
            " -I /nix/store/2sf9x4kf8lihldhnhp2b8q3ybas3p83l-compiler-rt-libc-18.1.5-dev/include"
            " -I /nix/store/hrssqr2jypca2qcqyy1xmfdw71nv6n14-catch2-3.5.2/include"
            " -I /nix/store/zc8xnz48ca61zjplxc3zz1ha3zss046p-fmt-10.2.1-dev/include"
            " -I /nix/store/2j35qpxbprdgcixyg70lyy6m0yay9352-magic-enum-0.9.5/include"
            " -I /nix/store/k3701zl6gmx3la7y4dnflcvf3xfy88kh-python3-3.11.9/include"
            " -I /nix/store/csml9b5w7z51yc7hxgd2ax4m6vj36iyq-libcxx-18.1.5-dev/include/c++/v1"
            " -I /nix/store/fymdqlxx6zsqvlmfwls3h2fly9kz0vcf-clang-wrapper-18.1.5/resource-root/include"
            " -I /nix/store/s3pvsv4as7mc8i2nwnk2hnsyi2qdj4bq-glibc-2.39-31-dev/include"
        ];

        derivation = stdenv.mkDerivation {

            name = "atom.core";

            src = ./.;

            propagatedBuildInputs = with pkgs; [
                fmt
                magic-enum
                cpptrace_pkg
            ];

            nativeBuildInputs = with pkgs; [
                catch2_3

                cmake
                cmake-format
                ninja
                git
            ];

            configurePhase = ''
                cmake -S . -B build \
                    -D fmt_DIR:PATH=${pkgs.fmt} \
                    -D Catch2_DIR:PATH=${pkgs.catch2_3} \
                    -D cpptrace_DIR:PATH=${cpptrace_pkg};
            '';

            buildPhase = ''
                cmake --build build --target atom.core;
            '';

            installPhase = ''
                cmake --install build --prefix $out;
            '';

            CXXFLAGS = clang_scan_deps_include_paths;
            CMAKE_GENERATOR = "Ninja";
            CMAKE_BUILD_TYPE = "Debug";
            CMAKE_EXPORT_COMPILE_COMMANDS = "true";
        };
    in
    {
        inherit clang_scan_deps_include_paths;

        devShells.${system}.default = derivation;

        packages.${system}.default = derivation;
    };
}
