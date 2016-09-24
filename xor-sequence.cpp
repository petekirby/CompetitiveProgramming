#include <cstdio>
#include <cassert>
using namespace std;

typedef unsigned long long ull;

inline ull firstorder(ull x) {
    ull ans = 0;
    int mod = int(x) & 3; // mod 4
    
    if (mod == 0) {
        ans = x; // finding the count part of the answer
        ans >>= 1; // divided by two, since count slowed by half
        ans <<= 1; // room made for the last bit (0)
    } else if (mod == 2) {
        ans = x; // finding the count part of the answer (shifting redundant)
        ans |= 1; // last bit set
    } else if (mod == 1) {
        ++ans; // ans = 1
    } // else (if mod == 3), ans = 0
    
    return ans;
}

inline ull secondorder(ull x) {
    ull ans = 0;
    int mod = int(x) & 7; // mod 8
    
    if (mod & 2) { // mod = 2, 3, 6, or 7
        if (mod <= 3) { // mod = 2 or 3
            ans = 2; // binary 10 if 2 or 3, binary 00 if 6 or 7
        }
    } else { // mod 0, 1, 4, or 5
        ans = x;
        ans >>= 2;
        ans <<= 2;
        if (mod == 1) {
            ans |= 1;
        } else if (mod == 4) {
            ans |= 2;
        } else if (mod == 5) {
            ans |= 3;
        }
    }
    
    return ans;
}

int main() {  
    int testCases;
    ull lower, upper, answer;
    assert(scanf("%d", &testCases) == 1);
    while (testCases--) {
        assert(scanf("%llu %llu", &lower, &upper) == 2);
        answer = firstorder(lower) ^ secondorder(lower) ^ secondorder(upper);
        printf("%llu\n", answer);
    }
    return 0;
}


/*
  My Discussion Comment:
The problem requires you to find a solution that will not loop over any array. That will take too long,
as the range can be as large as 10^15, so it's not really an option. At the same time, it's practically
impossible to see the answer/pattern just on hearing the problem (unless you're a true genius). What I
suggest is that you print out the binary representation of the numbers from 0 to 127, alongside the binary
representation of the XOR from 0 to N with N from 0 to 127, alongside the binary representation of the XOR
of the previous numbers from index 0 to N with N going from 0 to 127. This gives you a nice side-by-side
table to look at. What I finally also did was to put one last column on that table to show what it looks
like to XOR from 2 to N instead of from 0, to see the effect of changing the lower bound. This allowed me
to test hypotheses just by staring at the binary. When I coded a solution, I simply added another three
columns and made sure that they matched exactly from 0 to 127. At that point it was just to remove all
the test code, read the input from the input stream, output the answer, and compile.

Here is that code:

    vector<int> arr;
    int ans = 0, weirdo = 0, secondweirdo = 0;
    for (int i = 1; i < 128; ++i) {
        ans ^= i;
        weirdo ^= ans;
        arr.push_back(i);
        cout << setw(7) << bitset<7>(i) << "  " << setw(7) << bitset<7>(ans) << "  " << setw(7) << bitset<7>(weirdo) << "  " << bitset<7>(secondweirdo);
        cout << "       " << setw(7) << bitset<7>(firstorder(i)) << "  " << setw(7) << bitset<7>(secondorder(i)) << "  " << bitset<7>(firstorder(2) ^ secondorder(2) ^ secondorder(i)) << endl;
        secondweirdo ^= ans ^ (i + 1);
    }
    return 0;
*/
