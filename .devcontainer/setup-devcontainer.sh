#!/bin/bash
set -euo pipefail

# Implement custom devcontainer setup here. This is run after the devcontainer has been created.

# PlatformIO CLI (pio) for the terminal and Claude; the VS Code extension brings its own copy
# renovate: depName=platformio datasource=pypi
PLATFORMIO_VERSION="6.2.0"
pipx install "platformio==${PLATFORMIO_VERSION}"
