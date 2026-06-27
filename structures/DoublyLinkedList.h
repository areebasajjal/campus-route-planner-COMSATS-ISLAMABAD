#pragma once
#include <iostream>
using namespace std;

template<typename T>
struct DLLNode {
    T data;
    DLLNode<T>* prev;
    DLLNode<T>* next;
    DLLNode(T data) : data(data), prev(nullptr), next(nullptr) {}
};

template<typename T>
class DoublyLinkedList {
private:
    DLLNode<T>* head;
    DLLNode<T>* tail;
    int size;

public:
    DoublyLinkedList() : head(nullptr), tail(nullptr), size(0) {}

    ~DoublyLinkedList() {
        DLLNode<T>* curr = head;
        while (curr) {
            DLLNode<T>* nxt = curr->next;
            delete curr;
            curr = nxt;
        }
    }

    void insertAtEnd(T data) {
        DLLNode<T>* node = new DLLNode<T>(data);
        if (!tail) { 
            head = tail = node; 
        }
        else { 
            tail->next = node; 
            node->prev = tail; 
            tail = node; 
        }
        size++;
    }

    void insertAtFront(T data) {
        DLLNode<T>* node = new DLLNode<T>(data);
        if (!head) { 
            head = tail = node; 
        }
        else { 
            node->next = head; 
            head->prev = node; 
            head = node; }
        size++;
    }

    DLLNode<T>* findById(int id) {
        DLLNode<T>* curr = head;
        while (curr) {
            if (curr->data.id == id) 
            return curr;
            curr = curr->next;
        }
        return nullptr;
    }

    bool deleteById(int id) {
        DLLNode<T>* curr = head;
        while (curr) {
            if (curr->data.id == id) {
                if (curr->prev) 
                curr->prev->next = curr->next; 
                else head = curr->next;
                if (curr->next) 
                curr->next->prev = curr->prev; 
                else tail = curr->prev;
                delete curr;
                size--;
                return true;
            }
            curr = curr->next;
        }
        return false;
    }

    void traverseForward(void (*visit)(const T&)) const {
        DLLNode<T>* curr = head;
        while (curr) { 
            visit(curr->data); curr = curr->next; 
        }
    }

    void traverseBackward(void (*visit)(const T&)) const {
        DLLNode<T>* curr = tail;
        while (curr) { 
            visit(curr->data); curr = curr->prev; 
        }
    }

    DLLNode<T>* getHead() const { 
        return head; 
    }
    DLLNode<T>* getTail() const { 
        return tail; 
    }
    int getSize() const { 
        return size; 
    }
};
