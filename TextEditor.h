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
  int length;
  Stack<Action> undoStack;
  Stack<Action> redoStack;

public:
  TextEditor() : head(nullptr), tail(nullptr), length(0) {}
  void insertChar(char c);
  void deleteChar();
  void undo();
  void redo();
  void printText();
};
