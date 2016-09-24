#include <iostream>
#include <queue>
#include <vector>
#include <tuple>
#include <set>
#include <algorithm>
#include <climits>
#include <bitset>
using namespace std;
#define INF INT_MAX

// dijkstra will compute the distance and the fishbits to every other node

// getNextFish will provide a list of nodes to travel from current one to get a new fish (and only 1 new purchase!)
// it will need the tree to eliminate downstream options
// it operates by choosing the nearest new fish first, eliminating downstream options, then so on until done

// divide by 2 (or >> 1) to get the best-case distance for the current solution; abort if greater than upper bound so far

// Data on the solutions:
// the list of nodes with new fish that have been visited
// the distance between each node and the next (or the start)
// the last place and second-to-last place on the longer_leg
// find the meeting point that minimizes the difference of (one_place + its_path) and (other_place + its_path)
// finally (most important!), the maximum of those two values (the solution's total time/distance)
// check against the universal shortest total time/distance and update it as needed

// Data on the current search:
// the list of nodes with new fish that have been visited
// the distance traveled (for aborting when too long)
// implicitly (on the stack), a list of visited fish at each of these nodes\

// Data on the nodes:
// * Whether it is explored or not
// * Minimum distance to all other vertices
// * Fish picked up along the way to those vertices
// * reverse tree info for every node from the initial vertex

const int STARTNODE = 0;
const int MAXVERTS = 1000;
const int MAXFISH = 10;
const int FISHBIT[] = {0x1, 0x2, 0x4, 0x8, 0x10, 0x20, 0x40, 0x80, 0x100, 0x200};
int gAllFish; // = 0x3ff; // depends on input...

struct Edge { int to, length; };

class Node {
public:
    bool isKnown; // pre-initialized (whether visited with non-preinit info)
    int fishMarket; // pre-initialized, bit 0 true = fish 0 sold, etc.
    vector<Edge> edges; // pre-initialized (graph data)
    
    int *minDist;
    int *fishBits;
    int *prevNode;
    
    Node() {
        isKnown = false;
        fishMarket = 0;
        edges.reserve(8);
        minDist = 0;
        fishBits = 0;
        prevNode = 0;
    }
    
    ~Node() {
        if (minDist)
            delete[] minDist;
        if (fishBits)
            delete[] fishBits;
        if (prevNode)
            delete[] prevNode;
    }
};

// diagnostic print functions
void print_graph(const vector<Node> &graph) {
    int graphSize = graph.size();
    for (int i = 0; i < graphSize; ++i) {
        cout << "#" << (i + 1) << "\t" << bitset<10>(graph[i].fishMarket) << endl;
        for (auto edge : graph[i].edges) {
            cout << "#" << (i + 1) << "\t#" << (edge.to + 1) << "\t" << edge.length << endl;
        }
        cout << endl;
    }
}

void print_node(const vector<Node> &graph, int source) {
    const Node& node = graph[source];
    cout << "Node #" << (source + 1) << " Distances" << endl;
    for (int i = 0; i < graph.size(); ++i) {
        cout << "-> #" << (i + 1) << "\tminDist\t" << node.minDist[i] << "\tfishBits\t" <<  bitset<10>(node.fishBits[i]) << endl;
    }
    cout << endl;
}

void print_node_vector(const vector<int> &nodes) {
        for (int j = 0; j < nodes.size(); ++j) {
            cout << "-> #" << (nodes[j] + 1) << endl;
        }
        if (nodes.size() == 0) {
            cout << "empty" << endl;
        }
        cout << endl;
}

void print_next_node(const vector< vector<int> > &nextNode) {
    for (int i = 0; i < nextNode.size(); ++i) {
        cout << "Node #" << (i + 1) << " Next Nodes" << endl;
        print_node_vector(nextNode[i]);
    }
}

void print_fish_list(const vector<int> &fishList, int source, int fish) {
    cout << "Node #" << (source + 1) << " with fish " << bitset<10>(fish) << endl;
    print_node_vector(fishList);
}

void print_path(vector< pair<int, int> > &path) {
    for (int i = 0; i < path.size(); ++i)
        cout << "Node #" << path[i].second + 1 << " with distance " << path[i].first << endl;
    cout << endl;
}

// search functions
int findNextNodes(vector< vector<int> > &nextNode, const Node &node, int graphSize) {
    nextNode.clear();
    nextNode.resize(graphSize);
    for (int i = 0; i < graphSize; ++i) {
        int iPrevNode = node.prevNode[i];
        if (iPrevNode >= 0) {
            nextNode[ iPrevNode ].reserve(4);
            nextNode[ iPrevNode ].push_back(i);
        }
    }
    return 0;
}

int dijkstra(vector< vector<int> > &nextNode, vector<Node> &graph, int source) {
    Node& node = graph[ source ];
    int graphSize = graph.size();
    if (node.isKnown)
        return findNextNodes(nextNode, node, graphSize);
    node.minDist = new int[graphSize];
    fill_n(node.minDist, graphSize, INF);
    node.minDist[ source ] = 0;
    node.fishBits = new int[graphSize]();
    node.prevNode = new int[graphSize];
    fill_n(node.prevNode, graphSize, -2 );
    node.prevNode[source] = -1;
    
    set< pair<int,int> > active_vertices;
    active_vertices.insert( {0,source} );
    while (!active_vertices.empty()) {
        int where = active_vertices.begin()->second;
        active_vertices.erase( active_vertices.begin() );
        for (auto edge : graph[where].edges) 
            if (node.minDist[edge.to] > node.minDist[where] + edge.length) {
                active_vertices.erase( { node.minDist[edge.to], edge.to } );
                node.minDist[edge.to] = node.minDist[where] + edge.length;
                active_vertices.insert( { node.minDist[edge.to], edge.to } );
                node.prevNode[edge.to] = where;
            }
    }
    
    findNextNodes(nextNode, node, graphSize);
    
    // might be able to refactor this with similar looping code
    vector< pair<int, int> > current_vertices; // fishbits, vertex
    current_vertices.push_back( make_pair(node.fishMarket, source) );
    while (!current_vertices.empty()) {
        int where = current_vertices.back().second;
        int fish = current_vertices.back().first;
        node.fishBits[where] = fish;
        current_vertices.pop_back();
        for (auto vert : nextNode[where]) {
            current_vertices.push_back( make_pair(fish | graph[vert].fishMarket, vert) );
        }
    }
    
    node.isKnown = true;
    return 1;
}

void getFishList(vector<int> &fishList, const vector<Node> &graph, const vector< vector<int> > &nextNode, int source, int fish) {
    vector< tuple<int, int, int> > fish_dist; // new fish or not, distance, destination
    int graphSize = graph.size();
    const Node& node = graph[ source ];
    for (int i = 0; i < graphSize; ++i) {
        int isJustOldFish = ((fish | node.fishBits[i]) ^ fish) ? 0 : 1; // aka isZeroIfNewFish
        fish_dist.push_back( make_tuple(isJustOldFish, node.minDist[i], i) );
        // cout << "Just added fishtuple " << isJustOldFish << " " << node.minDist[i] << " " << i << endl;
    }
    
    // sort first by whether it has a new fish (zero first), then by distance, lastly by coincidence of ID
    sort(fish_dist.begin(), fish_dist.end());
    vector<int> fishFinder(graphSize);
    for (int i = 0; i < graphSize; ++i)
        fishFinder[get<2>(fish_dist[i])] = i;
    
    for (auto fishTuple : fish_dist) {
        // cout << "Fishtuple " << get<0>(fishTuple) << " " << get<1>(fishTuple) << " " << get<2>(fishTuple) << endl; 
        if (get<0>(fishTuple)) // either 1 for no new fish or marked 2 for being on another's path in this loop
            continue;          // we don't need this kind of thing
        fishList.push_back(get<2>(fishTuple));
        // cout << "Hurrah we just added " << get<2>(fishTuple) << endl;
        
        // might be able to refactor this with similar looping code
        vector<int> current_vertices;
        current_vertices.push_back(get<2>(fishTuple));
        while (!current_vertices.empty()) {
            int where = current_vertices.back();
            // get<0>(fish_dist[fishFinder[where]]) = 2; // marked for being on another fresh fish node's optimal path
            current_vertices.pop_back();
            for (auto vert : nextNode[where]) {
                current_vertices.push_back( vert );
            }
        }
    }
    // print_fish_list(fishList, source, fish);
    // the fishList is updated for the caller
}

int calcTime(int total, vector< pair<int, int> > &path, vector<Node> &graph) {
    // print_path( path );
    
    if (total == 0)
        return total;
    
    int littleCatPath, bigCatPath, halfTotal = (total >> 1);
    for (int i = 0; /* i < path.size() */ ; ++i)
        if (path[i].first > halfTotal) {
            littleCatPath = i;
            bigCatPath = i - 1;
            break;
        }
    
    if (path[bigCatPath].first == halfTotal) {
        return halfTotal;
    }
    
    int graphSize = graph.size();
    int bigCatDist = path[bigCatPath].first;
    Node &bigCatNode = graph[path[bigCatPath].second];
    int littleCatDist = total - path[littleCatPath].first;
    Node &littleCatNode = graph[path[littleCatPath].second];
    int bigCatTime, littleCatTime;
    int time, minTime = INF;
    
    // cout << "Big cat dist " << bigCatDist << " and little cat dist " << littleCatDist << endl;
    
    if (!(bigCatNode.isKnown && littleCatNode.isKnown)) {
        // cout << "big cat and little cat check" << endl;
        vector< vector<int> > nextNode;
        if (!bigCatNode.isKnown)
            dijkstra(nextNode, graph, path[bigCatPath].second);
        if (!littleCatNode.isKnown)
            dijkstra(nextNode, graph, path[littleCatPath].second);
        // cout << "on the other side of check" << endl;
    }
    
    for (int i = 0; i < graphSize; ++i) {
        bigCatTime = bigCatDist + bigCatNode.minDist[i];
        littleCatTime = littleCatDist + littleCatNode.minDist[i];
        time = (bigCatTime > littleCatTime) ? bigCatTime : littleCatTime;
        minTime = (time < minTime) ? time : minTime;
    }
    
    return minTime;
}

int bestTime(int &best, int curr, int fish, int index, vector< pair<int, int> > &path, vector<Node> &graph) {
    // cout << "bestTime best = " << best << ", curr = " << curr << ", fish = " << bitset<10>(fish) << ", i = " << index+1 << endl;
     
    path.push_back( make_pair(curr, index) );
    
    if (best < (curr >> 1) - (curr >> 3)) {
        ;
    } else if (fish == gAllFish) {
        if (index != STARTNODE) {
            int extraDist = graph[STARTNODE].minDist[index];
            best = bestTime(best, curr + extraDist, fish, STARTNODE, path, graph);
        } else {
            int time = calcTime(curr, path, graph); // find time of this path for two cats
            best = (time < best) ? time : best;
        }
    } else {    
        Node &node = graph[ index ];   
        vector< vector<int> > nextNode;
        vector<int> fishList;
        dijkstra(nextNode, graph, index);
        getFishList(fishList, graph, nextNode, index, fish);
        // print_fish_list(fishList, index, fish); // comment this away

        for (auto fishNode : fishList) {
            int extraDist = node.minDist[fishNode];
            int newFish = fish | node.fishBits[fishNode];
            best = bestTime(best, curr + extraDist, newFish, fishNode, path, graph);
        }
    }
    
    path.pop_back();
    return best;
}

// driver functions
void read_graph(vector<Node> &graph) {
    std::ios::sync_with_stdio(false);
    int iVerts, iRoads, iFish;
    cin >> iVerts >> iRoads >> iFish;
    graph.resize(iVerts);
    
    gAllFish = 0;
    for (int i = 0; i < iFish; ++i) {
        gAllFish |= FISHBIT[i];
    }
    
    for (int i = 0; i < iVerts; ++i) {
        int iMarketFish, fishID;
        cin >> iMarketFish;
        for (int j = 0; j < iMarketFish; ++j) {
            cin >> fishID;
            --fishID;
            graph[i].fishMarket = graph[i].fishMarket  | FISHBIT[fishID];
        }
    }
    
    for (int i = 0; i < iRoads; ++i) {
        int src, dest, dist;
        struct Edge myEdge;
        cin >> src >> dest >> dist;
        --src, --dest;
        myEdge.to = dest;
        myEdge.length = dist;
        graph[src].edges.push_back(myEdge);
        myEdge.to = src;
        // myEdge.length = dist;
        graph[dest].edges.push_back(myEdge);        
    }
}

int main()
{
    vector<Node> graph;
    graph.reserve(MAXVERTS);
    vector< pair<int, int> > path; // total distance so far, node visited
    path.reserve(MAXFISH + 4);
    
    read_graph(graph);
    
    int startNode = STARTNODE; // always start at node 0 (aka #1)
    int startDist = 0; // no time to get to first node
    int best = INF;
    
    best = bestTime(best, startDist, graph[startNode].fishMarket, startNode, path, graph);
    cout << best << endl;
    // cout << "Done!" << endl;

    return 0;
}
