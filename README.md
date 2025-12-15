# 🎵 Terminal Music Player (DSA Project)

> A lightweight, text-based music player for Linux and Windows (WSL), built from scratch using C++ to demonstrate core Data Structures and Algorithms.

![Language](https://img.shields.io/badge/Language-C++17-00599C?style=flat-square&logo=c%2B%2B)
![Platform](https://img.shields.io/badge/Platform-Linux%20%7C%20WSL-orange?style=flat-square&logo=linux)
![Library](https://img.shields.io/badge/Library-ncurses-green?style=flat-square)

## 📖 About The Project

This project was developed as a practical application of **Data Structures and Algorithms (DSA)** for my Computer Science coursework at Suez Canal University.

Unlike standard music players that rely on high-level media frameworks for queue management, this player implements its own internal logic for managing song order, history, and playback using custom data structures. It features a full **TUI (Terminal User Interface)**, local file playback, and integrated **YouTube streaming** without leaving the command line.

---

## 🧠 Data Structures Implemented

The core functionality relies on two custom implementations found in the source code:

### 1. Doubly Linked List (`Playlist.h`)
* **Purpose:** Manages the playback queue.
* **Why:** Allows for efficient `O(1)` traversal to the **Next** and **Previous** songs. It also supports a **Circular Mode** (Looping) by dynamically connecting the tail node back to the head.

### 2. Stack (`HistoryStack.h`)
* **Purpose:** Tracks user navigation history.
* **Why:** Implements an "Undo" feature. Every time you jump to a song (via Search or Skip), the previous location is pushed onto the stack. Pressing `[UP]` pops the stack, returning you to exactly where you were.

---

## ✨ Features

* **Local Playback:** Supports `.mp3` and `.wav` files loaded from any directory.
* **Global Streaming:** Integrated `yt-dlp` and `mpv` to stream audio from YouTube directly by typing a song name.
* **Smart Search:** Fuzzy search implementation to find songs in your library instantly.
* **History Undo:** Navigate back through your listening history using the Stack.
* **Loop Mode:** Toggle between linear playback and infinite loop.
* **Cross-Platform:** Runs natively on Linux (Arch/Debian) and on Windows via WSL.

---

## 🛠️ Installation Guide

### Option 1: Linux (Arch / Debian / Ubuntu)

**Prerequisites:**
The installer script automatically handles dependencies (`g++`, `ncurses`, `mpv`, `yt-dlp`).

1.  **Clone the repository:**
    ```bash
    git clone [https://github.com/Yousif-elkot/Terminal-Music-Player.git](https://github.com/Yousif-elkot/Terminal-Music-Player.git)
    cd Terminal-Music-Player
    ```

2.  **Run the Installer:**
    ```bash
    chmod +x install.sh
    ./install.sh
    ```

3.  **Run the Player:**
    ```bash
    ./music_player
    # OR specify a folder directly:
    ./music_player /home/user/Music
    ```

---

### Option 2: Windows (via WSL)

Since this project uses Linux-specific libraries (`ncurses`), Windows users must use **WSL (Windows Subsystem for Linux)**.

#### Step 0: Install WSL (If you don't have it)
If you do not have WSL installed, follow these steps:
1.  Open **PowerShell** as Administrator.
2.  Run the command:
    ```powershell
    wsl --install
    ```
3.  **Restart your computer.**
4.  Open the newly installed "Ubuntu" app from your start menu and create a username/password.

#### Step 1: Install & Run
1.  Open your project folder in the terminal (ensure it is a WSL terminal, usually named "Ubuntu").
2.  Run the WSL-specific installer (handles `apt` dependencies):
    ```bash
    chmod +x install_wsl.sh
    ./install_wsl.sh
    ```

#### Step 2: Playing Windows Music
WSL accesses your Windows files via the `/mnt/c/` path.
* If your music is in `C:\Users\Yousif\Music`...
* You must type: `/mnt/c/Users/Yousif/Music`

```bash
./music_player "/mnt/c/Users/Yousif/Music"

Key,Action,Data Structure Operation
RIGHT,Play Next Song,current = current->next
LEFT,Play Previous Song,current = current->prev
UP,Undo / Go Back,stack.pop()
L,Toggle Loop,Connects Tail -> Head
S,Search Library,Linear Search Traversal
G,Global Stream,Fetches URL & Appends Node
Q,Quit,Proper Memory Cleanup
