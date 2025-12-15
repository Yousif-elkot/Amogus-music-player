#ifndef HISTORYSTACK_H
#define HISTORYSTACK_H

#include "Song.h"
#include <iostream> // Added for NULL/nullptr if needed

using namespace std;

struct StackNode {
  Song *songRef;
  StackNode *down;
};

class HistoryStack {
private:
  StackNode *top;

public:
  HistoryStack() { top = nullptr; }

  ~HistoryStack() {
    while (!isEmpty())
      pop();
  }

  void push(Song *s) {
    StackNode *n = new StackNode;
    n->songRef = s;

    // If top is nullptr, n->down becomes nullptr automatically.
    n->down = top;
    top = n;
  }

  Song *pop() {
    if (isEmpty()) {
      return nullptr;
    }

    StackNode *curr = top;
    Song *s = curr->songRef; // Save the song pointer to return later

    top = top->down; // Move head down

    delete curr; // Delete the wrapper node (but NOT the song!)
    return s;
  }

  bool isEmpty() { return top == nullptr; }
};

#endif
