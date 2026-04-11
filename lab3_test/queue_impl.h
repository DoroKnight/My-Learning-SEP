#ifndef QUEUE_IMPL_H
#define QUEUE_IMPL_H
#include <cassert>
#include <cstddef>

template <typename T>
Queue<T>::Queue() : sz(0), head(nullptr), tail(nullptr) { }

template <typename T>
Queue<T>::~Queue() {
    // TODO
    Node<T> *tmp;
    while (head != nullptr) {
        tmp = head;
        head = head->next;
        delete tmp;
    }
    tail = head;
    sz = 0;
}

template <typename T>
void Queue<T>::push(T t) {
    // TODO
    if (head == nullptr && tail == nullptr) {
        head = new Node<T>(t);
        tail = head;
    } else {
        tail->next = new Node<T>(t);
        tail = tail->next;
    }
    sz++;
}


template <typename T>
void Queue<T>::pop() {
    // TODO
    assert(!this->empty());

    Node<T> *tmp = head;
    head = head->next;
    delete tmp;
    sz--;
}

template <typename T>
T &Queue<T>::front() {
    // TODO
    assert(!this->empty());
    return head->val;
}

template <typename T>
bool Queue<T>::empty() const {
    // TODO
    return sz == 0;
}

template <typename T>
size_t Queue<T>::size() const {
    // TODO
    return sz;
}
#endif
