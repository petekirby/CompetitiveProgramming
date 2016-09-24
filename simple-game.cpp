#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
#include <array>
using namespace std;

typedef vector<int> HeapSizes; // keep track of each heap's size
typedef vector< HeapSizes > Starts; // keeps track of winning starts (or possible divisions into piles)
typedef vector<int> NimValues; // nim-value corresponding to each heap size value (calculated once)
typedef vector<int> Nextnims; // nim-sum for each next move

// diagnostic print
void printStarts(const Starts &starts) {
    for (const auto &start : starts) {
        cout << "{ ";
        for (int x : start) {
            cout << x << " ";
        }
        cout << "}" << endl;
    }
}

inline int findNimSum(const HeapSizes &heapsizes, const NimValues &nimvalues) {
    int answer = 0;
    for (const int& size : heapsizes)
        answer ^= nimvalues[size];
    return answer;
}

int findMex(Nextnims &nextnims) {
    if (nextnims.empty())
        return 0;
    sort(nextnims.begin(), nextnims.end());
    unique(nextnims.begin(), nextnims.end());
    if (nextnims[0] /* != 0 */)
        return 0;
    
    int left = 0;
    int last = nextnims.size() - 1;
    int right = last;
    int result = -1;
    
    while (left <= right) {
        int mid = left + ((right - left) >> 1); // = l + (r - l)/2
        int value = nextnims[mid];
        
        // Check if this is the last value in its rightful place
        if (value == mid) {
            if (mid == last || nextnims[mid + 1] != mid + 1) {
                result = mid + 1; // mex is one past the last one in right place
                break;
            } else {
                left = mid + 1;
            }
        } else {
            right = mid - 1;
        }
    }

    return result;
}

void recursivelyFillHeapSizes(Starts &vectorVector, HeapSizes &currentPiles, int dispensingPile, int lastPile) {
    vectorVector.push_back(currentPiles);
    if (dispensingPile == lastPile)
        return;
    int nextPile = dispensingPile + 1;
    while (nextPile <= lastPile && currentPiles[nextPile] <= currentPiles[dispensingPile] - 1) {
        if (currentPiles[nextPile] == currentPiles[dispensingPile] - 1) {
            ++nextPile;
            continue;
        }
        --currentPiles[dispensingPile];
        ++currentPiles[nextPile];
        recursivelyFillHeapSizes(vectorVector, currentPiles, nextPile, lastPile);
    }
}

void fillVectorOfHeapSizeVectors(Starts &vectorVector, int stones, int numpiles) {
    HeapSizes initialVector(numpiles, 1);
    initialVector[0] += stones - numpiles;
    recursivelyFillHeapSizes(vectorVector, initialVector, 0, numpiles - 1);
}

void findNimValues(NimValues &nimvalues, int numstones, int maxpiles) {
    nimvalues.resize(numstones + 1);
    nimvalues[0] = 0;
    nimvalues[1] = 0;
    
    for (int pileSize = 2; pileSize <= numstones; ++pileSize) {
        Nextnims vectorOfNimSums;
        for (int newPiles = 2; newPiles <= maxpiles; ++newPiles) {
            Starts vectorOfHeapSizeVectors;
            fillVectorOfHeapSizeVectors(vectorOfHeapSizeVectors, pileSize, newPiles);
            for (HeapSizes &move : vectorOfHeapSizeVectors) {
                vectorOfNimSums.push_back(findNimSum(move, nimvalues));
            }
        }
        nimvalues[pileSize] = findMex(vectorOfNimSums);
    }
}

void findWinningStarts(Starts &winningStarts, int numstones, int numpiles, NimValues &nimvalues) {
    Starts allStarts;
    fillVectorOfHeapSizeVectors(allStarts, numstones, numpiles);
    // printStarts(allStarts);
    for (auto &start : allStarts) {
        if(findNimSum(start, nimvalues) /* != 0 */) { // is this right? zero or not zero??
            winningStarts.push_back(start);
        }    
    }
}

int findPermutations(const Starts &starts, int numpiles) {
    static const int FACTORIAL[] = {1, 1, 2, 6, 24, 120, 720, 5040, 40320, 362880, 3628800, 39916800, 479001600};
    static const int BIGNUMBER = 1000000007;
    static const int numpilesFactorial = FACTORIAL[numpiles];
    int answer = 0;
    
    for (auto &start : starts) {
        vector<int> duplicates;
        for (int i = 0; i < numpiles - 1; ++i) {
            int currentNumber = start[i];
            if (start[i + 1] != currentNumber)
                continue;
            int j, repetition = 2;
            for (j = i + 2; j < numpiles && start[j] == currentNumber; ++j) {
                ++repetition;
            }
            i = j - 1; // i will be incremented back to j, which is one past the last "valid" j
            duplicates.push_back(repetition);
        }
        
        int divisor = 1;
        for (int repetition : duplicates) {
            divisor *= FACTORIAL[repetition];
        }
        
        answer += numpilesFactorial / divisor;
        if (answer >= BIGNUMBER)
            answer %= BIGNUMBER;
    }
    
    return answer;
}

int main() {
    ios_base::sync_with_stdio(false);
    int numstones, numpiles, maxsplit;
    cin >> numstones >> numpiles >> maxsplit;
    
    Starts winningStarts;
    NimValues nimvalues;
    
    findNimValues(nimvalues, numstones, maxsplit);
    findWinningStarts(winningStarts, numstones, numpiles, nimvalues);
    
    // cout << "There are " << winningStarts.size() << " winning start combinations." << endl;
    // printStarts(winningStarts);
    
    int permutations = findPermutations(winningStarts, numpiles);
    
    cout << permutations << endl;
    
    return 0;
}
