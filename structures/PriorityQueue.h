#pragma once
#include <iostream>
using namespace std;

const int PQ_MAX = 300;

struct PQNode {
    int vertex;
    int distance;
    PQNode() : vertex(-1), distance(999999) {}
    PQNode(int v, int d) : vertex(v), distance(d) {}
};

class PriorityQueue {
private:
    PQNode heap[PQ_MAX];
    int size;

    void swap(int i, int j) { PQNode t = heap[i]; heap[i] = heap[j]; heap[j] = t; }

    void heapifyUp(int i) {
        while (i > 0) {
            int p = (i - 1) / 2;
            if (heap[p].distance > heap[i].distance) { swap(p, i); i = p; }
            else break;
        }
    }

    void heapifyDown(int i) {
        while (true) {
            int s = i, l = 2*i+1, r = 2*i+2;
            if (l < size && heap[l].distance < heap[s].distance) s = l;
            if (r < size && heap[r].distance < heap[s].distance) s = r;
            if (s != i) { swap(s, i); i = s; }
            else break;
        }
    }

public:
    PriorityQueue() : size(0) {}

    bool push(int vertex, int distance) {
        if (size >= PQ_MAX) { cout << "[PQ] Full!\n"; return false; }
        heap[size] = PQNode(vertex, distance);
        heapifyUp(size++);
        return true;
    }

    PQNode pop() {
        if (isEmpty()) 
        return PQNode();
        PQNode minNode = heap[0];
        heap[0] = heap[--size];
        if (size > 0) 
        heapifyDown(0);
        return minNode;
    }

    PQNode peek() const { 
        return isEmpty() ? PQNode() : heap[0]; 
    }
    bool isEmpty() const { 
        return size == 0; 
    }
    int getSize() const { 
        return size; 
    }
    void clear() { 
        size = 0; 
    }
};
