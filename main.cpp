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

void saveFile(TextEditor &editor) {
    std::ofstream out("output.md");
    CharNode* curr = editor.getHead();
    while (curr) {
        out << curr->data;
        curr = curr->next;
    }
    out.close();
}
int main() {
    TextEditor editor;
    enableRawMode();
    char c, cmd, choice;
    bool isSaved = false;

    std::cout << "press i to start typing" << std::endl;
    std::cin >> cmd;
    if (cmd == 'i') {
        while (read(STDIN_FILENO, &c, 1) == 1) {
            if (c == 27) {
                if (!isSaved) {
                    std::cout << "Your file is unsaved yet, do you want to save it? (y/n): ";
                    std::cin >> choice;
                    if (choice == 'y') {
                        saveFile(editor);
                        std::cout << "\nsuccessfuly saved file" << std::endl;
                    }
                }
                std::cout << "Bye" << std::endl;
                break;
            } else if (c == 127) {
                editor.deleteChar();
            } else if (c == '<') {
                editor.undo();
            } else if (c == '>') {
                editor.redo();
            } else if (c == '$') {
                saveFile(editor);
                std::cout << "file saved name output.md" << std::endl;
                isSaved = true;
            } else {
                editor.insertChar(c);
                isSaved = false;
            }
    
            system("clear");
            std::cout << "Input here: \n";
            editor.printText();
            std::cout << "\n\n[<] Undo | [>] Redo | [$] Save | [Backspace] Delete | [ESC] Exit\n";
        }
    }

    disableRawMode();
    return 0;
}
