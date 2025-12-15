#ifndef FILELOADER_H
#define FILELOADER_H

#include "Playlist.h"
#include <filesystem> // C++17 Standard
#include <iostream>
#include <string>
#include <vector>

namespace fs = std::filesystem;
using namespace std;

class FileLoader {
public:
  // This function scans a folder and fills your playlist
  void loadFromDirectory(string folderPath, Playlist &playlist) {

    // 1. Check if the folder actually exists
    if (!fs::exists(folderPath) || !fs::is_directory(folderPath)) {
      cout << "[Error]: Directory not found: " << folderPath << endl;
      return;
    }

    cout << "[System]: Scanning " << folderPath << " for music..." << endl;
    int count = 0;

    // 2. Iterate through every file in the folder
    for (const auto &entry : fs::directory_iterator(folderPath)) {

      // 3. Filter: We only want .mp3 or .wav files
      if (entry.path().extension() == ".mp3" ||
          entry.path().extension() == ".wav") {

        string fullPath = entry.path().string();
        string fileName =
            entry.path().stem().string(); // "Song Name" (without .mp3)

        // Simple parsing: We don't have ID3 tags, so we use "Unknown Artist"
        // or you can try to split the string if your files are named "Artist -
        // Song"
        playlist.addSong(fileName, "Unknown Artist", fullPath);
        count++;
      }
    }

    if (count == 0) {
      cout << "[Warning]: No music files found in that folder!" << endl;
    } else {
      cout << "[Success]: Loaded " << count << " songs." << endl;
    }
  }
};

#endif
