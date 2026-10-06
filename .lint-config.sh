#!/usr/bin/env bash
# shellcheck disable=SC2034 # Variables used by sourcing script (lint.sh)
# This file is automatically updated - do not modify directly
# The image pin lives in repo-operator (src/groups.yaml, or src/repos.yaml for a per-repo flavor), where Renovate bumps it

MEGALINTER_IMAGE="ghcr.io/anthony-spruyt/megalinter-chromance:1.1.1@sha256:1e8cfe826b494afabd2a3bc56f6d7512651138a6b4116fe605dc5b6ba97efc43"

SKIP_BOT_COMMITS=false

# MegaLinter flavor (use "all" for custom images to bypass flavor validation)
MEGALINTER_FLAVOR="all"
