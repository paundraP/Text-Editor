#include <iostream>
#include <termios.h>
#include <unistd.h>
#include <cstdlib>
#include <fstream>
#include <algorithm>
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

// untuk save file
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
                disableRawMode();
                if (!isSaved) {
                    std::cout << "\nYour file is unsaved yet, do you want to save it? (y/n): ";
                    std::cin >> choice;
                    if (choice == 'y') {
                        saveFile(editor);
                        std::cout << "\nsuccessfuly saved file" << std::endl;
                    }
                }
                std::cout << "\nBye" << std::endl;
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
    
            // nge get text buat ngitung panjangnya di nempatin cursor
            std::string content = editor.getText();

            // buat nge clear screen
            system("clear");
            std::cout << "Input here:\n" << content << std::endl;

            std::cout << "\n[<] Undo | [>] Redo | [$] Save | [Backspace] Delete | [ESC] Exit\n";

            // set row dan column buat cursor
            int row = 2; // karena ini start dari dibawah input here, jadi row nya di 2
            int col = 1;

            size_t lastNewline = content.rfind('\n'); // tracking buat last line biar cursor tau kita lagi di line berapa
            if (lastNewline != std::string::npos) { // looping buat tracking ketikan kita sama cursor
                row += std::count(content.begin(), content.begin() + lastNewline + 1, '\n');
                col = content.length() - lastNewline;
            } else {
                col += content.length(); // make sure buat cursor inline sama text
            }

            // Move cursor to calculated position
            std::cout << "\033[" << row << ";" << col << "H";
            std::cout.flush();
        }
    }

    disableRawMode();
    return 0;
}
