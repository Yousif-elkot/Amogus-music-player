#include "FileLoader.h"
#include "HistoryStack.h"
#include "Playlist.h"
#include <cstdlib>
#include <iostream> // For cin/cout
#include <ncurses.h>
#include <string>

using namespace std;

// --- AUDIO ENGINE ---
void playMusic(Song *song) {
  if (song == nullptr)
    return;

  // Stop any currently playing music
  system("pkill mpg123 > /dev/null 2>&1");
  system("pkill mpv > /dev/null 2>&1");

  string command;

  if (song->isStream) {
    // --- GLOBAL MODE (Integrated & SILENCED) ---
    // This prevents yt-dlp warnings from leaking onto your TUI screen.
    string query = song->path;
    command = "bash -c 'URL=$(yt-dlp -f bestaudio -g \"ytsearch1:" + query +
              "\" 2>/dev/null); if [ ! -z \"$URL\" ]; then nohup mpv "
              "--no-video \"$URL\" > "
              "/dev/null 2>&1 & fi'";
  } else {
    // --- LOCAL MODE ---
    command =
        "mpg123 -q --resync-limit -1 \"" + song->path + "\" > /dev/null 2>&1 &";
  }

  system(command.c_str());
}

void stopMusic() {
  system("pkill mpg123 > /dev/null 2>&1");
  system("pkill mpv > /dev/null 2>&1");
}

// --- TUI FUNCTIONS ---
void drawTitle(WINDOW *win) {
  wattron(win, COLOR_PAIR(3) | A_BOLD);
  mvwprintw(win, 1, 2, " === 🎵 ARCH LINUX MUSIC PLAYER (DSA PROJECT) 🎵 === ");
  wattroff(win, COLOR_PAIR(3) | A_BOLD);
  box(win, 0, 0);
  wrefresh(win);
}

void drawPlaylist(WINDOW *win, Playlist &pl) {
  werase(win);
  box(win, 0, 0);
  mvwprintw(win, 0, 2, " [ Playlist Library ] ");

  Song *temp = pl.getHead();
  int y = 2;
  int maxRows = getmaxy(win) - 2;

  if (!temp) {
    mvwprintw(win, 2, 2, "No songs loaded!");
    wrefresh(win);
    return;
  }

  do {
    if (y >= maxRows)
      break;
    if (temp == pl.current) {
      wattron(win, COLOR_PAIR(1) | A_BOLD); // Green
      mvwprintw(win, y, 2, "-> %s", temp->title.c_str());
      wattroff(win, COLOR_PAIR(1) | A_BOLD);
    } else {
      mvwprintw(win, y, 2, "   %s", temp->title.c_str());
    }
    temp = temp->next;
    y++;
  } while (temp != nullptr && temp != pl.getHead());

  wrefresh(win);
}

void drawControls(WINDOW *win, Playlist &pl, string statusMsg) {
  werase(win);
  box(win, 0, 0);
  int winH, winW;
  getmaxyx(win, winH, winW);

  mvwprintw(win, 0, 2, " [ Controls ] ");

  if (pl.current) {
    wattron(win, COLOR_PAIR(2) | A_BOLD);
    mvwprintw(win, 1, 2, "Now Playing: %s", pl.current->title.c_str());
    wattroff(win, COLOR_PAIR(2) | A_BOLD);
    mvwprintw(win, 2, 2, "Artist:      %s", pl.current->artist.c_str());
  } else {
    mvwprintw(win, 1, 2, "Now Playing: [Stopped]");
  }

  mvwprintw(win, 4, 2, "COMMANDS:");
  mvwprintw(win, 5, 4, "[RIGHT] Next       [LEFT] Prev");
  mvwprintw(win, 6, 4, "[UP]    Undo       [L]    Loop Mode");
  mvwprintw(win, 7, 4, "[S]     Search     [G]    Global Stream");

  if (winW > 20) {
    string label = "Status: " + statusMsg;
    int startX = winW - label.length() - 4;
    if (startX < 40)
      startX = 40;
    wattron(win, A_DIM);
    mvwprintw(win, 7, startX, "%s", label.c_str());
    wattroff(win, A_DIM);
  }
  wrefresh(win);
}

string getSearchQuery(int height, int width, string prompt) {
  echo();
  curs_set(1);
  int winH = 5, winW = 40;
  int startY = (height - winH) / 2;
  int startX = (width - winW) / 2;

  WINDOW *winInput = newwin(winH, winW, startY, startX);
  box(winInput, 0, 0);
  wattron(winInput, COLOR_PAIR(3) | A_BOLD);
  mvwprintw(winInput, 1, 2, " USER INPUT ");
  wattroff(winInput, COLOR_PAIR(3) | A_BOLD);
  mvwprintw(winInput, 2, 2, "%s", prompt.c_str());
  wrefresh(winInput);

  char inputStr[100];
  mvwgetnstr(winInput, 2, 2 + prompt.length(), inputStr, 99);

  noecho();
  curs_set(0);
  delwin(winInput);
  return string(inputStr);
}

int main(int argc, char *argv[]) {
  // --- FOLDER SELECTION LOGIC ---
  string musicPath;

  // 1. Check if user provided path as argument (e.g., ./player /home/music)
  if (argc > 1) {
    musicPath = argv[1];
  } else {
    // 2. If not, ask nicely before starting the GUI
    cout << "Enter the full path to your music directory: ";
    // Use getline to allow spaces in path
    getline(cin, musicPath);

    // Default fallback if they just hit enter
    if (musicPath.empty()) {
      musicPath = "/home/kot/Music/songs"; // Your default
      cout << "Using default path: " << musicPath << endl;
      system("sleep 1");
    }
  }

  Playlist myPlaylist;
  HistoryStack myHistory;
  FileLoader loader;

  loader.loadFromDirectory(musicPath, myPlaylist);

  // Auto-play first song if loaded
  if (myPlaylist.current)
    playMusic(myPlaylist.current);

  initscr();
  cbreak();
  noecho();
  keypad(stdscr, TRUE);
  curs_set(0);

  if (has_colors()) {
    start_color();
    init_pair(1, COLOR_GREEN, COLOR_BLACK);
    init_pair(2, COLOR_YELLOW, COLOR_BLACK);
    init_pair(3, COLOR_CYAN, COLOR_BLACK);
  }

  int height, width;
  getmaxyx(stdscr, height, width);
  WINDOW *winTitle = newwin(3, width, 0, 0);
  WINDOW *winList = newwin(height - 12, width, 3, 0);
  WINDOW *winCtrl = newwin(9, width, height - 9, 0);

  refresh();

  int ch;
  bool running = true;
  string status = "Ready.";

  while (running) {
    drawTitle(winTitle);
    drawPlaylist(winList, myPlaylist);
    drawControls(winCtrl, myPlaylist, status);

    ch = getch();

    switch (ch) {
    case KEY_RIGHT:
      if (myPlaylist.current)
        myHistory.push(myPlaylist.current);
      myPlaylist.playNext();
      playMusic(myPlaylist.current);
      status = "Skipped Forward";
      break;

    case KEY_LEFT:
      if (myPlaylist.current)
        myHistory.push(myPlaylist.current);
      myPlaylist.playPrevious();
      playMusic(myPlaylist.current);
      status = "Skipped Back";
      break;

    case KEY_UP:
      if (myHistory.isEmpty()) {
        status = "Error: History Empty!";
      } else {
        Song *s = myHistory.pop();
        if (s) {
          myPlaylist.current = s;
          playMusic(myPlaylist.current);
          status = "Undid Jump";
        }
      }
      break;

    case 'l':
    case 'L':
      myPlaylist.toggleLoopMode();
      status = "Loop Toggled";
      break;

    case 'q':
    case 'Q':
      running = false;
      break;

    case 's':
    case 'S': {
      string query = getSearchQuery(height, width, "Search: ");
      drawTitle(winTitle);
      drawPlaylist(winList, myPlaylist);

      if (query.empty()) {
        status = "Search cancelled.";
      } else {
        if (myPlaylist.current)
          myHistory.push(myPlaylist.current);

        Song *result = myPlaylist.searchSong(query);

        if (result != nullptr) {
          playMusic(myPlaylist.current);
          status = "Found: " + query;
        } else {
          myHistory.pop();
          status = "Song not found!";
        }
      }
      break;
    }

    case 'g':
    case 'G': {
      string webQuery = getSearchQuery(height, width, "Global: ");
      drawTitle(winTitle);
      drawPlaylist(winList, myPlaylist);

      if (!webQuery.empty()) {
        status = "Fetching: " + webQuery + "...";
        drawControls(winCtrl, myPlaylist, status);
        wrefresh(winCtrl); // Force update to show "Fetching"

        myPlaylist.addSong(webQuery, "World Wide Web", webQuery, true);

        // Jump to end of list
        Song *temp = myPlaylist.getHead();
        if (temp) {
          while (temp->next != nullptr && temp->next != myPlaylist.getHead()) {
            temp = temp->next;
          }
          myPlaylist.current = temp;
          playMusic(myPlaylist.current);
        }
      }
      break;
    }
    }
  }

  stopMusic();
  delwin(winTitle);
  delwin(winList);
  delwin(winCtrl);
  endwin();
  return 0;
}
