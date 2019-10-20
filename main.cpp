#include <bits/stdc++.h>
#include "heuristic.hpp"

using namespace std;

#define MAXN 1010

int min_number_of_branches;
vector<double> lambda;
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

int getNumberOfBranchs(vector<vector<int> >& graph) {
    int ret = 0 ;
    for (int i = 0; i < (int)graph.size(); i++) {
        if (graph[i].size() > 2) ret++;
    }
    return ret;
}

double getLagrangeRelaxation() {
    vector<vector<int> > mst = getMSTree(g, lambda);
    vector<double> subgradient(g.size(), 0);

    double precision_parameter = 0.0;
    double subgradient_square_sum = 0.0;
    double ret = 0.0;

    for (int i = 0; i < (int)g.size(); i++) {
        if ((int)g[i].size() <= 2) continue;
        ret += -2 * lambda[i];
        subgradient[i] = -2;
        if (lambda[i] * (int)g[i].size() > 1) {
            ret += 1 - lambda[i] * (int)g[i].size();
            subgradient[i] -= g[i].size();
        }
        ret += lambda[i] * mst[i].size();
        subgradient[i] += mst[i].size();
        precision_parameter += lambda[i] * -subgradient[i];
        subgradient_square_sum += subgradient[i] * subgradient[i];
    }
    double stepsize = precision_parameter * ((double)min_number_of_branches - ret / subgradient_square_sum);

    for (int i = 0; i < (int)g.size(); i++) {
        lambda[i] += stepsize * subgradient[i];
        //lambda[i] += 0.1 * subgradient[i];
        lambda[i] = max((double)0, lambda[i]);
        lambda[i] = min((double)1 / (double)g[i].size(), lambda[i]);
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
    min_number_of_branches = n;
    g = vector<vector<int> >(n);
    lambda = vector<double>(n);
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
    vector<vector<int> > mst_g = getMSTree(g, lambda);
    printGraph(primal_best);
    if (testGraph(primal_best)) cout << "OK" << endl;
    printGraph(mst_g);
    if (testGraph(mst_g)) cout << "OK" << endl;

    for (int i = 0; i < 150; i++) {
        cout << "lagrangian relax: " << getLagrangeRelaxation() << endl;
    }
    return 0;
}