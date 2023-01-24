#!/bin/bash
set -e

#on le garde juste pour la compat
hash make 2>/dev/null || { echo >&2 "I require make but it's not installed.  Aborting."; exit 1; }
hash g++ 2>/dev/null || { echo >&2 "I require g++ but it's not installed.  Aborting."; exit 1; }

echo "compilation being executed with the arguments;"
echo "$@"
make