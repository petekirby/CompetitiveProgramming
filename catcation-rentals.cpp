#include <vector>
#include <iostream>
#include <algorithm>
#include <set>
#include <tuple>
#include <iomanip>
#include <string>
using namespace std;

typedef set< pair<int, int> > Bookings;
typedef vector< pair<int, int> > Requests;

int successfulDays(const Requests &requests, int minDays) {
    Bookings checkouts;
    int sumDays = 0;
    
    for (auto &request : requests) {
        int numDays = request.second - request.first + 1;
        if (numDays < minDays)
            continue;
        
        auto checkoutIter = checkouts.lower_bound(make_pair(request.first,0));
        if (checkoutIter != checkouts.end() && checkoutIter->second <= request.second)
            continue;
        
        checkouts.insert(make_pair(request.second, request.first));
        sumDays += numDays;
    }
        
    return sumDays;
}

int main() {
    ios_base::sync_with_stdio(false);
    Requests requests;
    string result;
    int success;
    
    int numrequests, numdays, numqueries;
    cin >> numrequests >> numdays >> numqueries;
    
    requests.reserve(numrequests);
    vector<int> numRequestsOfLength(numrequests, 0);
    vector<int> numSuccessDays(numdays + 1, 0);
    
    // int zeroSuccessAbove = numdays;
    
    for (int x, y, i = 0; i < numrequests; ++i) {
        cin >> x >> y;
        requests.push_back(make_pair(x, y));
        ++numRequestsOfLength[y - x + 1];
    }
    
    numSuccessDays[1] = successfulDays(requests, 1);
    
    for (int i = 2; i <= numdays; ++i) {
        if (numRequestsOfLength[i - 1] == 0) {
            numSuccessDays[i] = numSuccessDays[i - 1];
        } else {
            numSuccessDays[i] = successfulDays(requests, i);
        }
    }
    
    for (int d, i = 0; i < numqueries; ++i) {
        cin >> d;
        
        result.append(to_string(numSuccessDays[d]));
        result.append("\n");
    }
    
    cout << result;
    cout << flush;
    
    return 0;
}
