#include <chrono>

using namespace std;

class Order {

    int _id;
    bool _side;
    float _price;
    uint _currentQuantity;
    uint _initialQuantity;
    chrono::system_clock::time_point time;

public:
    Order(int id, bool side, float price, uint curr, uint ini) : _id(id), _side(side), _price(price), _currentQuantity(curr), _initialQuantity(ini) {
        time = chrono::system_clock::now();
    };

    uint getCurrentQuantity();
    uint getInitialQuantity();
    float getPrice();
    bool getSide();

    void updateQuantity(uint quantityDelta);
};