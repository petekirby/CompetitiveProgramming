#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
#include <iterator>
using namespace std;

inline void swap(vector<int>::iterator pA, vector<int>::iterator pB) {
    int swap = *pA;
    *pA = *pB;
    *pB = swap;
}

vector<int>::iterator partition(vector<int>::iterator pLeft, vector<int>::iterator pRight) {
    int iPivot = *pRight;
    auto pPivot = pLeft;
    
    for (auto pIter = pLeft; pIter != pRight; ++pIter) {
        if (*pIter < iPivot) {
            swap(pIter, pPivot);
            ++pPivot;
        }
    }
    
    swap(pPivot, pRight);
    return pPivot;
}

void quicksort(vector<int> &v, vector<int>::iterator pLeft, vector<int>::iterator pRight) {
    if (pLeft == pRight || pLeft - 1 == pRight) {
        return;
    }
    
    auto pPivot = partition(pLeft, pRight);
    copy(v.begin(), v.end(), ostream_iterator<int>(cout, " "));
    cout << endl;
    
    quicksort(v, pLeft, pPivot - 1);
    quicksort(v, pPivot + 1, pRight);
}    

int main() {
    int n, iInput;
    cin >> n;
    vector<int> v;
    for (int i = 0; i < n; ++i) {
        cin >> iInput;
        v.push_back(iInput);
    }
    
    quicksort(v, v.begin(), --v.end());
    
    return 0;
}
