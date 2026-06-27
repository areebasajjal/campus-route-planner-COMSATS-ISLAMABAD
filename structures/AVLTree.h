#pragma once
#include <iostream>
#include <cstring>
#include <cctype>
using namespace std;

struct AVLNode {
    char key[100];    // lowercase location name
    int locationId;
    int height;
    AVLNode* left;
    AVLNode* right;

    AVLNode(const char* k, int id) : locationId(id), height(1), left(nullptr), right(nullptr) {
        strncpy(key, k, 99); key[99] = '\0';
    }
};

class AVLTree {
private:
    AVLNode* root;

    int height(AVLNode* n) const { 
        return n ? n->height : 0; 
    }

    int maxH(int a, int b) { 
        return a > b ? a : b; 
    }

    void updateHeight(AVLNode* n) {
        if (n) n->height = 1 + maxH(height(n->left), height(n->right));
    }

    int bf(AVLNode* n) { 
        return n ? height(n->left) - height(n->right) : 0; 
    }

    AVLNode* rotateRight(AVLNode* y) {
        AVLNode* x = y->left; 
        AVLNode* T2 = x->right;
        x->right = y; 
        y->left = T2;
        updateHeight(y); 
        updateHeight(x);
        return x;
    }

    AVLNode* rotateLeft(AVLNode* x) {
        AVLNode* y = x->right; 
        AVLNode* T2 = y->left;
        y->left = x; 
        x->right = T2;
        updateHeight(x);
        updateHeight(y);
        return y;
    }

    AVLNode* balance(AVLNode* n) {
        updateHeight(n);
        int b = bf(n);
        if (b > 1) {
            if (bf(n->left) < 0) 
            n->left = rotateLeft(n->left);
            return rotateRight(n);
        }
        if (b < -1) {
            if (bf(n->right) > 0) 
            n->right = rotateRight(n->right);
            return rotateLeft(n);
        }
        return n;
    }

    void toLower(const char* src, char* dest) {
        int i = 0;
        while (src[i]) { 
            dest[i] = tolower((unsigned char)src[i]); i++; }
        dest[i] = '\0';
    }

    AVLNode* insert(AVLNode* node, const char* key, int id) {
        if (!node) 
        return new AVLNode(key, id);
        int cmp = strcmp(key, node->key);
        if (cmp < 0)      
        node->left  = insert(node->left,  key, id);
        else if (cmp > 0) 
        node->right = insert(node->right, key, id);
        else 
        return node; // duplicate ignored
        return balance(node);
    }

    AVLNode* searchNode(AVLNode* node, const char* key) {
        if (!node) 
        return nullptr;
        int cmp = strcmp(key, node->key);
        if (cmp == 0) 
        return node;
        return cmp < 0 ? searchNode(node->left, key) : searchNode(node->right, key);
    }

    void inorder(AVLNode* node) const {
        if (!node)
         return;
        inorder(node->left);
        cout << "    [ID:" << node->locationId << "] " << node->key << "\n";
        inorder(node->right);
    }

    void destroy(AVLNode* node) {
        if (!node) 
        return;
        destroy(node->left); 
        destroy(node->right);
        delete node;
    }

public:
    AVLTree() : root(nullptr) {}
    ~AVLTree() { destroy(root); }

    void insert(const char* name, int id) {
        char lower[100]; toLower(name, lower);
        root = insert(root, lower, id);
    }

    // Returns locationId or -1 if not found
    int search(const char* name) {
        char lower[100]; toLower(name, lower);
        AVLNode* res = searchNode(root, lower);
        return res ? res->locationId : -1;
    }

    void printInorder() const {
        cout << "\n  [AVL Tree - Alphabetical Location Index]\n";
        inorder(root);
    }

    int getHeight() const { return height(root); }
};
