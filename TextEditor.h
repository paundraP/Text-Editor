#include "Action.h"
#include "Stack.h"
#include <iostream>

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
  
  // helper func
  void insertAt(char c, int pos);
  void deleteAt(int pos);
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
  action.position = length;  // Posisi karakter yang dimasukkan
  
  undoStack.push(action);
  
  // Reset redo stack karena ada operasi baru
  while (!redoStack.isEmpty()) {
      redoStack.pop();
  }
  
  // Update panjang teks
  length++;
}

void TextEditor::deleteChar() {
  char targetChar; // Mendeklarasikan target karakter yang akan dihapus
    if (head == nullptr) { // Jika teks kosong maka tidak ada yang perlu dihapus
      return;
    }
    
    if (head == tail) { // Jika teks hanya terdiri dari satu karakter (head) maka hanya perlu menghapus node head dan meng-assign pointer head ke null.
      targetChar = head->data;
      delete head;
      head = tail = nullptr;
    }
    else { // Jika teks lebih dari satu karakter
      CharNode *tmp = head; // Menyimpan node sebelum tail
      while (tmp->next != tail) { // Traverse ke node sebelum node tail
        tmp = tmp->next;
      }
      targetChar = tail->data; // Menyimpan karakter yang ada pada tail
      delete tail; // Menghapus tail
      tail = tmp; // Mengassign pointer tail ke node sebelum tail
      tail->next = nullptr; // Meng-assign pointer ke node setelah tail baru menjadi null (bukan tail lama)
    }

    Action action;
    action.type = INSERT; // Untuk meng-undo penghapusan karakter, maka karakter di insert kembali ke dalam text editor
    action.character = targetChar;
    action.position = length - 1; // Posisi karakter pada text editor

    undoStack.push(action);

    while (!redoStack.isEmpty()) { // Menghapus riwayat redo text editor
      redoStack.pop();
    }

    length--; // Mengurangi jumlah karakter pada text editor sebanyak satu
}

void TextEditor::undo() {
  if (undoStack.isEmpty()) return;

  Action action = undoStack.pop();
  if(action.type == INSERT) {
    deleteAt(action.position);
    redoStack.push(Action(DELETE, action.character, action.position));
  } else if (action.type == DELETE) {
    insertAt(action.character, action.position);
    redoStack.push(Action(INSERT, action.character, action.position));
  }
}

void TextEditor::redo() {
  if (redoStack.isEmpty()) return;

  Action action = redoStack.pop();
  if (action.type == INSERT) {
    insertAt(action.character, action.position);
    undoStack.push(action);
  } else if (action.type == DELETE) {
    deleteAt(action.position);
    undoStack.push(action);
  }
}

void TextEditor::insertAt(char c, int pos) {
  if (pos < 0 || pos > length) return;

  CharNode *newnode = new CharNode(c);

  if (pos == 0) {
    newnode->next = head;
    head = newnode;
    if (length == 0) tail = newnode;
} else {
    CharNode* prev = head;
    for (int i = 0; i < pos - 1; i++) {
        prev = prev->next;
    }
    newnode->next = prev->next;
    prev->next = newnode;
    if (newnode->next == nullptr) tail = newnode;
}

length++;
}

void TextEditor::deleteAt(int pos) {
  if (!head || pos < 0 || pos >= length) return;

  CharNode *toDelete = nullptr;

  if (pos == 0) {
    toDelete = head;
    head = head->next;
    if(!head) tail = nullptr;
  } else {
    CharNode *prev = head;
    for (int i = 0; i < pos - 1; i++) {
      prev = prev->next;
  }

  toDelete = prev->next;
  prev->next = toDelete->next;
  if (prev->next == nullptr) {
      tail = prev; // kalo yang dihapus itu yang paling belakang, update tail nya
  }
  }
  delete toDelete;
  length--;
}