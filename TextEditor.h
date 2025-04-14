#include "Action.h"
#include "Stack.h"

class CharNode {
public:
  char data;
  CharNode *next;

  CharNode(char c) : data(c), next(nullptr) {}
};

class TextEditor {
private:
  CharNode *head;
  CharNode *tail;
  int length = 0;
  Stack<Action> undoStack;
  Stack<Action> redoStack;

public:
  void insertChar(char c);
  void deleteChar();
  void undo();
  void redo();
  void printText();
};
