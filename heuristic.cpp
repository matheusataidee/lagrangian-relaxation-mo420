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

    extremity_node = id_maxi;
    id_maxi = maxi = 0;
    fill(pred.begin(), pred.end(), -1);
    fill(d.begin(), d.end(), -1);
    d[extremity_node] = 0;
    pred[extremity_node] = -1;

    dfs(extremity_node, graph, d, pred, maxi, id_maxi);
    while (pred[id_maxi] != -1) {
        ret[id_maxi].push_back(pred[id_maxi]);
        ret[pred[id_maxi]].push_back(id_maxi);
        used[id_maxi] = used[pred[id_maxi]] = true;
        id_maxi = pred[id_maxi];
    }
    int qnt = accumulate(used.cbegin(), used.cend(), (int)0, [] 
            (int prv, bool x) {
                return x ? prv + 1 : prv;
            });
    while (qnt < graph.size()) {
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

pair<int, int> getInitialEdge(vector<vector<int> >& graph, vector<double>& lambda) {
    double mini = 0x7fffffff;
    pair<int, int> ret{-1, -1};
    for (int i = 0; i < graph.size(); i++) {
        for (int j = 0; j < graph[i].size(); j++) {
            int to = graph[i][j];
            if (lambda[i] + lambda[to] < mini) {
                mini = lambda[i] + lambda[to];
                ret = {i, to};
            }
        }
    }
    return ret;
}

vector<vector<int> > getMSTree(vector<vector<int> >& graph, vector<double>& lambda) {
    vector<vector<int> > ret(graph.size());
    priority_queue<pair<double, pair<int, int> >, vector<pair<double, pair<int, int> > >, greater<pair<double, pair<int, int> > > > heap;
    pair<int, int> initial_edge = getInitialEdge(graph, lambda);
    ret[initial_edge.first].push_back(initial_edge.second);
    ret[initial_edge.second].push_back(initial_edge.first);
    for (int k = 0; k < 2; k++) {
        int p = (k == 0) ? initial_edge.first : initial_edge.second;
        for (int i = 0; i < graph[p].size(); i++) {
            int to = graph[p][i];
            if (!ret[to].empty()) continue;
            heap.push({lambda[p] + lambda[to], {p, to}});
        }
    }
    while (!heap.empty()) {
        int from = heap.top().second.first;
        int p = heap.top().second.second; heap.pop();
        if (!ret[p].empty()) continue;
        ret[from].push_back(p);
        ret[p].push_back(from);
        for (int i = 0; i < (int)graph[p].size(); i++) {
            int to = graph[p][i];
            if (!ret[to].empty()) continue;
            heap.push({lambda[p] + lambda[to], {p, to}});
        }
    }
    return ret;
}