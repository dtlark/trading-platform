#include <map>
#include <string>
#include <vector>

using namespace std;

class Orderbook {
    map<string, vector<string>> orders;
    vector<string> bids;
    vector<string> asks;

    Order() {
        
    }

    void MarketOrder() {};
    void LimitOrder() {};
    void CancelOrder() {};
    void OrderStatus();

}