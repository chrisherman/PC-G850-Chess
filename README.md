# PC-G850V Dev Environment: Over-Engineering a 40-Year-Old Calculator

Welcome to your modern CI/CD pipeline for a Z80 pocket computer. Because why just compile code when you can ship it across a network to a containerized microservice, build it, ship it back, and launch a virtualized emulator, all to see a text prompt on a 144x32 pixel screen? 

Here is exactly how to set up this architectural marvel, step by step.

## 1. Setting Up the Remote z88dk Docker
You're running this on `homedocker`. The official `z88dk/z88dk` image is built on Alpine Linux (meaning `apt-get` is a myth here, use `apk`).

Create a directory for your environment on the host server:
```bash
mkdir -p ~/z88dk/workspace
cd ~/z88dk
```

Create this **`Dockerfile`**. It pulls the latest z88dk nightly, installs an SSH server so you can tunnel in, and fixes the paths so the compiler actually knows where it lives.

```dockerfile
FROM z88dk/z88dk:latest

# Install SSH server and dev utilities using Alpine's package manager
RUN apk add --no-cache \
    openssh \
    sudo \
    git \
    make \
    vim \
    tmux \
    curl \
    bash

# Configure SSH daemon settings
RUN ssh-keygen -A && \
    mkdir -p /var/run/sshd

# Create user 'dev' (UID 1000) and grant passwordless sudo permissions
RUN adduser -D -u 1000 -s /bin/bash dev && \
    echo "dev:g850v" | chpasswd && \
    echo "dev ALL=(ALL) ALL" >> /etc/sudoers

# Correct Z88DK paths pointing to /opt/z88dk
ENV PATH="/opt/z88dk/bin:${PATH}"
ENV ZCCCFG="/opt/z88dk/lib/config"

RUN echo 'export PATH="/opt/z88dk/bin:${PATH}"' >> /home/dev/.bashrc && \
    echo 'export ZCCCFG="/opt/z88dk/lib/config"' >> /home/dev/.bashrc

WORKDIR /home/dev/workspace
RUN chown -R dev:dev /home/dev

EXPOSE 22

CMD ["/usr/sbin/sshd", "-D"]
```

Create this **`docker-compose.yml`** right next to it. No version header, because Docker will yell at you about it being obsolete.

```yaml
services:
  z88dk-dev:
    build: .
    container_name: z88dk_g850v_env
    ports:
      - "2222:22"
    volumes:
      - ./workspace:/home/dev/workspace
    restart: unless-stopped
```

Boot it up:
```bash
docker compose up -d --build
```

---

## 2. Manually Compiling & Downloading

If you want to feel the raw power of typing long commands, you can invoke the cross-compiler from your local machine.

**Step A: Push your code to the server**
```bash
scp your_code.c chrisherman@homedocker.local:~/z88dk/workspace/
```

**Step B: Invoke the compiler remotely**
This tells Docker to execute `zcc` inside the running container. The `-create-app` flag is what forces z88dk to vomit out the `.ihx` and `.rom` files you actually need.
```bash
ssh chrisherman@homedocker.local "docker exec -w /home/dev/workspace z88dk_g850v_env zcc +g800 -clib=g850 -O3 --opt-code-size -create-app -o your_code.bin your_code.c"
```

**Step C: Retrieve the spoils**
```bash
scp chrisherman@homedocker.local:~/z88dk/workspace/your_code.ihx .
scp chrisherman@homedocker.local:~/z88dk/workspace/your_code.rom .
```

---

## 3. Launching MAME

Because watching emulator boot screens is for peasants, here is the command to launch MAME, bypass the warnings, load the Intel HEX file over the virtual serial port, and auto-type the monitor sequence.

```bash
mame pcg850v -serial your_code.ihx \
             -skip_gameinfo \
             -video opengl -window \
             -resolution 800x400 \
             -autoboot_delay 3 \
             -autoboot_command "N\n\nMON\nR\n"
```

---

## 4. Configuring and Using `loop.sh`

Doing the above manually contradicts the entire point of computers. Here is `loop.sh` (previously known as the `automame` script). 

**What it does:**
1. Looks for a `.c` file in the folder you ran it from (or takes one as an argument).
2. Uploads it to `homedocker`.
3. Compiles it.
4. Downloads the `.ihx` file *directly into the folder where your source code lives*.
5. Fires up MAME.

Create **`loop.sh`**, paste the full script below, and run `chmod +x loop.sh`.

```bash
#!/bin/bash
set -e

# --- CONFIGURATION ---
# Change these if your network or container naming deviates.
REMOTE_USER="chrisherman"
REMOTE_HOST="homedocker.local"
REMOTE_DIR="~/z88dk/workspace"
CONTAINER_NAME="z88dk_g850v_env"
# ---------------------

# Get the directory where this script lives
SCRIPT_DIR="$(dirname "$0")"

if [ -z "$1" ]; then
    # Enable nullglob so the array is empty if no .c files exist
    shopt -s nullglob
    C_FILES=("$SCRIPT_DIR"/*.c)
    shopt -u nullglob

    if [ ${#C_FILES[@]} -eq 0 ]; then
        echo "❌ Error: No C file provided and none found in $SCRIPT_DIR."
        echo "Usage: ./loop.sh [program.c]"
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

echo "🚀 Booting MAME with '$IHX_FILE'..."

# Auto-boot sequence into the PC-G850V monitor mode
AUTO_CMD="N\n\nMON\nR\n"

mame pcg850v -serial "$IHX_FILE" \
             -skip_gameinfo \
             -video opengl -window \
             -resolution 800x400 \
             -autoboot_delay 3 \
             -autoboot_command "$AUTO_CMD"

echo "🏁 MAME runtime terminated."
```

**Usage:**
Run it from anywhere on your machine.
```bash
./loop.sh path/to/your/chess850.c
```
Or, if you are already in the directory with the C file, just invoke it naked:
```bash
./loop.sh
```
