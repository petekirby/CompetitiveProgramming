#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
#include <tuple>
using namespace std;

pair<int, int> getSrc(int rows, int cols, int spins, int i, int j) {
    pair<int, int> answer;
    int loopTopRow = min(i, rows - 1 - i);
    int loopLeftColumn = min(j, cols - 1 - j);
    loopTopRow = min(loopTopRow, loopLeftColumn);
    loopLeftColumn = min(loopTopRow, loopLeftColumn);
    int loopBottomRow = rows - 1 - loopTopRow;
    int loopRightColumn = cols - 1 - loopLeftColumn;
    
    int loopHeight = loopBottomRow - loopTopRow + 1;
    int loopWidth = loopRightColumn - loopLeftColumn + 1;
    
    int loopLength = (loopHeight > 1) ? (loopWidth << 1) : loopWidth;
    if (loopHeight > 2)
        loopLength += (loopWidth > 1) ? ((loopHeight - 2) << 1) : loopHeight -2;
    
    int startPart, loopPosition;
    if (i == loopTopRow) {
        loopPosition = j - loopLeftColumn;
        startPart = 1;
    } else {
        if (j == loopRightColumn) {
            loopPosition = loopWidth - 1 + i - loopTopRow;
            startPart = 2;
        } else {
            if (i == loopBottomRow) {
                loopPosition = loopWidth - 1 + loopHeight - 1 + loopRightColumn - j;
                startPart = 3;
            } else { // if j == loopLeftColumn
                loopPosition = loopLength - (i - loopTopRow);
                startPart = 4;
            }
        }
    }
        
    int endPart, srcLoopPosition = (loopPosition + spins) % loopLength;
        
    if (srcLoopPosition < loopWidth) {
        answer.first = loopTopRow;
        answer.second = loopLeftColumn + srcLoopPosition;
        endPart = 1;
    } else if (srcLoopPosition - loopWidth + 1 < loopHeight) {
        answer.first = loopTopRow + srcLoopPosition - loopWidth + 1;
        answer.second = loopRightColumn;
        endPart = 2;
    } else if (srcLoopPosition - loopWidth - loopHeight + 2 < loopWidth) { // not sure
        answer.first = loopBottomRow;
        answer.second = loopRightColumn - (srcLoopPosition - loopWidth - loopHeight + 2);
        endPart = 3;
    } else {
        answer.first = loopTopRow + (loopLength - srcLoopPosition);
        answer.second = loopLeftColumn;
        endPart = 4;
    }
    
    /*
    cout << "row: " << i << " col: " << j << " spins: " << spins << " src: " << answer.first << ", " << answer.second;
    cout << " top/bot: " << loopTopRow << ", " << loopBottomRow << " left/right: " << loopLeftColumn << ", " << loopRightColumn;
    cout << " startPos: " << loopPosition << " endPos: " << srcLoopPosition << " length " << loopLength;
    cout << " width " << loopWidth << " height " << loopHeight;
    cout << " startPart: " << startPart << " endPart " << endPart << endl;
    */

    return answer;
}

int main() {
    int rows, cols, spins;
    cin >> rows >> cols >> spins;
    
    int a[rows][cols];
    // int b[rows][cols];
    
    for (int i = 0; i < rows; ++i)
        for (int j = 0; j < cols; ++j)
            cin >> a[i][j];
        
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            pair<int, int> src = getSrc(rows, cols, spins, i, j);
            cout << a[src.first][src.second] << " ";
        }
        cout << endl;
    }
    
    return 0;
}
