FROM ubuntu:latest
LABEL authors="dev"

# Set environment variables
ARG USERNAME=dev

# Install system dependencies and clean up
RUN apt-get update && \
    apt-get install -y  \
    build-essential \
    gdb \
    cmake \
    ninja-build \
    && rm -rf /var/lib/apt/lists/*

# Create a non-root user
RUN useradd -ms /bin/bash ${USERNAME}

# Set working directory and copy files
WORKDIR /home/${USERNAME}/app
COPY --chown=${USERNAME}:${USERNAME} . /home/${USERNAME}/app

# Ensure bootstrap.sh is executable
RUN chmod +x bootstrap.sh

# Switch to non-root user
USER ${USERNAME}

# Run bootstrap script
RUN ./bootstrap.sh

# Use bash as default shell
SHELL ["/bin/bash", "-c"]