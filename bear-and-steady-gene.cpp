#include <climits>
#include <iostream>
#include <algorithm>
using namespace std;

inline int toIndex(char ch) {
    if (ch <= 'C') {
        if (ch == 'A')
            return 0;
        else
            return 1;
    } else {
        if (ch == 'G')
            return 2;
        else
            return 3;
    }
}

void countGenes(vector< vector<int> > &accumulator, const string &s, int size) {
    ++accumulator[toIndex(s[0])][0];
    for (int i = 1; i < size; ++i) {
        accumulator[0][i] = accumulator[0][i - 1];
        accumulator[1][i] = accumulator[1][i - 1];
        accumulator[2][i] = accumulator[2][i - 1];
        accumulator[3][i] = accumulator[3][i - 1];
        ++accumulator[toIndex(s[i])][i];
    }
}

int main() {
    cin.sync_with_stdio(false);
    cout.sync_with_stdio(false);
    int size;
    cin >> size;
    string s;
    s.reserve(size);
    cin >> s;
    vector< vector<int> > accumulator(4, vector<int>(size, 0));
    countGenes(accumulator, s, size);
    
    int sizePerGene = size / 4;
    vector< pair<int, int> > badCounts;
    for (int i = 0; i < 4; ++i)
        badCounts.push_back(make_pair(accumulator[i][size - 1] - sizePerGene, i));
    sort(badCounts.rbegin(), badCounts.rend());
    auto badIter = lower_bound(badCounts.rbegin(), badCounts.rend(), make_pair(1, 0));
    if (badIter == badCounts.rend()) {
        cout << 0 << endl;
        return 0;
    }
    int numBad = badCounts.rend() - badIter;
    
    // what we need is the smallest range i,j such that accum[n][j] - accum[n][i] >= badCounts[n] for each bad n
    // to do that, for each i we must find the smallest j with this property for each bad gene
    // the shortest sequence starting from i that has the desired property is the max of these
    // the shortest sequence in all is the minimum of that
    
    int minimumLength = INT_MAX;
    for (int i = 0; ; ++i) {
        int thisLength = 0;
        for (int badGene = 0; badGene < numBad; ++badGene) {
            int minBads = badCounts[badGene].first;
            int geneID = badCounts[badGene].second;
            auto iter = accumulator[geneID].begin() + i;
            auto subStrEnd = lower_bound(iter + minBads, accumulator[geneID].end(), accumulator[geneID][i] + minBads);
            if (subStrEnd == accumulator[geneID].end())
                goto exitloop;
            thisLength = max(thisLength, (int)distance(iter, subStrEnd));
        }
        minimumLength = min(thisLength, minimumLength);
    }
    exitloop:
    
    cout << minimumLength << endl;
    
    return 0;
}
