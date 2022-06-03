#include <Order.h>


Order::Order(uint quantity, float price, bool buy) {

    _qunatity = quantity;
    _price = price;
    _buy = buy;
    
};

uint Order::getCurrentQuantity() {
    return _currentQunatity;
}

uint Order::getInitialQuantity() {
    return _initialQunatity;
}

float Order::getPrice() {
    return _price;
}

bool Order::getType() {
    return _buy;
}

void Order::updateQuantity(uint quantityDelta) {

    currentQuantity += quantityDelta;

}