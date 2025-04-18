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
    int cursorPos = 0; // tracking cursor

    if (cmd == 'i') {
        while (read(STDIN_FILENO, &c, 1) == 1) {
            if (c == 27) {
                char seq[2];
                if (read(STDIN_FILENO, &seq[0], 1) == 0) continue;
                if (read(STDIN_FILENO, &seq[1], 1) == 0) continue;

                if (seq[0] == '[') {
                    switch (seq[1]) {
                        case 'D': // left arrow
                            if (cursorPos > 0) cursorPos--;
                            break;
                        case 'C': // right arrow
                            if (cursorPos < editor.getText().length()) cursorPos++;
                            break;
                        case 'A': case 'B':
                            // kalau arrow nya atas/bawah skip aja
                            break;
                    }
                } else {
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
                }
            } else if (c == 127) {
                editor.deleteAt(cursorPos);
                if (cursorPos > 0) cursorPos--;
            } else if (c == '<') {
                editor.undo();
                cursorPos = editor.getLength(); // set cursor ke character terakhir
            } else if (c == '>') {
                editor.redo();
                cursorPos = editor.getLength();
            } else if (c == '$') {
                saveFile(editor);
                std::cout << "file saved as output.md" << std::endl;
                isSaved = true;
            } else {
                editor.insertAt(c, cursorPos);
                cursorPos++;
                isSaved = false;
            }

            std::string content = editor.getText();

            system("clear");
            std::cout << "Input here:\n" << content << std::endl;
            std::cout << "\n[<] Undo | [>] Redo | [$] Save | [Backspace] Delete | [ESC] Exit\n";

            // set cursor di setelah input here
            int row = 2;
            int col = 1;

            int newlines = 0;
            size_t lastNewline = content.rfind('\n', cursorPos - 1);
            if (lastNewline != std::string::npos) {
                newlines = std::count(content.begin(), content.begin() + cursorPos, '\n');
                row += newlines;
                col = cursorPos - lastNewline;
            } else {
                col += cursorPos;
            }

            std::cout << "\033[" << row << ";" << col << "H";
            std::cout.flush();
        }
    }

    disableRawMode();
    return 0;
}
