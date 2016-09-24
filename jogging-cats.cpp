#include <climits>
#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
#include <set>
#include <list>
#include <tuple>
using namespace std;

#define INF INT_MAX
typedef vector< vector< int > > Graph;
typedef vector< int > Neighbors;

void readGraph(Graph &graph, Neighbors &neighbors, int roads) {
    int x, y;
    for (int i = 0; i < roads; ++i) {
        cin >> x >> y;
        --x, --y;
        graph[x].push_back(y);
        graph[y].push_back(x);
        ++neighbors[x];
        ++neighbors[y];
    }
    for (auto &node : graph) {
        sort(node.begin(), node.end());
    }
}

int isConnected(const Neighbors &neighbors, int min) {
    int index = -1;
    int size = neighbors.size();
    for (int i = min; i < size; ++i) {
        if (neighbors[i]) {
            index = i;
            break;
        }
    }
    return index;
}

void eliminateConnection(Graph &graph, Neighbors &neighbors, int start, int end) {
    auto endIndexIter = lower_bound(graph[end].begin(), graph[end].end(), start);
    auto startIndexIter = lower_bound(graph[start].begin(), graph[start].end(), end);
    graph[end].erase(endIndexIter);
    graph[start].erase(startIndexIter);
    --neighbors[start];
    --neighbors[end];
}

int firstLoner(const Neighbors &neighbors, int min) {
    int size = neighbors.size();
    for (int i = min; i < size; ++i)
        if (neighbors[i] == 1)
            return i;
    return -1;
}

void eliminateLoners(Graph &graph, Neighbors &neighbors) {
    int loner = 0, onlyfriend;
    while ((loner = firstLoner(neighbors, loner)) != -1) {
        onlyfriend = graph[loner][0];
        eliminateConnection(graph, neighbors, loner, onlyfriend);
        while (neighbors[onlyfriend] == 1) {
            int otherfriend = graph[onlyfriend][0];
            eliminateConnection(graph, neighbors, onlyfriend, otherfriend);
            onlyfriend = otherfriend;
        }
    }
}

void eliminateIndex(Graph &graph, Neighbors &neighbors, int index) {
    vector<int> hitList;
    for (int dest : graph[index]) 
        hitList.push_back(dest);
    for (int dest : hitList)
        eliminateConnection(graph, neighbors, index, dest);
}

/* n - 1         \
   n - 2
   n - 3
   ...               total n
   n - (n - 2)
   n - (n - 1)
   n - n         /
   
   additions = n * n
   subtractions = n * (n + 1) / 2
   total = n * n - n * (n + 1) / 2 = n^2 - (n^2 + n) / 2 = n^2 * (1 - 1/2) - n * (1/2) = (n^2 - n) / 2
   total = n * (n - 1) / 2   ... sum from 0 to n - 1
*/

int countUniqueFourCycles(const Graph &graph, int index) {
    int sum = 0;
    vector<int> waysToMiddle(graph.size(), 0);
    vector<int> middles;
    middles.reserve(32);
    
    for (int a : graph[index]) {
        for (int b : graph[a]) {
            if (b == index)
                continue;
            if (waysToMiddle[b]) { // we've been here
                ++waysToMiddle[b];    
            } else {
                middles.push_back(b);
                ++waysToMiddle[b];
            }
        }
    }
    
    int numMiddles = middles.size();
    for (int i = 0; i < numMiddles; ++i) {
        int n = waysToMiddle[middles[i]];
        sum += ((n * (n - 1)) >> 1);
    }
    
    return sum;
}

int main() {
    int verts, roads;
    int sum = 0;
    int index = 0;
    cin >> verts >> roads;
    
    vector<int> dummyVector;
    dummyVector.reserve(16);
    Graph graph(verts, dummyVector);
    Neighbors neighbors(verts, 0);
    
    readGraph(graph, neighbors, roads);    
    eliminateLoners(graph, neighbors);
    
    while ((index = isConnected(neighbors, index)) != -1) {
        sum += countUniqueFourCycles(graph, index);
        eliminateIndex(graph, neighbors, index);
        eliminateLoners(graph, neighbors);
    }
    
    cout << sum << endl;
    
    return 0;
}
