#include <map>
#include <string>
#include <vector>
#include <list>
#include <map>
#include "Order.h"

using namespace std;

bool descending(const tuple<float, uint16_t>& a, const tuple<float, uint16_t>& b) {
    return (get<1>(a) < get<1>(b));
}

bool ascending(const tuple<float, uint16_t>& a, const tuple<float, uint16_t>& b) {
    return (get<1>(a) > get<1>(b));
}

class Orderbook {
    struct Orders
    {
        std::map<uint16_t, char> firms;
        std::list<std::tuple<float, uint16_t>> buys;
        std::list<std::tuple<float, uint16_t>> sells;
    };

    std::map<std::string, Orders> symbols;
    std::map<uint16_t, std::tuple<int, int, float>> info;

public:

    Orderbook() {

    }

    void NewOrder(uint16_t firmId, std::string symbol, char side, float price) {
        if (!symbols[symbol].firms.count(firmId)) {
            std::get<0>(info[firmId])++; 
            
            if(side == 'B') {
                if (!symbols[symbol].sells.empty())  {
                    float sellPrice = std::get<0>(symbols[symbol].sells.front());
                    float sellFirm = std::get<1>(symbols[symbol].sells.front());
           
                    if(isFillable(price, sellPrice,  firmId, sellFirm)) {
                        fill(symbols[symbol].sells, firmId, sellFirm, sellPrice, symbols[symbol].firms);
                        return;
                    }  
                } 
                //add to buy list
                symbols[symbol].buys.push_back(std::make_tuple(price, firmId));
                symbols[symbol].firms[firmId] = 'B';
                symbols[symbol].buys.sort(descending);
            }
            else if(side == 'S') {
                if (!symbols[symbol].buys.empty()) {
                    float buyPrice = std::get<0>(symbols[symbol].buys.front());
                    float buyFirm = std::get<1>(symbols[symbol].buys.front());
                    
                    if(isFillable(buyPrice, price,buyFirm, firmId)) {
                        fill(symbols[symbol].buys, buyFirm, firmId, price, symbols[symbol].firms);
                        return;
                    }
                } 
                //add to sell list
                symbols[symbol].sells.push_back(std::make_tuple(price, firmId));
                symbols[symbol].firms[firmId] = 'S';
                symbols[symbol].sells.sort(ascending);
            }
        }
    }
    void ModifyOrder(uint16_t firmId, std::string symbol, float price) {
    
        if (symbols[symbol].firms.count(firmId)) {
            if (symbols[symbol].firms[firmId] == 'B') {
                for (auto& tup : symbols[symbol].buys) { // BINARY SEARCH
                    if (std::get<1>(tup) == firmId) {
                        std::get<0>(tup) = price;
                        break;
                    };
                }
                
                if (!symbols[symbol].sells.empty()) {
                    float sellPrice = std::get<0>(symbols[symbol].sells.front());
                    float sellFirm = std::get<1>(symbols[symbol].sells.front());
                    
                    if(isFillable(price, sellPrice, firmId, sellFirm)) {
                        fill(symbols[symbol].sells, firmId, sellFirm, sellPrice, symbols[symbol].firms);
                        return;
                    }  
                } 
            }
            else if(symbols[symbol].firms[firmId] == 'S') {
                for (auto& tup : symbols[symbol].sells) { // BINARY SEARCH
                    if (std::get<1>(tup) == firmId) {
                        std::get<0>(tup) = price;
                        break;
                    };
                }
                if (!symbols[symbol].buys.empty()) {
                    float buyPrice = std::get<0>(symbols[symbol].buys.front());
                    float buyFirm = std::get<1>(symbols[symbol].buys.front());
                    
                    if(isFillable(buyPrice, price, buyFirm, firmId)) {
                        fill(symbols[symbol].buys, buyFirm, firmId, price, symbols[symbol].firms);
                        return;
                    }
                } 
            }
        }
    }
    void CancelOrder(uint16_t firmId, std::string symbol) {        
        if (symbols[symbol].firms.count(firmId)) {
            if (symbols[symbol].firms[firmId] == 'B') {
                for (auto& tup : symbols[symbol].buys) { // BINARY SEARCH
                    
                    if (std::get<1>(tup) == firmId) {
                        symbols[symbol].buys.remove(tup);
                        std::get<0>(info[firmId])--;
                        symbols[symbol].firms.erase(firmId);
                        break;
                    };
                }
            }
            else if(symbols[symbol].firms[firmId] == 'S') { // BINARY SEARCH
                for (auto& tup : symbols[symbol].sells) {
                    if (std::get<1>(tup) == firmId) {
                        symbols[symbol].buys.remove(tup);
                        std::get<0>(info[firmId])--;
                        symbols[symbol].firms.erase(firmId); 
                        break;
                    };
                }
            }
        }
    
    }
    void print() {
        for (auto const &pair: info) {
            std::cout << pair.first << " " << std::get<0>(pair.second) 
            << " " << std::get<1>(pair.second) << " " << std::get<2>(pair.second) << std::endl;
        }
    }
    bool isFillable(float buyPrice, float sellPrice, uint16_t buyFirm, uint16_t sellFirm) {
        return buyPrice >= sellPrice && buyFirm != sellFirm;
    }
    void fill(std::list<std::tuple<float, uint16_t>>& buys, uint16_t buyFirm, uint16_t sellFirm, float price, std::map<uint16_t, char>& firms) {
        buys.pop_front();  
        updateInfo(buyFirm, sellFirm, price); 
        firms.erase(buyFirm);
    }
    void updateInfo(uint16_t buyer, uint16_t seller, float sellPrice) {
        std::get<0>(info[seller])--;
        std::get<1>(info[seller])++;
        std::get<2>(info[seller]) += sellPrice;
        
        std::get<0>(info[buyer])--;
        std::get<1>(info[buyer])++;
        std::get<2>(info[buyer]) -= sellPrice;
    }
};