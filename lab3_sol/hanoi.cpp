#include <climits>
#include <iostream>
#include <string>

#include "board.h"

using namespace std;

int main() {
    while (true) {
        cout << "How many disks do you want? (1 ~ 5)" << endl;
        string input;
        getline(cin, input);
        if (input == "Q") {
            break;
        }

        // TODO
        if (input.length() < 1) continue;

        string trueInput;
        char *front = &input[0], *back = &input[input.length()-1];
        int index = 0, length = 0;

        while (*front == ' ') {
            front++;
            index++;
        }
        while (*back == ' ') back--;

        trueInput = input.substr(index, back - front + 1);
        if (trueInput.length() > 1 || trueInput.length() < 1) continue;
        bool isLegal = true;
        switch(trueInput[0]) {
            case '1': case '2': case '3': case '4': case '5':
                break;
            default:
                isLegal = false;
        }
        if(!isLegal) continue;
        Board board(trueInput[0] - '0');
        board.draw();
        int x = 0, y = 1;
        bool isAuto = false;
        while (!board.win()) {
            cout << "Move a disk. Format: x y" << endl;
            cin >> x >> y;
            cin.clear();
            cin.ignore(1024, '\n');
            if (x == 0 && y == 0) {
                isAuto = true;
                break;
            } else if (x < 1 || x > 3 || y < 1 || y > 3) {
                board.draw();
                continue;
            } else {
                board.move(x, y, true);
                board.draw();
            }
        }
        if (isAuto) board.autoplay();
        cout << "Congratulations! You win!" << endl;
    }
    return 0;
}
