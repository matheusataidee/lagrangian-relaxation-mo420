#include <bits/stdc++.h>
#include "heuristic.hpp"

using namespace std;

#define MAXN 1010

vector<vector<int> > g;

void printGraph(vector<vector<int> >& graph) {
    for (int i = 0; i < graph.size(); i++) {
        cout << i << ": ";
        for (int j = 0; j < graph[i].size(); j++) {
            cout << graph[i][j] << ", ";
        }
        cout << endl;
    }
}

bool testGraph(vector<vector<int> >& myg) {
    if ((int)myg.size() != (int)g.size()) return false;
    for (int i = 0; i < (int)g.size(); i++) {
        set<int> myset;
        for (int j = 0; j < g[i].size(); j++) myset.insert(g[i][j]);
        for (int j = 0; j < myg[i].size(); j++) {
            if (myset.find(myg[i][j]) == myset.end()) return false;
            myset.erase(myg[i][j]);
        }
    }
    queue<int> myq;
    vector<bool> used(myg.size(), false);
    myq.push(0);
    used[0] = true;
    while (!myq.empty()) {
        int p = myq.front(); myq.pop();
        for (int i = 0; i < myg[p].size(); i++) {
            int to = myg[p][i];
            if (!used[to]) {
                used[to] = true;
                myq.push(to);
            }
        }
    }
    for (int i = 0; i < (int)g.size(); i++) {
        if (!used[i]) return false;
    }
    return true;
}

int main(int argc, char** argv) {
    if (argc < 2) {
        printf("Usage: ./main <INPUT INSTANCE>\n");
        return -1;
    }
    ifstream fin(argv[1]);
    int n, m;
    fin >> n >> m;
    g = vector<vector<int> >(n);
    for (int i = 0; i < m; i++) {
        int a, b;
        fin >> a >> b;
        a--; b--;
        g[a].push_back(b);
        g[b].push_back(a);
    }
    vector<vector<int> > primal_best = getHeuristicTree(g);
    printGraph(primal_best);
    if (testGraph(primal_best)) cout << "OK" << endl;
    return 0;
}