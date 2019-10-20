#include <bits/stdc++.h>
#include "heuristic.hpp"

using namespace std;

void printGraph(vector<vector<int> >& graph) {
    for (int i = 0; i < graph.size(); i++) {
        cout << i << ": ";
        for (int j = 0; j < graph[i].size(); j++) {
            cout << graph[i][j] << ", ";
        }
        cout << endl;
    }
}

bool testGraph(vector<vector<int> >& original_graph, vector<vector<int> >& myg) {
    if ((int)myg.size() != (int)original_graph.size()) return false;
    for (int i = 0; i < (int)original_graph.size(); i++) {
        set<int> myset;
        for (int j = 0; j < original_graph[i].size(); j++) myset.insert(original_graph[i][j]);
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
    for (int i = 0; i < (int)original_graph.size(); i++) {
        if (!used[i]) return false;
    }
    return true;
}

int getNumberOfBranchs(vector<vector<int> >& graph) {
    int ret = 0 ;
    for (int i = 0; i < (int)graph.size(); i++) {
        if (graph[i].size() > 2) ret++;
    }
    return ret;
}

pair<double, int> getLagrangeRelaxation(vector<vector<int> >& original_graph, vector<double>& lambda, vector<vector<int> >& primal_best) {
    vector<vector<int> > mst = getMSTree(original_graph, lambda);
    vector<double> subgradient(original_graph.size(), 0);

    pair<double, int> ret{0.0, 0};

    for (int i = 0; i < (int)original_graph.size(); i++) {
        if ((int)original_graph[i].size() <= 2) continue;
        ret.first += -2 * lambda[i];
        subgradient[i] = -2;
        if (lambda[i] * (int)original_graph[i].size() > 1) {
            ret.first += 1 - lambda[i] * (int)original_graph[i].size();
            subgradient[i] -= original_graph[i].size();
        }
        ret.first += lambda[i] * mst[i].size();
        subgradient[i] += mst[i].size();
    }

    for (int i = 0; i < (int)original_graph.size(); i++) {
        lambda[i] += 0.001 * subgradient[i];
        lambda[i] = max((double)0, lambda[i]);
    }
    ret.second = getNumberOfBranchs(mst);
    if (ret.second < getNumberOfBranchs(primal_best)) {
        primal_best = mst;
    }
    return ret;
}

int main(int argc, char** argv) {
    if (argc < 2) {
        printf("Usage: ./main <INPUT INSTANCE>\n");
        return -1;
    }
    ifstream fin(argv[1]);
    int n, m;
    fin >> n >> m;

    int min_number_of_branches = n;
    double max_dual = numeric_limits<float>::min();
    vector<vector<int> > g(n);
    vector<double> lambda(n);

    for (int i = 0; i < m; i++) {
        int a, b;
        fin >> a >> b;
        a--; b--;
        g[a].push_back(b);
        g[b].push_back(a);
    }
    transform(g.begin(), g.end(), lambda.begin(), []
        (vector<int>& p) {
            if (p.size() <= 2) {
                return 0.0;
            } else {
                return (double)1 / (double)(2 * p.size());
            }
        });
    


    vector<vector<int> > primal_best = getHeuristicTree(g);
    min_number_of_branches = getNumberOfBranchs(primal_best);

    for (int i = 0; i < 15000; i++) {
        pair<double, int> dual_primal = getLagrangeRelaxation(g, lambda, primal_best);
        if (dual_primal.first > max_dual) {
            max_dual = dual_primal.first;
        }
        if (dual_primal.second < min_number_of_branches) {
            min_number_of_branches = dual_primal.second;
        }
        cout << "lagrangian relax: " << dual_primal.first << " " << dual_primal.second << endl;
    }
    cout << max_dual << " " << min_number_of_branches << endl;
    return 0;
}