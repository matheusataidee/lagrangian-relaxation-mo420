#include <vector>
#include <numeric>
#include <queue>
#include <iostream>

using namespace std;

vector<vector<int> > getHeuristicTree(vector<vector<int> >& graph);

vector<vector<int> > getMSTree(vector<vector<int> >& graph, vector<double>& lambda);