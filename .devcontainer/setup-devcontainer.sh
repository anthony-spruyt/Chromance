#!/bin/bash
set -euo pipefail

# Implement custom devcontainer setup here. This is run after the devcontainer has been created.

# PlatformIO CLI (the VS Code extension installs its own copy on first launch; this one is for the terminal and Claude)
pipx install platformio
