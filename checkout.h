#pragma once
#include <queue>

using namespace std;

class Inventory;

struct CartNode {
    int productId;
    int quantity;
    CartNode* next;

    CartNode(int id, int amount);
};

class Cart {
private:
    CartNode* head;

public:
    Cart();
    ~Cart();

    Cart(const Cart&) = delete;
    Cart& operator=(const Cart&) = delete;

    void add(int id, int quantity);
    bool empty() const;
    void display(const Inventory& inventory) const;
    void transferToQueue(queue<CartNode*>& checkoutQueue);
    bool checkout(Inventory& inventory, queue<CartNode*>& checkoutQueue);
    void clear();
};