#ifndef PLAYLIST_H
#define PLAYLIST_H

#include "Song.h"
#include <algorithm> // For transform()
#include <cctype>    // For tolower()
#include <string>

using namespace std;

class Playlist {
private:
  Song *head;
  Song *tail;
  bool isLooping;

  // --- HELPER FUNCTION (New) ---
  // Converts string to lowercase so "Golden" matches "golden"
  string toLowerCase(string str) {
    transform(str.begin(), str.end(), str.begin(),
              [](unsigned char c) { return tolower(c); });
    return str;
  }

public:
  Song *current;

  // Constructor implementation (Inline)
  Playlist()
      : head(nullptr), tail(nullptr), isLooping(false), current(nullptr) {}

  // ---------------------------------------------------------
  // CORE LINKED LIST OPERATIONS
  // ---------------------------------------------------------

  Song *getHead() { return head; }

  void addSong(string title, string artist, string path,
               bool isStream = false) {
    Song *newSong = new Song(title, artist, path, isStream);

    if (head == nullptr) {
      head = tail = current = newSong; // Set current if it's the first song!
    } else {
      tail->next = newSong;
      newSong->prev = tail;
      tail = newSong;
    }

    if (isLooping) {
      tail->next = head;
      head->prev = tail;
    }
  }

  void playNext() {
    if (current == nullptr) {
      if (head != nullptr)
        current = head; // Start playing if stopped
      return;
    }

    // Now safe to check next
    if (current->next == nullptr) {
      return;
    }

    current = current->next;
    // Visual feedback for debugging
  }

  void playPrevious() {
    if (current == nullptr) {
      return;
    }

    if (current->prev == nullptr) {
      return;
    }

    current = current->prev;
  }

  // --- Improved FUZZY SEARCH ---
  Song *searchSong(string title) {
    if (head == nullptr)
      return nullptr;

    // 1. convvert user input to lowercase
    string qlower = toLowerCase(title);

    Song *temp = head;
    do {
      // 2. conver current song title to lower
      string tlower = toLowerCase(temp->title);

      if (tlower.find(qlower) != string::npos) {
        current = temp; // Optional: Determine if searching should auto-play it
        return current;
      }
      temp = temp->next;

      // Stop if we hit null (Linear mode) OR if we arrive back at head
      // (Circular mode)
    } while (temp != nullptr && temp != head);

    return nullptr;
  }

  void toggleLoopMode() {
    if (head == nullptr || tail == nullptr)
      return;

    if (!isLooping) {
      // Activate Circle
      head->prev = tail;
      tail->next = head;
      isLooping = true;
    } else {
      // Break Circle
      head->prev = nullptr;
      tail->next = nullptr;
      isLooping = false;
    }
  }
};

#endif
