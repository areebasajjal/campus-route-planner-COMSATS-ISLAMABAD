/*
 * api_backend.cpp
 * Called by server.py with arguments:
 *   api_backend <command> [args...]
 *
 * Commands:
 *   locations
 *   shortest <src_id> <dst_id>
 *   bfs <src_id>
 *   dfs <src_id>
 *   emergency <src_id>
 *   routes <src_id> <dst_id>
 *   connectivity <src_id> <dst_id>
 *   block <from_id> <to_id>
 *   unblock <from_id> <to_id>
 *   graph
 *
 * Outputs JSON to stdout.
 */

#include <iostream>
#include <cstring>
#include <cstdlib>
using namespace std;

#include "models/Location.h"
#include "structures/DoublyLinkedList.h"
#include "structures/Stack.h"
#include "structures/Queue.h"
#include "structures/PriorityQueue.h"
#include "structures/AVLTree.h"
#include "graph/Graph.h"
#include "algorithms/Dijkstra.h"
#include "algorithms/BFS.h"
#include "algorithms/DFS.h"
#include "utils/FileLoader.h"

DoublyLinkedList<Location> locationList;
Graph campusGraph;
AVLTree locationIndex;

// JSON helpers 
void jsonStr(const char* s) {
    cout << "\"";
    for (const char* p = s; *p; p++) {
        if (*p == '"')  cout << "\\\"";
        else if (*p == '\\') cout << "\\\\";
        else            cout << *p;
    }
    cout << "\"";
}

// Commands 

void cmdLocations() {

    cout << "{\"locations\":[";

    DLLNode<Location>* node = locationList.getHead();

    bool first = true;

    while (node) {

        if (!first) cout << ",";
        first = false;

        Location& l = node->data;

        cout << "{";
        cout << "\"id\":" << l.id << ",";
        cout << "\"name\":"; jsonStr(l.name); cout << ",";
        cout << "\"type\":"; jsonStr(l.type); cout << ",";
        cout << "\"description\":"; jsonStr(l.description); cout << ",";
        cout << "\"emergency\":" << (l.isEmergencyPoint ? "true" : "false");
        cout << "}";

        node = node->next;
    }

    cout << "]}";
}

void cmdShortest(int src, int dst) {

    DijkstraResult res = runDijkstra(campusGraph, src, dst);

    cout << "{";

    cout << "\"reachable\":" << (res.reachable ? "true" : "false") << ",";

    if (res.reachable) {
        // build path
        int pathArr[MAX_LOC], pathLen = 0, curr = dst;
        while (curr != -1) { pathArr[pathLen++] = curr; curr = res.prev[curr]; }
        cout << "\"path\":[";

        for (int i = pathLen - 1; i >= 0; i--) {

            if (i != pathLen-1) cout << ",";

            cout << "{\"id\":" << pathArr[i] << ",\"name\":";
            jsonStr(campusGraph.getName(pathArr[i]));

            cout << "}";
        }
        cout << "],";
        cout << "\"distance\":" << res.dist[dst] << ",";
        cout << "\"time\":" << res.timeCost[dst];
    }
    cout << "}";
}

void cmdBFS(int src) {

    bool visited[MAX_LOC] = {false};

    Queue q;

    visited[src] = true;

    q.enqueue(src);

    cout << "{\"start\":"; jsonStr(campusGraph.getName(src)); cout << ",\"order\":[";

    bool first = true;

    while (!q.isEmpty()) {

        int u = q.dequeue();

        if (!first) cout << ",";
        first = false;

        cout << "{\"id\":" << u << ",\"name\":";
        jsonStr(campusGraph.getName(u));

        cout << "}";

        EdgeNode* edge = campusGraph.getEdges(u);

        while (edge) {
            if (!edge->blocked && !visited[edge->dest]) {
                visited[edge->dest] = true;
                q.enqueue(edge->dest);
            }

            edge = edge->next;
        }
    }

    cout << "]}";
}

void cmdDFS(int src) {

    bool visited[MAX_LOC] = {false};

    Stack stk;

    stk.push(src);

    cout << "{\"start\":"; jsonStr(campusGraph.getName(src)); cout << ",\"order\":[";

    bool first = true;

    while (!stk.isEmpty()) {

        int u = stk.pop();

        if (visited[u]) 
         continue;

        visited[u] = true;

        if (!first) 
        cout << ",";

        first = false;

        cout << "{\"id\":" << u << ",\"name\":";

        jsonStr(campusGraph.getName(u));
        cout << "}";

        int neighbors[MAX_LOC], cnt = 0;
        EdgeNode* edge = campusGraph.getEdges(u);

        while (edge) {

            if (!edge->blocked && !visited[edge->dest])
                neighbors[cnt++] = edge->dest;
            edge = edge->next;
        }

        for (int i = cnt - 1; i >= 0; i--) 
        stk.push(neighbors[i]);
    }
    cout << "]}";
}

void cmdEmergency(int src) {

    bool visited[MAX_LOC] = {false};
    int parent[MAX_LOC];

    for (int i = 0; i < MAX_LOC; i++) 
    parent[i] = -1;

    Queue q;

    visited[src] = true;

    q.enqueue(src);

    while (!q.isEmpty()) {

        int u = q.dequeue();

        if (campusGraph.isEmergency(u) && u != src) {

            int path[MAX_LOC], pathLen = 0, curr = u;

            while (curr != -1) { 
                path[pathLen++] = curr; curr = parent[curr]; 
            }

            cout << "{\"found\":true,\"emergency\":";
            jsonStr(campusGraph.getName(u));

            cout << ",\"path\":[";

            for (int i = pathLen - 1; i >= 0; i--) {
                if (i != pathLen-1) cout << ",";
                
                cout << "{\"id\":" << path[i] << ",\"name\":";
                jsonStr(campusGraph.getName(path[i]));
                cout << "}";
            }
            cout << "]}";
            return;
        }
        EdgeNode* edge = campusGraph.getEdges(u);
        while (edge) {
            if (!edge->blocked && !visited[edge->dest]) {
                visited[edge->dest] = true;
                parent[edge->dest] = u;
                q.enqueue(edge->dest);
            }
            edge = edge->next;
        }
    }
    cout << "{\"found\":false}";
}

void cmdRoutes(int src, int dst) {

    cout << "{\"routes\":[";
    int routeCount = 0;

    DijkstraResult r1 = runDijkstra(campusGraph, src, dst);
    if (!r1.reachable) { cout << "]}"; return; }

    auto printRoute = [&](DijkstraResult& r, const char* label) {

        if (!r.reachable) 
        return;
        if (routeCount > 0) cout << ",";
        routeCount++;

        int pathArr[MAX_LOC], pathLen = 0, curr = dst;

        while (curr != -1) { 
            pathArr[pathLen++] = curr; curr = r.prev[curr]; 
        }
        cout << "{\"label\":"; jsonStr(label); cout << ",";
        cout << "\"path\":[";

        for (int i = pathLen - 1; i >= 0; i--) {

            if (i != pathLen-1) cout << ",";
            cout << "{\"id\":" << pathArr[i] << ",\"name\":";
            jsonStr(campusGraph.getName(pathArr[i]));
            cout << "}";
        }
        cout << "],\"distance\":" << r.dist[dst] << ",\"time\":" << r.timeCost[dst] << "}";
    };

    printRoute(r1, "Route 1 - Shortest");

    int pathArr[MAX_LOC], pathLen = 0, curr = dst;
    while (curr != -1) { pathArr[pathLen++] = curr; curr = r1.prev[curr]; }

    if (pathLen >= 2) {
        int p1 = pathArr[pathLen-1], p2 = pathArr[pathLen-2];
        campusGraph.blockEdge(p1, p2);
        DijkstraResult r2 = runDijkstra(campusGraph, src, dst);
        printRoute(r2, "Route 2 - Alternative A");

        if (r2.reachable && pathLen >= 3) {
            int p3 = pathArr[pathLen-3];
            campusGraph.blockEdge(p2, p3);
            DijkstraResult r3 = runDijkstra(campusGraph, src, dst);
            printRoute(r3, "Route 3 - Alternative B");
            campusGraph.unblockEdge(p2, p3);
        }
        campusGraph.unblockEdge(p1, p2);
    }
    cout << "]}";
}

void cmdConnectivity(int src, int dst) {

    bool connected = isConnected(campusGraph, src, dst);

    cout << "{";
    cout << "\"connected\":" << (connected ? "true" : "false") << ",";
    cout << "\"from\":"; jsonStr(campusGraph.getName(src)); cout << ",";
    cout << "\"to\":"; jsonStr(campusGraph.getName(dst));
    cout << "}";
}

void cmdBlock(int from, int to) {

    bool ok = campusGraph.blockEdge(from, to);

    cout << "{\"ok\":" << (ok ? "true" : "false") << ",";
    cout << "\"from\":"; jsonStr(campusGraph.getName(from)); cout << ",";
    cout << "\"to\":"; jsonStr(campusGraph.getName(to));
    cout << "}";
}

void cmdUnblock(int from, int to) {

    bool ok = campusGraph.unblockEdge(from, to);

    cout << "{\"ok\":" << (ok ? "true" : "false") << ",";
    cout << "\"from\":"; jsonStr(campusGraph.getName(from)); cout << ",";
    cout << "\"to\":"; jsonStr(campusGraph.getName(to));
    cout << "}";
}

void cmdGraph() {

    cout << "{\"adjacency\":[";

    bool firstLoc = true;

    for (int i = 1; i < campusGraph.getNumLoc(); i++) {

        if (campusGraph.getName(i)[0] == '\0') continue;
        if (!firstLoc) cout << ",";
        firstLoc = false;

        cout << "{\"id\":" << i << ",\"name\":";

        jsonStr(campusGraph.getName(i));

        cout << ",\"edges\":[";

        EdgeNode* e = campusGraph.getEdges(i);

        bool firstEdge = true;

        while (e) {
            if (!firstEdge) cout << ",";
            firstEdge = false;

            cout << "{\"to\":" << e->dest << ",\"name\":";
            jsonStr(campusGraph.getName(e->dest));

            cout << ",\"distance\":" << e->distance;
            cout << ",\"time\":" << e->time;
            cout << ",\"blocked\":" << (e->blocked ? "true" : "false") << "}";
            e = e->next;
        }
        cout << "]}";
    }
    cout << "]}";
}

// Main 
int main(int argc, char* argv[]) {

    if (argc < 2) {

        cout << "{\"error\":\"No command given\"}";
        return 1;
    }

    if (!loadCampusData("data/campus_map.csv", locationList, campusGraph, locationIndex)) {

        cout << "{\"error\":\"Could not load campus_map.csv\"}";
        return 1;
    }

    const char* cmd = argv[1];

    if (strcmp(cmd, "locations") == 0) {
        cmdLocations();
    }
    else if (strcmp(cmd, "shortest") == 0 && argc >= 4) {
        cmdShortest(atoi(argv[2]), atoi(argv[3]));
    }
    else if (strcmp(cmd, "bfs") == 0 && argc >= 3) {
        cmdBFS(atoi(argv[2]));
    }
    else if (strcmp(cmd, "dfs") == 0 && argc >= 3) {
        cmdDFS(atoi(argv[2]));
    }
    else if (strcmp(cmd, "emergency") == 0 && argc >= 3) {
        cmdEmergency(atoi(argv[2]));
    }
    else if (strcmp(cmd, "routes") == 0 && argc >= 4) {
        cmdRoutes(atoi(argv[2]), atoi(argv[3]));
    }
    else if (strcmp(cmd, "connectivity") == 0 && argc >= 4) {
        cmdConnectivity(atoi(argv[2]), atoi(argv[3]));
    }
    else if (strcmp(cmd, "block") == 0 && argc >= 4) {
        cmdBlock(atoi(argv[2]), atoi(argv[3]));
    }
    else if (strcmp(cmd, "unblock") == 0 && argc >= 4) {
        cmdUnblock(atoi(argv[2]), atoi(argv[3]));
    }
    else if (strcmp(cmd, "graph") == 0) {
        cmdGraph();
    }
    else {
        cout << "{\"error\":\"Unknown command\"}";
        return 1;
    }

    cout << endl;
    return 0;
}
