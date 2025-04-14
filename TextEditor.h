#include "Action.h"
#include "Stack.h"
#include <string>

class TextEditor {
private:
  std::string text;
  Stack<Action> undoStack;
  Stack<Action> redoStack;

public:
  void insertChar(char c);
  void deleteChar();
  void undo();
  void redo();
  void printText();
};
