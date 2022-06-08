#include <chrono>

class Order {

    int id;
    bool _side;
    float _price;
    uint _currentQuantity;
    uint _initialQuantity;
    chrono::system_clock::time_point time;

public:
    Order(uint currentQuantity, uint initialQuantity, float price, bool side) {
        time = chrono::system_clock::now();
    };

    uint getCurrentQuantity();
    uint getInitialQuantity();
    float getPrice();
    bool getSide();

    void updateQuantity(uint quantityDelta);
}