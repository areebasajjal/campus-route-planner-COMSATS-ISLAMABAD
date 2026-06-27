#pragma once
#include <iostream>
using namespace std;

const int STACK_MAX = 300;

class Stack {
private:
    int data[STACK_MAX];
    int topIdx;
public:
    Stack() : topIdx(-1) {}

    bool push(int val) {
        if (topIdx >= STACK_MAX - 1) { 
            cout << "[Stack] Overflow!\n";  
            return false; }
        data[++topIdx] = val;
        return true;
    }

    int pop() {
        if (isEmpty()) { 
            cout << "[Stack] Underflow!\n"; 
            return -1; }
        return data[topIdx--];
    }

    int peek() const { 
        return isEmpty() ? -1 : data[topIdx]; 
    }
    bool isEmpty() const { 
        return topIdx == -1; 
    }
    int getSize() const { 
        return topIdx + 1; 
    }
    void clear() { 
        topIdx = -1; 
    }
};
