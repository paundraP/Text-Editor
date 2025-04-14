#include "stack.h"
#include <iostream>
using namespace std;

int main() {
  Stack<int> st;
  st.push(1);
  st.push(4);
  cout << "the top of the stack is: " << st.peek() << endl;
  st.pop();
  cout << "now, the top of the stack is: " << st.peek() << endl;
  return 0;
}
