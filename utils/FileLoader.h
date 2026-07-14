#pragma once
#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>
using namespace std;

#include "../models/Location.h"
#include "../structures/DoublyLinkedList.h"
#include "../structures/AVLTree.h"
#include "../graph/Graph.h"

// Trim leading/trailing spaces and \r
void trimStr(char* s) {
    int len = strlen(s);
    // Remove trailing \r \n spaces
    while (len > 0 && (s[len-1] == '\r' || s[len-1] == '\n' || s[len-1] == ' '))
        s[--len] = '\0';
    // Remove leading spaces
    int start = 0;
    while (s[start] == ' ') start++;
    if (start > 0) memmove(s, s + start, len - start + 1);
}

// Split line by comma into tokens, returns count
int splitCSV(char* line, char tokens[][200], int maxTok) {
    int count = 0;
    char* p = line;
    while (*p && count < maxTok) {
        char* start = p;
        while (*p && *p != ',') p++;
        int len = (int)(p - start);
        if (len >= 200) len = 199;
        strncpy(tokens[count], start, len);
        tokens[count][len] = '\0';
        trimStr(tokens[count]);
        count++;
        if (*p == ',') p++;
    }
    return count;
}

bool loadCampusData(const char* filepath,
                    DoublyLinkedList<Location>& dll,
                    Graph& graph,
                    AVLTree& avl)
{
    ifstream file(filepath);
    if (!file.is_open()) {
        cout << "[Error] Cannot open file: " << filepath << "\n";
        return false;
    }

    char line[512];
    enum Section { NONE, LOCATIONS, EDGES } section = NONE;

    while (file.getline(line, 512)) {
        trimStr(line);
        if (strlen(line) == 0) continue;

        // Section headers
        if (strcmp(line, "LOCATIONS") == 0) { section = LOCATIONS; continue; }
        if (strcmp(line, "EDGES")     == 0) { section = EDGES;     continue; }

        // Skip header rows
        if (strncmp(line, "id,", 3) == 0 || strncmp(line, "from_id,", 8) == 0) continue;

        char tok[6][200];
        int cnt = splitCSV(line, tok, 6);

        if (section == LOCATIONS && cnt >= 5) {
            int id = atoi(tok[0]);
            bool emerg = (strcmp(tok[4], "yes") == 0);

            Location loc(id, tok[1], tok[2], tok[3], emerg);
            dll.insertAtEnd(loc);
            graph.addLocation(id, tok[1], tok[2], emerg);
            avl.insert(tok[1], id);
        }
        else if (section == EDGES && cnt >= 5) {
            int from  = atoi(tok[0]);
            int to    = atoi(tok[1]);
            int dist  = atoi(tok[2]);
            int t     = atoi(tok[3]);
            bool bidir = (strcmp(tok[4], "yes") == 0);
            graph.addEdge(from, to, dist, t, bidir);
        }
    }

    file.close();
    cout << "  [OK] Loaded " << dll.getSize() << " locations from dataset.\n";
    return true;
}
