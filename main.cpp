#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>
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

int getTerminalRows() {
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    return w.ws_row;
}

int main() {
    TextEditor editor;
    enableRawMode();
    char c, cmd, choice;
    bool isSaved = false;

    enableRawMode();
    std::cout << "press [i] to start typing" << std::endl;
    read(STDIN_FILENO, &cmd, 1);
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
                        // ngambil rows
                        int rows = getTerminalRows();
                        std::cout << "\033[" << rows << ";1H"; // memindahkan ke paling bawah screen
                        std::cout << "Your file is unsaved. Save before exiting? (y/n): ";
                        std::cout.flush();
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
                editor.deleteChar(cursorPos);
                if (cursorPos > 0) cursorPos--;     
            } else if (c == 21) {
                editor.undo();
                cursorPos = editor.getLength();
            } else if (c == 18) {
                editor.redo();
                cursorPos = editor.getLength();
            } else if (c == '$') {
                saveFile(editor);
                isSaved = true;
            } else {
                editor.insertChar(c, cursorPos);
                cursorPos++;
                isSaved = false;
            }

            system("clear");

            // display character yang terecord di linked list
            std::string content = editor.getText();
            std::cout << content << std::endl;


            int terminalRows = getTerminalRows();

            // mindahin ke paling bawah dari display terminal window (like nano)
            std::cout << "\033[" << (terminalRows - 1) << ";1H";
            std::cout << "[^+U] Undo | [^+R] Redo | [$] Save | [Backspace] Delete | [ESC] Exit";

            // sama kaya atasnya, kalau ke save akan muncul tulisan tersave
            std::cout << "\033[" << terminalRows << ";1H";
            if (isSaved) {
                std::cout << "File saved as output.md";
            } else {
                std::cout << " "; // kalau user ngetik lagi akan ke clear.
            }


            // set cursor di setelah input here
            int row = 1;
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
