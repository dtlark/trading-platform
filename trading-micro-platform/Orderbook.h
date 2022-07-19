#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <list>
#include <unordered_map>
#include "Order.h"

using namespace std;

/*
Rules:
    - Buy XOR Sell: Firm cannot have both a buy and sell order within each asset(symbol). 
    - In a symbol, the highest buy is matched with the lowest sell assuming the buy price is >= the sell price. Sold for sell price.
*/

inline bool descending(const tuple<float, uint16_t>& a, const tuple<float, uint16_t>& b) {
    return (get<1>(a) < get<1>(b));
}

inline bool ascending(const tuple<float, uint16_t>& a, const tuple<float, uint16_t>& b) {
    return (get<1>(a) > get<1>(b));
}

class Orderbook {
    struct Orders {
        std::unordered_map<uint16_t, char> firms;
        std::list<std::tuple<float, uint16_t>> buys; // sorted by float
        std::list<std::tuple<float, uint16_t>> sells;
    };
    std::unordered_map<std::string, Orders> symbols; // O(1) find

    struct FirmInfo {
        int open_orders = 0;
        int filled_orders = 0;
        float net_funds = 0.f;
    };
    std::unordered_map<uint16_t, FirmInfo> info; // O(1) find

public:

    Orderbook() {}

    inline void NewOrder(uint16_t firmId, std::string &symbol, char side, float price) {

        Orders &o = symbols[symbol]; // does not create entry in map - why?

        if (o.firms.find(firmId) == o.firms.end()) {  // if firm NOT found
            ++info[firmId].open_orders; // add to firm's open orders
            
            if(side == 'B') {
                match(o, firmId, price, o.sells);  // attempt to match
                
                // add to buy list
                o.buys.push_back(std::make_tuple(price, firmId));
                o.firms[firmId] = 'B';
                o.buys.sort(descending);  // nearing O(n)
            }
            else if(side == 'S') {
                match(o, firmId, price, o.buys); // attempt to match
                
                // add to sell list
                o.sells.push_back(std::make_tuple(price, firmId));
                o.firms[firmId] = 'S';
                o.sells.sort(ascending); // nearing O(n)
            }
        }
    }
    inline void ModifyOrder(uint16_t firmId, std::string &symbol, float price) {
    
        Orders &o = symbols[symbol];

        if (o.firms.find(firmId) != o.firms.end()) { // if order IS found
            if (o.firms[firmId] == 'B') {
                for (auto& tup : o.buys) { // BINARY SEARCH
                    if (std::get<1>(tup) == firmId) {
                        std::get<0>(tup) = price;
                        break;
                    };
                }
                match(o, firmId, price, o.sells); // attempt to match
            }
            else if(o.firms[firmId] == 'S') {
                for (auto& tup : o.sells) { // BINARY SEARCH
                    if (std::get<1>(tup) == firmId) {
                        std::get<0>(tup) = price;
                        break;
                    };
                }
                match(o, firmId, price, o.buys);   // attempt to match
            }
        }
    }
    inline void CancelOrder(uint16_t firmId, std::string &symbol) { 

        Orders &o = symbols[symbol];

        if (o.firms.find(firmId) != o.firms.end()) { // if order IS found
            if (o.firms[firmId] == 'B') {
                for (auto& tup : o.buys) {  
                    if (std::get<1>(tup) == firmId) {
                        o.buys.remove(tup);
                        info[firmId].open_orders--;
                        o.firms.erase(firmId);
                        break;
                    }
                }
            }
            else if(o.firms[firmId] == 'S') { 
                for (auto& tup : o.sells) { 
                    if (std::get<1>(tup) == firmId) {
                        o.buys.remove(tup);
                        info[firmId].open_orders--;
                        o.firms.erase(firmId); 
                        break;
                    }
                }
            }
        }
    }
    inline void print() {
        for (auto const &pair: info) {
            std::cout << pair.first << " " << pair.second.open_orders
            << " " << pair.second.filled_orders << " " << pair.second.net_funds << std::endl;
        }
    }
    inline bool isFillable(float buyPrice, float sellPrice, uint16_t buyFirm, uint16_t sellFirm) {
        return buyPrice >= sellPrice && buyFirm != sellFirm;
    }
    inline void fill(std::list<std::tuple<float, uint16_t>> &side, uint16_t buyFirm, uint16_t sellFirm, float price, std::unordered_map<uint16_t, char> &firms) {
        side.pop_front();  
        updateInfo(buyFirm, sellFirm, price); 
        firms.erase(buyFirm);
    }
    inline void updateInfo(uint16_t buyer, uint16_t seller, float sellPrice) {
        --info[seller].open_orders;
        ++info[seller].filled_orders;
        info[seller].net_funds += sellPrice;
        
        --info[buyer].open_orders;
        ++info[buyer].filled_orders;
        info[buyer].net_funds -= sellPrice;
    }
    inline void match(Orders &o, uint16_t firmId, float price, std::list<std::tuple<float, uint16_t>> &side) {

        if (!side.empty()) {
            float xPrice = std::get<0>(side.front());
            float xFirm = std::get<1>(side.front());
                    
            if(isFillable(price, xPrice, firmId, xFirm)) {
                fill(side, firmId, xFirm, xPrice, o.firms);
            }  
        }
    }
};