#include <iostream>
#include <termios.h>
#include <unistd.h>
#include <cstdlib>
#include <fstream>
#include "TextEditor.h"

// buat raw mode
void enableRawMode() {
    termios raw;
    tcgetattr(STDIN_FILENO, &raw);
    raw.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

// buat ngeclose
void disableRawMode() {
    termios normal;
    tcgetattr(STDIN_FILENO, &normal);
    normal.c_lflag |= (ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &normal);
}

int main() {
    TextEditor editor;
    enableRawMode();

    std::cout << "enter char to start typing" << std::endl;

    char c;
    while (read(STDIN_FILENO, &c, 1) == 1) {
        if (c == 27) {
            break;
        } else if (c == 127) {
            editor.deleteChar();
        } else if (c == '<') {
            editor.undo();
        } else if (c == '>') {
            editor.redo();
        } else if (c == '$') {
            std::ofstream out("output.txt");
            CharNode* curr = editor.getHead();
            while (curr) {
                out << curr->data;
                curr = curr->next;
            }
            out.close();
            std::cout << "file saved name output.txt" << std::endl;
            break;
        } else {
            editor.insertChar(c);
        }

        system("clear");
        std::cout << "Input here: \n";
        editor.printText();
        std::cout << "\n\n[<] Undo | [>] Redo | [$] Save | [Backspace] Delete | [ESC] Exit\n";
    }

    disableRawMode();
    return 0;
}
