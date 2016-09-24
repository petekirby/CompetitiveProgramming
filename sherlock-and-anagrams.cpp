#include <iostream>
#include <iterator>
#include <algorithm>
#include <array>
using namespace std;

typedef array<int, 26> Alphabet;

struct Letters {
    Alphabet a;
    inline Alphabet::iterator begin() {return a.begin();}
    inline Alphabet::iterator end() {return a.end();}
    int operator [](int i) const {return a[i];}
    int & operator [](int i) {return a[i];}
};

bool operator<(const Letters& a, const Letters& b) {
    for (int i = 0; i < 26; ++i) {
        if (a[i] < b[i])
            return true;
        else if (a[i] > b[i])
            return false;
    }
    return false;
}

bool operator==(const Letters& a, const Letters& b) {
    for (int i = 0; i < 26; ++i)
        if (a[i] != b[i])
            return false;
    return true;
}

inline int toIndex(const char &ch) {
    return ch - 'a';
}

void countLetters(vector< vector<int> > &accumulator, const string &s, int size) {
    ++accumulator[toIndex(s[0])][0];
    for (int i = 1; i < size; ++i) {
        for (int j = 0; j < 26; ++j)
        accumulator[j][i] = accumulator[j][i - 1];
        ++accumulator[toIndex(s[i])][i];
    }
}

void countSubstrLetters(vector< Letters > &letters, const vector< vector<int> > &accumulator, const vector< pair<int, int> > &indexes, int size, int numStrs) {
    for (int i = 0; i < size; ++i)
        for (int j = 0; j < 26; ++j)
            letters[i][j] = accumulator[j][indexes[i].second];
    for (int i = size; i < numStrs; ++i)
        for (int j = 0; j < 26; ++j)
            letters[i][j] = accumulator[j][indexes[i].second] - accumulator[j][indexes[i].first - 1];
}

int main() {
    int testCases;
    cin >> testCases;
    while (testCases--) {
        string s;
        cin >> s;
        int size = s.length();
        vector< vector<int> > accumulator(26, vector<int>(size, 0));
        countLetters(accumulator, s, size);
        int numStrs = (size * ( size + 1 )) / 2;
        vector< Letters > letters(numStrs);
        vector< pair<int, int> > indexes; indexes.reserve(numStrs);
        for (int i = 0; i < size; ++i)
            for (int j = i; j < size; ++j)
                indexes.push_back(make_pair(i, j));
        countSubstrLetters(letters, accumulator, indexes, size, numStrs);
        sort(letters.begin(), letters.end());
        vector<int> repeats;
        for (int i = 1, repeater = 0; i < numStrs; ++i) {
            if (letters[i] == letters[i - 1]) {
                repeats.push_back(2);
                while (++i < numStrs && letters[i] == letters[i - 1]) {
                    ++repeats[repeater];
                }
                ++repeater;
            }
        }
        unsigned long long sum = 0;
        for (int reps : repeats)
            sum += (reps * (reps - 1));
        sum /= 2;
        cout << sum << endl;
    }
    return 0;
}

        /* for (int i = 0; i < numStrs; ++i) {
            cout << "substr{" << indexes[i].first << ", " << indexes[i].second << "}: ";
            copy(letters[i].begin(), letters[i].end(), ostream_iterator<int>(cout, " ")); cout << endl;
        } */
