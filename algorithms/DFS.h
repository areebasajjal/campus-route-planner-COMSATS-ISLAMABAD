#pragma once
#include "../graph/Graph.h"
#include "../structures/Stack.h"
#include <iostream>
using namespace std;

void runDFS(const Graph& g, int src) {
    bool visited[MAX_LOC] = {false};
    Stack stk;

    stk.push(src);
    cout << " [DFS Traversal] Starting from: " << g.getName(src) << "";
    cout << "  Visit Order: ";

    bool first = true;
    while (!stk.isEmpty()) {
        int u = stk.pop();
        if (visited[u]) continue;
        visited[u] = true;

        if (!first) cout << " -> ";
        cout << g.getName(u);
        first = false;

       
       
        int neighbors[MAX_LOC], cnt = 0;

        EdgeNode* edge = g.getEdges(u);

        while (edge) {

            if (!edge->blocked && !visited[edge->dest])
                neighbors[cnt++] = edge->dest;
            edge = edge->next;
        }
        
        for (int i = cnt - 1; i >= 0; i--)
            stk.push(neighbors[i]);
    }
   // cout << "\n";
}


// returns true if dest is reachable from src
bool isConnected(const Graph& g, int src, int dest) {

    bool visited[MAX_LOC] = {false};
    Stack stk;

    stk.push(src);

    while (!stk.isEmpty()) {

        int u = stk.pop();

        if (visited[u]) 
        continue;

        visited[u] = true;
        if (u == dest) 
        return true;

        EdgeNode* edge = g.getEdges(u);
        while (edge) {
            if (!edge->blocked && !visited[edge->dest])
                stk.push(edge->dest);
            edge = edge->next;
        }
    }
    return false;
}
