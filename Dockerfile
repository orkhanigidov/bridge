# ==========================================
# Stage 1: Build Environment
# ==========================================
FROM ubuntu:24.04 AS builder

SHELL ["/bin/bash", "-c"]

# Install build toolchain and dependencies
RUN apt-get update && \
    apt-get install -y --no-install-recommends \
    git \
    g++ \
    cmake \
    lua5.4 \
    liblua5.4-dev \
    libclang1-18 \
    ninja-build \
    ca-certificates && \
    rm -rf /var/lib/apt/lists/*

WORKDIR /src

# Copy repository source files
COPY . .

# Compile in Release mode (bootstrap.sh defaults to Debug)
RUN chmod +x bootstrap.sh && ./bootstrap.sh --build-type Release

# ==========================================
# Stage 2: Minimal Runtime Environment
# ==========================================
FROM ubuntu:24.04 AS runner

ARG USERNAME=engineuser

# Runtime libraries only (libclang is only needed at build time for codegen)
RUN apt-get update && \
    apt-get install -y --no-install-recommends \
    liblua5.4-0 \
    ca-certificates && \
    rm -rf /var/lib/apt/lists/*

# Create non-root system user
RUN useradd -ms /bin/bash ${USERNAME}

WORKDIR /home/${USERNAME}/app

# Copy compiled executable from Stage 1
COPY --from=builder --chown=${USERNAME}:${USERNAME} /src/build/Engine ./Engine

USER ${USERNAME}

EXPOSE 8000

CMD ["./Engine", "--host", "0.0.0.0", "--port", "8000"]