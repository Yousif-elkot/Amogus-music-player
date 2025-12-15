#!/bin/bash

echo "========================================"
echo "   Windows (WSL) Music Player Installer"
echo "========================================"

# 1. Update Package Lists (Critical for WSL)
echo "[Installer]: Updating package lists..."
sudo apt update -y

# 2. Install Dependencies (Using APT for Ubuntu/Debian)
# We need: build-essential (gcc), libncurses-dev, mpg123, mpv, python3-pip
echo "[Installer]: Installing dependencies..."
sudo apt install -y build-essential libncurses5-dev libncursesw5-dev mpg123 mpv python3-pip ffmpeg

# 3. Install yt-dlp (Often outdated in apt, better via pip or direct curl)
if ! command -v yt-dlp &>/dev/null; then
  echo "[Installer]: Installing yt-dlp..."
  sudo curl -L https://github.com/yt-dlp/yt-dlp/releases/latest/download/yt-dlp -o /usr/local/bin/yt-dlp
  sudo chmod a+rx /usr/local/bin/yt-dlp
fi

# 4. Compile the Code
echo "[Installer]: Compiling main.cpp..."
g++ main.cpp -o music_player -lncurses

if [ $? -eq 0 ]; then
  echo "========================================"
  echo "[Success]: Build complete!"
  echo "To play music from your Windows Music folder, use:"
  echo "./music_player \"/mnt/c/Users/YOUR_USER/Music\""
  echo "========================================"
else
  echo "[Error]: Compilation failed."
fi
