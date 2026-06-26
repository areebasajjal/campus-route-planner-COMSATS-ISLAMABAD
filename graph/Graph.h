#pragma once
#include <iostream>
#include <cstring>
using namespace std;

const int MAX_LOC = 50;

struct EdgeNode {
    int dest;
    int distance; // meters
    int time;     // mins
    bool blocked;
    EdgeNode* next;

    EdgeNode(int d, int dist, int t)
        : dest(d), distance(dist), time(t), blocked(false), next(nullptr) {}
};

class Graph {
private:
    EdgeNode* adjList[MAX_LOC];
    char      locName[MAX_LOC][100];
    char      locType[MAX_LOC][50];
    bool      emergency[MAX_LOC];
    int       numLoc;

public:
    Graph() : numLoc(0) {
        for (int i = 0; i < MAX_LOC; i++) {
            adjList[i] = nullptr;
            locName[i][0] = '\0';
            locType[i][0] = '\0';
            emergency[i] = false;
        }
    }

   

    void addLocation(int id, const char* name, const char* type, bool emerg = false) {
        if (id < 0 || id >= MAX_LOC) return;
        strncpy(locName[id], name, 99); locName[id][99] = '\0';
        strncpy(locType[id], type, 49); locType[id][49] = '\0';
        emergency[id] = emerg;
        if (id >= numLoc) numLoc = id + 1;
    }

    void addEdge(int from, int to, int dist, int t, bool bidir = true) {
        EdgeNode* e1 = new EdgeNode(to, dist, t);
        e1->next = adjList[from]; adjList[from] = e1;
        if (bidir) {
            EdgeNode* e2 = new EdgeNode(from, dist, t);
            e2->next = adjList[to]; adjList[to] = e2;
        }
    }

    bool blockEdge(int from, int to) {
        bool found = false;
        for (EdgeNode* e = adjList[from]; e; e = e->next)
            if (e->dest == to) { e->blocked = true; found = true; break; }
        for (EdgeNode* e = adjList[to]; e; e = e->next)
            if (e->dest == from) { e->blocked = true; break; }
        return found;
    }

    bool unblockEdge(int from, int to) {
        bool found = false;
        for (EdgeNode* e = adjList[from]; e; e = e->next)
            if (e->dest == to) { e->blocked = false; found = true; break; }
        for (EdgeNode* e = adjList[to]; e; e = e->next)
            if (e->dest == from) { e->blocked = false; break; }
        return found;
    }

    EdgeNode* getEdges(int id) const {
        if (id < 0 || id >= MAX_LOC) return nullptr;
        return adjList[id];
    }

    const char* getName(int id) const {
        if (id < 0 || id >= MAX_LOC) return "Unknown";
        return locName[id];
    }

    const char* getType(int id) const {
        if (id < 0 || id >= MAX_LOC) return "";
        return locType[id];
    }

    bool isEmergency(int id) const {
        if (id < 0 || id >= MAX_LOC) return false;
        return emergency[id];
    }

    int getNumLoc() const { return numLoc; }

    void printAdjacencyList() const {
        cout << "\n  [Graph - Adjacency List]\n";
        for (int i = 1; i < numLoc; i++) {
            if (locName[i][0] == '\0') continue;
            cout << "  " << locName[i] << " --> ";
            EdgeNode* e = adjList[i];
            if (!e) { cout << "(no connections)\n"; continue; }
            while (e) {
                if (!e->blocked)
                    cout << locName[e->dest] << "(" << e->distance << "m," << e->time << "min)";
                else
                    cout << locName[e->dest] << "[BLOCKED]";
                if (e->next) cout << " | ";
                e = e->next;
            }
            cout << "\n";
        }
    }
};
