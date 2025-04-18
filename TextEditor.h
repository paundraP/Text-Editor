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
  void insertChar(char c);
  void deleteChar();
  void undo();
  void redo();
  void printText();
  
  // helper func
  void insertAt(char c, int pos);
  void deleteAt(int pos);
  CharNode *getHead();
  int getLength();
  std::string getText();
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
  // cek kalau belum ada character yang masuk di undo stack langsung return atau tidak melakukan operasi apapun
  if (undoStack.isEmpty()) return;

  // kalau ada, pop Action yang ada di undo stack dan simpan sementara di variabel action
  Action action = undoStack.pop();

  // cek kalau actionnya itu insert, maka dimasukin lagi ke editor
  if(action.type == INSERT) {
    insertAt(action.character, action.position);
    redoStack.push(Action(DELETE, action.character, action.position));
  } else if (action.type == DELETE) { // kalau type nya delete, jadi dihapus lagi characternya dari editor
    deleteAt(action.position);
    redoStack.push(Action(INSERT, action.character, action.position));
  }
}

void TextEditor::redo() {
  if (redoStack.isEmpty()) return;

  // pop Action di redo stack dan disimpan sementara di variabel action
  Action action = redoStack.pop();

  // cek kalau waktu dimasukin pertama kali itu action type nya delete, maka dia bakal di delete di editor
  if (action.type == DELETE) {
    deleteAt(action.position);
    undoStack.push(action);
  } else if (action.type == INSERT) { // kebalikan dari delete
    insertAt(action.character, action.position);
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

// helper function buat masukin character di spesific position, bisa di tengah tengah
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

CharNode *TextEditor::getHead() {
  return head;
}

int TextEditor::getLength() {
  return length;
}

std::string TextEditor::getText() {
  std::string text = "";
  CharNode* curr = head;
  while (curr) {
      text += curr->data;
      curr = curr->next;
  }
  return text;
}
