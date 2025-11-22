FROM ubuntu:24.04
LABEL authors="dev"

# Set environment variables
ARG USERNAME=dev

# Install system dependencies and clean up
RUN apt-get update && \
    apt-get install -y  \
    git \
    build-essential \
    g++ \
    cmake \
    lua5.4 \
    liblua5.4-dev \
    libclang1-18 \
    ninja-build \
    && rm -rf /var/lib/apt/lists/*

# Create a non-root user
RUN useradd -ms /bin/bash ${USERNAME}

# Set working directory and copy files
WORKDIR /home/${USERNAME}/app
COPY --chown=${USERNAME}:${USERNAME} . /home/${USERNAME}/app
RUN chown -R ${USERNAME}:${USERNAME} /home/${USERNAME}/app

# Ensure bootstrap.sh is executable
RUN chmod +x bootstrap.sh

# Switch to non-root user
USER ${USERNAME}

# Run bootstrap script
RUN ./bootstrap.sh

# Run the application
CMD ["./build/Engine", "--host", "0.0.0.0"]

# Use bash as default shell
SHELL ["/bin/bash", "-c"]