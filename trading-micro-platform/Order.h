class Order {

    uint _currentQuantity;
    uint _initialQuantity;
    float _price;
    bool _buy;

public:
    Order(uint currentQuantity, uint initialQuantity, float price, bool buy);

    uint getCurrentQuantity();
    uint getInitialQuantity();
    float getPrice();
    bool getType();

    void updateQuantity(uint quantityDelta);
}