#include <map>
#include <string>
#include <vector>
#include "Order.h"

using namespace std;

class Orderbook {
    map<string, vector<string>> orders;
    vector<string> bids;
    vector<string> asks;

    Orderbook() {
        
    }

    void MarketOrder() {};
    void LimitOrder() {};
    void CancelOrder() {};
    void OrderStatus();

};