#pragma once
#include <iostream>
using namespace std;

const int QUEUE_MAX = 300;

class Queue {
private:
    int data[QUEUE_MAX];
    int frontIdx, rearIdx, count;

public:
    Queue() : frontIdx(0), rearIdx(-1), count(0) {}

    bool enqueue(int val) {
        if (count >= QUEUE_MAX) { cout << "[Queue] Full!\n"; return false; }
        rearIdx = (rearIdx + 1) % QUEUE_MAX;
        data[rearIdx] = val;
        count++;
        return true;
    }

    int dequeue() {
        if (isEmpty()) {
             cout << "[Queue] Empty!\n"; 
             return -1; }
        int val = data[frontIdx];
        frontIdx = (frontIdx + 1) % QUEUE_MAX;
        count--;
        return val;
    }

    int front() const { 
        return isEmpty() ? -1 : data[frontIdx]; 
    }
    bool isEmpty() const { 
        return count == 0; 
    }
    int getSize() const { 
        return count; 
    }
    void clear() { 
        frontIdx = 0; rearIdx = -1; count = 0; 
    }
};
