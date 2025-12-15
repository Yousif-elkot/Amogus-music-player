#ifndef SONG_H
#define SONG_H
#include <cstddef>
#include <string>

using namespace std;

struct Song {
  string title;
  string artist;
  string path;
  bool isStream; // this will tell the player are using a local file or not

  // pointrs for linked list
  Song *next;
  Song *prev;

  Song(string t, string a, string p, bool stream = false)
      : title(t), artist(a), path(p), isStream(stream), next(nullptr),
        prev(nullptr) {}
};

#endif
