#pragma once
#include "../graph/Graph.h"
#include "../structures/PriorityQueue.h"
#include "../structures/Stack.h"
#include <iostream>
using namespace std;

const int INF = 999999;

struct DijkstraResult {
    int dist[MAX_LOC];
    int timeCost[MAX_LOC];
    int prev[MAX_LOC];
    bool reachable;
};

DijkstraResult runDijkstra(const Graph& g, int src, int dest) {
    DijkstraResult res;
    res.reachable = false;
    

    for (int i = 0; i < MAX_LOC; i++) {
        res.dist[i] = INF;
        res.timeCost[i] = INF;
        res.prev[i] = -1;
    }

    res.dist[src] = 0;
    res.timeCost[src] = 0;

    PriorityQueue pq;
    pq.push(src, 0);

    while (!pq.isEmpty()) {
        PQNode curr = pq.pop();
        int u = curr.vertex;
        if (curr.distance > res.dist[u])
        { continue;} 

        EdgeNode* edge = g.getEdges(u);
        while (edge) {
            if (!edge->blocked) {
                int v = edge->dest;
                int newDist = res.dist[u] + edge->distance;
                int newTime = res.timeCost[u] + edge->time;
                if (newDist < res.dist[v]) {
                    res.dist[v] = newDist;
                    res.timeCost[v] = newTime;
                    res.prev[v] = u;
                    pq.push(v, newDist);
                }
            }
            edge = edge->next;
        }
    }

    if (res.dist[dest] < INF) res.reachable = true;
    return res;
}

void printShortestPath(const Graph& g, const DijkstraResult& res, int src, int dest) {
    if (!res.reachable) {
        cout << "[!] No reachable path from " << g.getName(src)
             << " to " << g.getName(dest) << " (path may be blocked).";
        return;
    }

    
    Stack pathStack;
    int curr = dest;
    while (curr != -1) {
        pathStack.push(curr);
        curr = res.prev[curr];
    }

    cout << " [Shortest Path] " << g.getName(src) << " --> " << g.getName(dest) << "";
    cout << "  Route: ";
    while (!pathStack.isEmpty()) {
        int node = pathStack.pop();
        cout << g.getName(node);
        if (!pathStack.isEmpty()) cout << " ==> ";
    }
 //   cout << "\n";
    cout << "  Total Distance : " << res.dist[dest] << " meters\n";
    cout << "  Estimated Time : " << res.timeCost[dest] << " minutes\n";
}
