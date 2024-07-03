{
    description = "atom_core";

    inputs = {
        nixpkgs.url = "github:nixos/nixpkgs/nixos-unstable";

        atom_doc = {
            url = "github:shifu-dev/atom-doc";
            inputs.nixpkgs.follows = "nixpkgs";
        };

        fmt = {
            url = "github:fmtlib/fmt";
            flake = false;
        };

        cpptrace = {
            url = "github:jeremy-rifkin/cpptrace";
            flake = false;
        };

        libdwarf-lite = {
            url = "github:jeremy-rifkin/libdwarf-lite";
            flake = false;
        };

        zstd = {
            url = "github:facebook/zstd";
            flake = false;
        };
    };

    outputs = inputs:
    let
        system = "x86_64-linux";
        pkgs = inputs.nixpkgs.legacyPackages.${system};
        lib = pkgs.lib;
        llvmPackages = pkgs.llvmPackages_18;
        stdenv = llvmPackages.libcxxStdenv;

        magic_enum_pkg = pkgs.magic-enum;
        atom_doc_pkg = inputs.atom_doc.packages.${system}.default;
        catch2_pkg = pkgs.catch2_3.override { stdenv = stdenv; };

        fmt_pkg = stdenv.mkDerivation {
            name = "fmt";
            src = inputs.fmt;

            nativeBuildInputs = with pkgs; [
                cmake
                ninja
            ];

            configurePhase = ''
                cmake -S . -B . -G Ninja \
                    -D FMT_DOC=OFF \
                    -D FMT_TEST=OFF \
                    -D CMAKE_INSTALL_PREFIX=$out;
            '';
        };

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
                fmt_pkg
                magic_enum_pkg
                cpptrace_pkg
            ];

            nativeBuildInputs = with pkgs; [
                atom_doc_pkg
                catch2_pkg

                cmake
                cmake-format
                ninja
                git
            ];

            configurePhase = ''
                cmake -S . -B build \
                    -D fmt_DIR:PATH=${fmt_pkg} \
                    -D magic_enum_DIR:PATH=${fmt_pkg} \
                    -D cpptrace_DIR:PATH=${cpptrace_pkg} \
                    -D Catch2_DIR:PATH=${catch2_pkg} \
                    -D CMAKE_INSTALL_PREFIX=$out;
            '';

            buildPhase = ''
                cmake --build build --target atom_core;
            '';

            installPhase = ''
                cmake --install build;
            '';

            clang_scan_deps_include_paths = [
                "/nix/store/2ykf9vnwl6s3nvisgd9vpzm74wxabysd-clang-18.1.7-lib/lib/clang/18/include"
                "/nix/store/fsb7lmhyy01flrnviwjfz3fgm53w990v-libcxx-18.1.7-dev/include/c++/v1"
                "/nix/store/fsb7lmhyy01flrnviwjfz3fgm53w990v-libcxx-18.1.7-dev/include"
                "/nix/store/il0vjm4nf1yv4swn0pi5rimh64hf3jrz-compiler-rt-libc-18.1.7-dev/include"
                "/nix/store/ip5wiylb41wli3yy33sqibqcj6l1yawl-clang-wrapper-18.1.7/resource-root/include"
                "/nix/store/4vgk1rlzdqjnpjicb5qcxjcd4spi7wyw-glibc-2.39-52-dev/include"

                "${fmt_pkg}/include"
                "${magic_enum_pkg}/include"
                "${cpptrace_pkg}/include"
                "${catch2_pkg}/include"
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
