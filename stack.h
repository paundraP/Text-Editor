#ifndef STACK_H
#define STACK_H

#include <stdexcept>
template <typename T> class StackNode {
public:
  T data;
  StackNode *next;
  StackNode(T val) : data(val), next(nullptr) {}
};

template <typename T> class Stack {
private:
  StackNode<T> *top;

public:
  Stack() : top(nullptr) {}
  ~Stack() {
    while (!isEmpty()) {
      pop();
    }
  }
  void push(T value) {
    // ngebuat node baru
    StackNode<T> *node = new StackNode<T>(value);
    // link node ke top/data yang barusan ditambahin
    node->next = top;
    // ngejadiin data yang barusan ditambahin jadi top
    top = node;
  }
  T pop() {
    if (isEmpty()) {
      // use C++ exception: https://www.w3schools.com/cpp/cpp_exceptions.asp
      throw std::out_of_range("stack is empty");
    }

    // ngambil data paling atas / terbaru
    StackNode<T> *node = top;
    // nyimpen datanya di variable val
    T val = node->data;
    // ngeubah top nya jadi data dibawahnya node
    top = top->next;
    // ngehapus nodenya karena mau di pop
    delete node;
    // ngembaliin value yang abis di pop atau delete.
    return val;
  }
  T peek() {
    if (isEmpty()) {
      throw std::out_of_range("stack is empty");
    }
    return top->data;
  }
  bool isEmpty() { return top == nullptr; }
};

#endif
