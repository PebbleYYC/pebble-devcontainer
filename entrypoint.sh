#!/bin/zsh

echo "Detecting display system..."

# Detect if Wayland is available
if [[ -n "$WAYLAND_DISPLAY" ]]; then
    echo "Detected Wayland"
    export DISPLAY=""
    export WAYLAND_DISPLAY=$WAYLAND_DISPLAY
    export XDG_RUNTIME_DIR=${XDG_RUNTIME_DIR:-/tmp}
    echo "WAYLAND_DISPLAY is set to: $WAYLAND_DISPLAY"

# If Wayland is not available, fallback to X11
elif [[ -n "$DISPLAY" ]]; then
    echo "Detected X11"
    export DISPLAY=$DISPLAY
    export WAYLAND_DISPLAY=""
    export XDG_RUNTIME_DIR=${XDG_RUNTIME_DIR:-/tmp}
    echo "DISPLAY is set to: $DISPLAY"

# If neither is detected, use host.docker.internal:0
else
    echo "No GUI system detected, defaulting to host.docker.internal:0"
    export DISPLAY="host.docker.internal:0"
    export WAYLAND_DISPLAY=""
    export XDG_RUNTIME_DIR=${XDG_RUNTIME_DIR:-/tmp}
    echo "DISPLAY is set to: $DISPLAY"
fi

# Execute the original CMD or start an interactive shell
exec "$@"
