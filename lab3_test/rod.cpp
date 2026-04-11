#include "rod.h"
#include <cstddef>

Rod::Rod(const int capacity, const int id) : capacity(capacity), id(id) {};

bool Rod::push(const Disk d) {
    // TODO
    stack.push(d);
    capacity--;
    return true;
}

const Disk &Rod::top() {
    // TODO
    return stack.top();
}

void Rod::pop() {
    // TODO
    stack.pop();
    capacity++;
}

size_t Rod::size() const {
    // TODO
    return stack.size();
}

bool Rod::empty() const {
    // TODO
    return stack.empty();
}
bool Rod::full() const {
    // TODO
    return capacity == 0;
}
void Rod::draw(Canvas &canvas) {
    // TODO
    Stack<const Disk> tmp;
    // 先画杆子
    int order[3] = {5, 20, 35};
    for (int i = 0; i < Canvas::HEIGHT; i++) {
        canvas.buffer[i][order[id - 1]] = '|';
    }
    
    // 取出圆盘
    while (!stack.empty()) {
        int level = stack.size();
        const Disk disk = stack.top();
        stack.pop();
        tmp.push(disk);
        disk.draw(canvas, level - 1, id - 1);
    }

    // 归位
    while (!tmp.empty()) {
        const Disk disk = tmp.top();
        tmp.pop();
        stack.push(disk);
    }
}
