#pragma once
#include "../graph/Graph.h"
#include "../structures/Queue.h"
#include <iostream>
using namespace std;

void runBFS(const Graph& g, int src) {

    bool visited[MAX_LOC] = {false};
    Queue q;

    visited[src] = true;
    q.enqueue(src);

    cout << " [BFS Traversal] Starting from: " << g.getName(src) << "";
    cout << "  Visit Order: ";

    while (!q.isEmpty()) {
        int u = q.dequeue();
        cout << g.getName(u);

        EdgeNode* edge = g.getEdges(u);
        while (edge) {
            if (!edge->blocked && !visited[edge->dest]) {
                visited[edge->dest] = true;
                q.enqueue(edge->dest);
            }
            edge = edge->next;
        }
        if (!q.isEmpty()) cout << " -> ";
    }
}

// finds nearest emergency point 
void emergencyBFS(const Graph& g, int src) {
    bool visited[MAX_LOC] = {false};
    int parent[MAX_LOC];
    for (int i = 0; i < MAX_LOC; i++) parent[i] = -1;

    Queue q;
    visited[src] = true;
    q.enqueue(src);

    cout << "[Emergency Mode] Finding nearest emergency point from: "
         << g.getName(src) << "";

    while (!q.isEmpty()) {
        int u = q.dequeue();

        if (g.isEmergency(u) && u != src) {
            
            cout << "  Nearest Emergency Point: " << g.getName(u) << "";
            
            int path[MAX_LOC], pathLen = 0, curr = u;

            while (curr != -1) { 
                path[pathLen++] = curr; curr = parent[curr]; 
            }

            cout << "  Route: ";

            for (int i = pathLen - 1; i >= 0; i--) {
                    cout << g.getName(path[i]);
                    if (i > 0) 
                    cout << " ==> ";
            }
           // cout << "\n";
            return;
        }

        EdgeNode* edge = g.getEdges(u);
        while (edge) {

            if (!edge->blocked && !visited[edge->dest]) {
                
                visited[edge->dest] = true;
                parent[edge->dest] = u;
                q.enqueue(edge->dest);
            }
            edge = edge->next;
        }
    }
    cout << " No emergency point reachable from current location.";
}
