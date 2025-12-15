#!/bin/bash

echo "========================================"
echo "   Arch Linux Music Player Installer"
echo "========================================"

# 1. Check for required dependencies
DEPENDENCIES="ncurses mpg123 mpv yt-dlp gcc"
MISSING=""

for dep in $DEPENDENCIES; do
  if ! command -v $dep &>/dev/null; then
    MISSING="$MISSING $dep"
  fi
done

# 2. Install missing dependencies
if [ ! -z "$MISSING" ]; then
  echo "[Installer]: Missing dependencies detected: $MISSING"
  echo "[Installer]: Installing via Pacman (needs sudo)..."
  sudo pacman -S --noconfirm $MISSING
else
  echo "[Installer]: All dependencies met."
fi

# 3. Compile the C++ Code
echo "[Installer]: Compiling main.cpp..."
g++ main.cpp -o music_player -lncurses

if [ $? -eq 0 ]; then
  echo "[Success]: Build complete!"
  echo "----------------------------------------"
  echo "Run the player using:"
  echo "  ./music_player"
  echo "OR specify a folder:"
  echo "  ./music_player /path/to/songs"
else
  echo "[Error]: Compilation failed."
fi
