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
void TextEditor::insertChar(char c) {
  // Buat node baru
  CharNode* newNode = new CharNode(c);
  
  // Jika teks masih kosong
  if (head == nullptr) {
      head = tail = newNode;
  } else {
      // Sisipkan di akhir (tail)
      tail->next = newNode;
      tail = newNode;
  }
  
  // Tambahkan action ke undo stack
  Action action;
  action.type = DELETE;  // Untuk undo insert adalah delete
  action.character = c;
  action.posisiton = length;  // Posisi karakter yang dimasukkan
  
  undoStack.push(action);
  
  // Reset redo stack karena ada operasi baru
  while (!redoStack.isEmpty()) {
      redoStack.pop();
  }
  
  // Update panjang teks
  length++;
}
