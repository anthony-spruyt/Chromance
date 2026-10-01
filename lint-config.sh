#!/usr/bin/env bash
# shellcheck disable=SC2034 # Variables used by sourcing script (lint.sh)

# renovate: datasource=docker depName=ghcr.io/anthony-spruyt/megalinter-chromance
MEGALINTER_IMAGE="ghcr.io/anthony-spruyt/megalinter-chromance:1.1.0@sha256:12c6f486763ec18e8dda4cc059378efed159911db4a16240879bb8d35f626ce6"

SKIP_BOT_COMMITS=false

# MegaLinter flavor (use "all" for custom images to bypass flavor validation)
MEGALINTER_FLAVOR="all"
