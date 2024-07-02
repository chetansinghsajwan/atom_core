{
    description = "atom_core";

    inputs = {
        nixpkgs.url = "github:nixos/nixpkgs/nixos-unstable";

        atom_doc = {
            url = "github:shifu-dev/atom-doc";
            inputs.nixpkgs.follows = "nixpkgs";
        };

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

        atom_doc_pkg = inputs.atom_doc.packages.${system}.default;

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
    in rec
    {
        env.${system}.default = rec {

            name = "atom_core";

            src = ./.;

            propagatedBuildInputs = with pkgs; [
                fmt
                magic-enum
                cpptrace_pkg
            ];

            nativeBuildInputs = with pkgs; [
                atom_doc_pkg
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
                cmake --build build --target atom_core;
            '';

            installPhase = ''
                cmake --install build --prefix $out;
            '';

            clang_scan_deps_include_paths = [
                "/nix/store/csml9b5w7z51yc7hxgd2ax4m6vj36iyq-libcxx-18.1.5-dev/include"
                "/nix/store/2sf9x4kf8lihldhnhp2b8q3ybas3p83l-compiler-rt-libc-18.1.5-dev/include"
                "/nix/store/hrssqr2jypca2qcqyy1xmfdw71nv6n14-catch2-3.5.2/include"
                "/nix/store/zc8xnz48ca61zjplxc3zz1ha3zss046p-fmt-10.2.1-dev/include"
                "/nix/store/2j35qpxbprdgcixyg70lyy6m0yay9352-magic-enum-0.9.5/include"
                "/nix/store/k3701zl6gmx3la7y4dnflcvf3xfy88kh-python3-3.11.9/include"
                "/nix/store/csml9b5w7z51yc7hxgd2ax4m6vj36iyq-libcxx-18.1.5-dev/include/c++/v1"
                "/nix/store/fymdqlxx6zsqvlmfwls3h2fly9kz0vcf-clang-wrapper-18.1.5/resource-root/include"
                "/nix/store/s3pvsv4as7mc8i2nwnk2hnsyi2qdj4bq-glibc-2.39-31-dev/include"
            ];

            envVars = {
                CXXFLAGS = lib.strings.concatMapStrings (v: " -I " + v) clang_scan_deps_include_paths;
                CMAKE_GENERATOR = "Ninja";
                CMAKE_BUILD_TYPE = "Debug";
                CMAKE_EXPORT_COMPILE_COMMANDS = "true";
                ATOM_DOC_DOXYFILE_DIR = atom_doc_pkg;
            };
        };

        devShells.${system}.default = with env.${system}.default; stdenv.mkDerivation ({
            inherit name;
            inherit src;
            inherit propagatedBuildInputs;
            inherit nativeBuildInputs;
        } // envVars);

        packages.${system}.default = with env.${system}.default; stdenv.mkDerivation ({
            inherit name;
            inherit src;
            inherit propagatedBuildInputs;
            inherit nativeBuildInputs;
            inherit configurePhase;
            inherit buildPhase;
            inherit installPhase;
        } // envVars);
    };
}
