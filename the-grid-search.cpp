#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
using namespace std;

typedef vector<int> vi;

void readMatrix(vector<vi> &matrix, int rows, int cols) {
    getchar();
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            matrix[i][j] = getchar() - '0';
        }
        getchar();
    }
}

int main() {
    int testCases;
    cin >> testCases;
    while (testCases--) {
        int bigRows, bigCols, smallRows, smallCols;
        cin >> bigRows >> bigCols;
        vector<vi> big(bigRows, vi(bigCols, 0));
        readMatrix(big, bigRows, bigCols);
        cin >> smallRows >> smallCols;
        vector<vi> small(smallRows, vi(smallCols, 0));
        readMatrix(small, smallRows, smallCols);
        
        int firstDigit = small[0][0];
        bool found = false;
        for (int i = 0; i <= bigRows - smallRows; ++i) {
            for (int j = 0; j <= bigCols - smallCols; ++j) {
                if (big[i][j] == firstDigit) {
                    for (int k = 0; k < smallRows; ++k) {
                        for (int l = 0; l < smallCols; ++l) {
                            if (big[i + k][j + l] != small[k][l])
                                goto exitinner;
                        }
                    }
                    found = true;
                    goto exitouter;
                }
                exitinner:
                ;
            }
        }
        exitouter:
        
        if (found)
            cout << "YES" << endl;
        else
            cout << "NO" << endl;
    }
    return 0;
}
