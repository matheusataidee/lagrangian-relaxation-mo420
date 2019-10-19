#include "heuristic.hpp"

void dfs(int p, vector<vector<int> >& graph, vector<int>& d, vector<int>& pred,
                                            int& maxi, int& id_maxi) {
    if (d[p] > maxi) {
        maxi = d[p];
        id_maxi = p;
    }
    for (int i = 0; i < graph[p].size(); i++) {
        int to = graph[p][i];
        if (d[to] == -1) {
            d[to] = d[p] + 1;
            pred[to] = p;
            dfs(to, graph, d, pred, maxi, id_maxi);
        }
    }
}

vector<vector<int> > getHeuristicTree(vector<vector<int> >& graph) {
    vector<int> d(graph.size(), -1);
    vector<int> pred(graph.size(), -1);
    vector<bool> used(graph.size(), false);
    vector<vector<int> > ret(graph.size());
    int maxi = 0, id_maxi = 0, extremity_node = 0;
    d[0] = 0;
    dfs(0, graph, d, pred, maxi, id_maxi);
    
    cout << "After first dfs" << endl;

    extremity_node = id_maxi;
    id_maxi = maxi = 0;
    fill(pred.begin(), pred.end(), -1);
    fill(d.begin(), d.end(), -1);
    d[extremity_node] = 0;
    pred[extremity_node] = -1;

    dfs(extremity_node, graph, d, pred, maxi, id_maxi);
    cout << "After Second dfs" << endl;
    while (pred[id_maxi] != -1) {
        ret[id_maxi].push_back(pred[id_maxi]);
        ret[pred[id_maxi]].push_back(id_maxi);
        used[id_maxi] = used[pred[id_maxi]] = true;
        id_maxi = pred[id_maxi];
    }
    cout << "After diameter while" << endl;
    /*for (int i = 0; i < ret.size(); i++) {
        cout << i << ": ";
        for (int j = 0; j < ret[i].size(); j++) {
            cout << ret[i][j] << ", ";
        }
        cout << endl;
    } */
    int qnt = accumulate(used.cbegin(), used.cend(), (int)0, [] 
            (int prv, bool x) {
                return x ? prv + 1 : prv;
            });
    while (qnt < graph.size()) {
        cout << qnt << endl;
        maxi = id_maxi = -1;
        for (int i = 0; i < graph.size(); i++) {
            if (!used[i]) continue;
            int acc = 0;
            for (int j = 0; j < graph[i].size(); j++) {
                int to = graph[i][j];
                if (!used[to]) acc++;
            }
            if (acc > maxi) {
                maxi = acc;
                id_maxi = i;
            }
        }
        queue<int> myq;
        for (int i = 0; i < graph[id_maxi].size(); i++) {
            int to = graph[id_maxi][i];
            if (used[to]) continue;
            cout << "maoe" << endl;
            myq.push(to);
            used[to] = true;
            ret[id_maxi].push_back(to);
            ret[to].push_back(id_maxi);
        }
        while (!myq.empty()) {
            int p = myq.front(); myq.pop();
            for (int i = 0; i < graph[p].size(); i++) {
                int to = graph[p][i];
                if (used[to]) continue;
                myq.push(to);
                used[to] = true;
                ret[p].push_back(to);
                ret[to].push_back(p);
            }
        }
        qnt = accumulate(used.cbegin(), used.cend(), (int)0, [] 
            (int prv, bool x) {
                return x ? prv + 1 : prv;
            });
    }
    return ret;
}