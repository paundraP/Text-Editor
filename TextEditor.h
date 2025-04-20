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
  ~TextEditor() {
    CharNode *curr = head;
    while(curr) {
      CharNode *next = curr->next;
      delete curr;
      curr = next;
    }
  }
  void insertChar(char c, int pos);
  void deleteChar(int pos);
  void undo();
  void redo();
  void printText();
  
  // helper func
  CharNode *getHead();
  int getLength();
  std::string getText();
};

void TextEditor::insertChar(char c, int pos) {
  if (pos < 0 || pos >= length) {
    return;
  }
  // Buat node baru
  CharNode* newNode = new CharNode(c);
  
  // Jika teks masih kosong
  if (pos == 0) {
    newNode->next = head; 
    head = newNode;
    if (length == 0) {
      tail = newNode;
    }
  } else {
      CharNode *tmp = head;
      for (int i = 0; i < pos - 1; i++) {
        tmp = tmp->next;
      }
      newNode->next = tmp->next;
      tmp->next = newNode;
      if (newNode->next == nullptr) {
        tail = newNode;
      }
  }
  
  // Tambahkan action ke undo stack
  Action action;
  action.type = DELETE;  // Untuk undo insert adalah delete
  action.character = c;
  action.position = pos;  // Posisi karakter yang dimasukkan
  
  undoStack.push(action);
  
  // Reset redo stack karena ada operasi baru
  while (!redoStack.isEmpty()) {
      redoStack.pop();
  }
  
  // Update panjang teks
  length++;
}

void TextEditor::deleteChar(int pos) {
  CharNode* toDelete; // Mendekalarasikan node yang akan dihapus
  char targetChar; // Mendeklarasikan target karakter yang akan dihapus
    if (head == nullptr || pos < 0 || pos >= length) { // Jika teks kosong maka tidak ada yang perlu dihapus
      return;
    }
    
    if (pos == 0) { // Jika teks hanya terdiri dari satu karakter (head) maka hanya perlu menghapus node head dan meng-assign pointer head ke null.
      toDelete = head;
      targetChar = toDelete->data;
      head = head->next;
      if (head == nullptr) {
        tail = nullptr;
      }
    }
    else { // Jika teks lebih dari satu karakter
      CharNode *tmp = head; // Menyimpan node sebelum tail
      for (int i = 0; i < pos - 1; i++) {
        tmp = tmp->next;
      }
      toDelete = tmp->next;
      targetChar = toDelete->data; // Menyimpan karakter yang ada pada tail
      tmp->next = toDelete->next;
      if (toDelete == tail) {
        tail = tmp;
      }
    }
    delete toDelete; // Menghapus char
    
    Action action;
    action.type = INSERT; // Untuk meng-undo penghapusan karakter, maka karakter di insert kembali ke dalam text editor
    action.character = targetChar;
    action.position = pos; // Posisi karakter pada text editor

    undoStack.push(action);

    while (!redoStack.isEmpty()) { // Menghapus riwayat redo text editor
      redoStack.pop();
    }

    length--; // Mengurangi jumlah karakter pada text editor sebanyak satu
}

void TextEditor::undo() {
  // cek kalau belum ada character yang masuk di undo stack langsung return atau tidak melakukan operasi apapun
  if (undoStack.isEmpty()) return;

  // kalau ada, pop Action yang ada di undo stack dan simpan sementara di variabel action
  Action action = undoStack.pop();

  // cek kalau actionnya itu insert, maka dimasukin lagi ke editor
  if(action.type == INSERT) {
    insertChar(action.character, action.position);
    redoStack.push(Action(DELETE, action.character, action.position));
  } else if (action.type == DELETE) { // kalau type nya delete, jadi dihapus lagi characternya dari editor
    deleteChar(action.position);
    redoStack.push(Action(INSERT, action.character, action.position));
  }
}

void TextEditor::redo() {
  if (redoStack.isEmpty()) return;

  // pop Action di redo stack dan disimpan sementara di variabel action
  Action action = redoStack.pop();

  // cek kalau waktu dimasukin pertama kali itu action type nya delete, maka dia bakal di delete di editor
  if (action.type == DELETE) {
    deleteChar(action.position);
    undoStack.push(action);
  } else if (action.type == INSERT) { // kebalikan dari delete
    insertChar(action.character, action.position);
    undoStack.push(action);
  }
}

void TextEditor::printText() {
  CharNode* current = head;
  while (current) {
      std::cout << current->data;
      current = current->next;
  }
  std::cout << std::endl;
}

CharNode *TextEditor::getHead() {
  return head;
}

int TextEditor::getLength() {
  return length;
}

std::string TextEditor::getText() {
  std::string text;
  CharNode* curr = head;
  while (curr != nullptr) {
      text += curr->data;
      curr = curr->next;
  }
  return text;
}