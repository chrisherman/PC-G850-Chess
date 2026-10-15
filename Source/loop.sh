#!/bin/bash
set -e

# --- CONFIGURATION ---
REMOTE_USER="chrisherman"
REMOTE_HOST="homedocker.local"
REMOTE_DIR="~/z88dk/workspace"
CONTAINER_NAME="z88dk_g850v_env"
# ---------------------

# Get the directory where this script lives
SCRIPT_DIR="$(dirname "$0")"

if [ -z "$1" ]; then
    shopt -s nullglob
    C_FILES=("$SCRIPT_DIR"/*.c)
    shopt -u nullglob

    if [ ${#C_FILES[@]} -eq 0 ]; then
        echo "❌ Error: No C file provided and none found in $SCRIPT_DIR."
        echo "Usage: ./automame.sh [program.c]"
        exit 1
    elif [ ${#C_FILES[@]} -gt 1 ]; then
        echo "⚠️ Multiple .c files found in $SCRIPT_DIR."
        echo "👉 Defaulting to the first one: $(basename "${C_FILES[0]}")"
        FULL_PATH="${C_FILES[0]}"
    else
        echo "✅ Auto-detected C file: $(basename "${C_FILES[0]}")"
        FULL_PATH="${C_FILES[0]}"
    fi
else
    FULL_PATH="$1"
fi

# Extract the exact folder where the target .c file lives
DEST_DIR="$(dirname "$FULL_PATH")"
C_FILE="$(basename "$FULL_PATH")"
BASENAME="$(basename "$C_FILE" .c)"
IHX_FILE="${DEST_DIR}/${BASENAME}.ihx"
BIN_FILE="${DEST_DIR}/${BASENAME}.bin"

if [ ! -f "$FULL_PATH" ]; then
    echo "❌ Error: File '$FULL_PATH' not found locally."
    exit 1
fi

echo "📤 Uploading '$C_FILE' to $REMOTE_HOST..."
scp "$FULL_PATH" "${REMOTE_USER}@${REMOTE_HOST}:${REMOTE_DIR}/${C_FILE}"

echo "⚙️ Compiling on $REMOTE_HOST via z88dk container..."
ssh "${REMOTE_USER}@${REMOTE_HOST}" "docker exec -w /home/dev/workspace ${CONTAINER_NAME} zcc +g800 -clib=g850 -O3 --opt-code-size -create-app -o ${BASENAME}.bin ${C_FILE}"

echo "📥 Downloading generated '${BASENAME}.ihx' into '${DEST_DIR}'..."
scp "${REMOTE_USER}@${REMOTE_HOST}:${REMOTE_DIR}/${BASENAME}.ihx" "$IHX_FILE"

if [ ! -f "$IHX_FILE" ]; then
    echo "❌ Error: Failed to retrieve '$IHX_FILE'."
    exit 1
fi

echo "📥 Downloading generated '${BASENAME}.bin' into '${DEST_DIR}'..."
scp "${REMOTE_USER}@${REMOTE_HOST}:${REMOTE_DIR}/${BASENAME}.bin" "$BIN_FILE"

if [ ! -f "$BIN_FILE" ]; then
    echo "❌ Error: Failed to retrieve '$BIN_FILE'."
    exit 1
fi

# --- MEMORY INJECTION PREPARATION ---
# 1. Get exact file size in bytes (Mac / Linux safe)
if stat -f %z "$BIN_FILE" >/dev/null 2>&1; then
    BYTES=$(stat -f %z "$BIN_FILE") # macOS / BSD
else
    BYTES=$(stat -c %s "$BIN_FILE") # Linux / GNU
fi

# 2. Convert bytes to Hex format for MAME debugger
HEX_SIZE=$(printf "%X" "$BYTES")

echo "📊 Binary Size: $BYTES bytes (0x$HEX_SIZE)"

# 3. Create temporary MAME debug script
DEBUG_SCRIPT="${DEST_DIR}/temp_boot.txt"
cat << EOF > "$DEBUG_SCRIPT"
load "$BIN_FILE", 0100, $HEX_SIZE
go
EOF

# Ensure script is cleaned up when MAME exits or if script aborts
trap 'rm -f "$DEBUG_SCRIPT"' EXIT

# --- LAUNCH MAME ---
echo "🚀 Booting MAME (Injecting 0x${HEX_SIZE} bytes into 0100)..."

# Delay 1.5 seconds for PC-G850V boot screen, then enter MON and G0100
AUTO_CMD="N\n\n\nMON\nG0100\n"

mame pcg850v \
     -debug \
     -debugscript "$DEBUG_SCRIPT" \
     -skip_gameinfo \
     -video opengl -window \
     -resolution 800x400 \
     -autoboot_delay 2 \
     -autoboot_command "$AUTO_CMD"

echo "🏁 MAME runtime terminated."
