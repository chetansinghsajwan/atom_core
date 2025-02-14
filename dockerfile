FROM mcr.microsoft.com/vscode/devcontainers/base:ubuntu-24.04 AS base

ARG CLANG_VERSION="18"
ARG CATCH2_VERSION="3.8.0"
ARG CPPTRACE_VERSION="0.7.5"
ARG FMT_VERSION="11.1.3"
ARG MAGIC_ENUM_VERSION="0.9.7"
ARG INSTALL_DIR="/out"

# -------------------------------------------------------------------------------------------------
# Install build tools
# -------------------------------------------------------------------------------------------------

RUN apt-get update \
    && apt-get install --yes --no-install-recommends \
    clang-$CLANG_VERSION \
    libc++-$CLANG_VERSION-dev \
    ninja-build \
    cmake \
    pkg-config \
    git \
    ca-certificates \
    && rm -rf /var/lib/apt/lists/*

ENV CC="clang-$CLANG_VERSION" \
    CXX="clang++-$CLANG_VERSION" \
    CXXFLAGS="-stdlib=libc++" \
    CMAKE_GENERATOR="Ninja"

# -------------------------------------------------------------------------------------------------
# Install catch2, required by
# - atom_core
# -------------------------------------------------------------------------------------------------

FROM base AS catch2-builder

RUN git clone "https://github.com/catchorg/catch2.git" \
    --depth 1 --branch v$CATCH2_VERSION \
    && cd catch2 \
    && cmake -S . -B build \
    && cmake --build build \
    && cmake --install build --prefix /out

# -------------------------------------------------------------------------------------------------
# Install cpptrace, required by
# - atom_core
# -------------------------------------------------------------------------------------------------

FROM base AS cpptrace-builder

RUN git clone "https://github.com/jeremy-rifkin/cpptrace.git" \
    --depth 1 --branch v$CPPTRACE_VERSION \
    && cd cpptrace \
    && cmake -S . -B build \
    && cmake --build build \
    && cmake --install build --prefix /out

# -------------------------------------------------------------------------------------------------
# Install fmt, required by
# - atom_core
# -------------------------------------------------------------------------------------------------

FROM base AS fmt-builder

RUN git clone "https://github.com/fmtlib/fmt.git" \
    --depth 1 --branch $FMT_VERSION \
    && cd fmt \
    && cmake -S . -B build \
    && cmake --build build \
    && cmake --install build --prefix /out

# -------------------------------------------------------------------------------------------------
# Install magic_enum, required by
# - atom_core
# -------------------------------------------------------------------------------------------------

FROM base AS magic_enum-builder

RUN git clone "https://github.com/neargye/magic_enum.git" \
    --depth 1 --branch v$MAGIC_ENUM_VERSION \
    && cd magic_enum \
    && cmake -S . -B build \
    && cmake --build build \
    && cmake --install build --prefix /out

# -------------------------------------------------------------------------------------------------
# Builder envrionment
# -------------------------------------------------------------------------------------------------

FROM base AS builder

# Install dependencies
COPY --from=catch2-builder /out /usr/local
COPY --from=cpptrace-builder /out /usr/local
COPY --from=fmt-builder /out /usr/local
COPY --from=magic_enum-builder /out /usr/

# -------------------------------------------------------------------------------------------------
# Development environment
# -------------------------------------------------------------------------------------------------

FROM builder AS devenv

# Install dev tools
RUN apt-get update \
    && apt-get install --yes --no-install-recommends \
    clang-format-$CLANG_VERSION \
    clang-tidy-$CLANG_VERSION \
    clangd-$CLANG_VERSION \
    lldb-$CLANG_VERSION \
    cmake-format \
    doxygen \
    vim \
    && rm -rf /var/lib/apt/lists/*

# -------------------------------------------------------------------------------------------------
# Runtime envrionment
# -------------------------------------------------------------------------------------------------

FROM builder AS runtime

COPY . .
RUN cmake -S . -B build \
    && cmake --build build --target atom_core \
    && cmake --install build --prefix $INSTALL_DIR
