#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
#include <array>
using namespace std;

typedef unsigned long long ull;

class Student {
    static const int SIZE = 8; // 8 * 64 = 512
    array<ull, SIZE> topics;
    
public:
    Student() {
        topics.fill(0ull);
    }
    
    void ReadTopics() {
        string s;
        cin >> s;
        int length = s.length();
        int number = 0;
        int bit = 0;
        
        for (int i = 0; i < length; ++i) {
            if (s[i] == '0' || s[i] == '1') {
                topics[number] <<= 1;
                topics[number] += s[i] - '0';
                if (++bit >= 64) {
                    ++number;
                    bit = 0;
                }
            } else {
                break;
            }
        }
    }
    
    int NumberTopics() {
        int sum = 0;
        for (auto number : topics) {
            sum += __builtin_popcountll(number);
        }
        return sum;
    }
    
    int CombinedTopics(const Student &otherStudent) {
        Student combined;
        for (int i = 0; i < SIZE; ++i) {
            combined.topics[i] = topics[i] | otherStudent.topics[i];
        }
        return combined.NumberTopics();
    }
};

int main() {
    int numStudents, numTopics;
    cin >> numStudents >> numTopics;
    vector<Student> students(numStudents);
    
    for (int i = 0; i < numStudents; ++i) {
        students[i].ReadTopics();
        // cout << students[i].NumberTopics() << endl;
    }
    
    int max = -1, countMax = 0;
    for (int i = 0; i < numStudents; ++i) {
        for (int j = i + 1; j < numStudents; ++j) {
            int combined = students[i].CombinedTopics(students[j]);
            if (combined > max) {
                max = combined;
                countMax = 1;
            } else if (combined == max) {
                ++countMax;
            }
        }
    }
    
    cout << max << endl;
    cout << countMax << endl;
    
    return 0;
}
