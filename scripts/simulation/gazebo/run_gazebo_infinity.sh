#!/usr/bin/env bash

# Questo script verrà eseguito da run_sitl_infinty.sh e si occuperà di avviare gazebo (capire che modello e world eseguire)
# e inoltre avvierà il bridge per gazebo
SCRIPT_DIR="$(cd "$(dirname "$(realpath "$0")")" && pwd)"
PROJECT_ROOT="$(cd "$SCRIPT_DIR/../../.." && pwd)"

# avvio gazebo

source "$PROJECT_ROOT/.venv/bin/activate"

python "$SCRIPT_DIR/infinity_gazebo_bridge.py"