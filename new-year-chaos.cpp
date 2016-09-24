#include <vector>
#include <iostream>
using namespace std;

int bubbleSort (vector<int> &arr)
{
    int sum = 0;
    int n = arr.size();
    for (int i = n - 1; i >= 0; --i) {
        for (int j = max(0, i - 2); j < i; ++j) {
            if (arr[j] > arr[j + 1]) {
                ++sum;
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
    return sum;
}		

int main() {
    int t, n, badge;
    cin >> t;
    while (t--) {
        vector<int> badges;
        bool chaotic = false;
        cin >> n;
        
        for (int i = 1; i <= n; ++i) {
            cin >> badge;
            badges.push_back(badge);
            if (badge - i > 2)
                chaotic = true;
        }
        
        if (chaotic) {
            cout << "Too chaotic" << endl;
        } else {
            cout << bubbleSort(badges) << endl;
        }
    }
    return 0;
}
