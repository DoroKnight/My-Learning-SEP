#include <iostream>

#include "board.h"
#include "queue.h"

Board::Board(const int num_disk) : num_disk(num_disk), rods{Rod(num_disk, 1), Rod(num_disk, 2), Rod(num_disk, 3)} {
    // TODO
    disks = new Disk[num_disk];
    for (int i = 0; i < num_disk; i++) {
        disks[i].val = 2 * (i + 1) + 1;
        disks[i].id = 1;
    }
    for (int i = num_disk - 1; i >= 0; i--) {
        rods[0].push(disks[i]);
    }
}

Board::~Board() {
    // TODO
    delete []disks;
}

void Board::draw() {
    Canvas canvas {};
    canvas.reset();
    // TODO

    // 先初始化底座
    for (int i = 0; i < Canvas::WIDTH; i++) {
        canvas.buffer[Canvas::HEIGHT - 1][i] = '-';
    }

    // 再画 rod
    for (int i = 0; i < ROD_SIZE; i++) {
        rods[i].draw(canvas);
    }
    
    canvas.draw();
}

void Board::move(int from, int to, const bool log) {
    // TODO
    if (from > 3 || from < 1 || to > 3 || to < 1) return;
    if (from == to) return;

    if (rods[from - 1].empty()) return;
    
    if (!rods[to - 1].empty()) {
        if (rods[from - 1].top().val > rods[to - 1].top().val) return;
    } 

    Disk tmp = rods[from - 1].top();
    rods[from - 1].pop();
    tmp.id = to;
    rods[to - 1].push(tmp);
    if (log) {
        history.push(std::pair(from, to));
    }
}

bool Board::win() const {
    // TODO
    if (rods[0].empty() && rods[1].full() && rods[2].empty()) {
        return true;
    } else return false;
}

void solve(
    const int n,
    const int src,
    const int buf,
    const int dest,
    Queue<std::pair<int, int>> &solution
) {
    // TODO
    if (n == 1) {
        solution.push(std::pair(src, dest));
    } else {
        solve(n - 1, src, dest, buf, solution);       
        solution.push(std::pair(src, dest));          
        solve(n - 1, buf, src, dest, solution);       
    }
}

void Board::autoplay() {
    // TODO
    while (!history.empty()) {
        if (!history.empty()) {
            std::pair Pair = history.top();
            history.pop();
            this->move(Pair.second, Pair.first, false);
            std::cout << "Auto moving:" << Pair.second << "->" << Pair.first << std::endl;
            this->draw();
        }
    }
    Queue<std::pair<int, int>> solution;
    solve(num_disk, 1, 3, 2, solution);
    while (!solution.empty()) {
        std::pair<int, int> Pair = solution.front();
        solution.pop();
        this->move(Pair.first, Pair.second, false);
        std::cout << "Auto moving:" << Pair.first << "->" << Pair.second << std::endl;
        this->draw();
    }
}
