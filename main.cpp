#include <iostream>
#include "TextEditor.h"
using namespace std;

int main() { 
    cout << "hello world" << endl; 
    TextEditor x;
    x.insertChar('S');
    x.insertChar('k');
    while (x->next != NULL);
    return 0;
}
