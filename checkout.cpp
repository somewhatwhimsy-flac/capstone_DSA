#include "checkout.h"
#include "inventory.h"
#include <iomanip>
#include <iostream>

using namespace std;

CartNode::CartNode(int id, int amount)
    : productId(id), quantity(amount), next(nullptr) {}

Cart::Cart() : head(nullptr) {}

Cart::~Cart() {
    clear();
}

void Cart::add(int id, int quantity) {
    CartNode* current = head;
    while (current) {
        if (current->productId == id) {
            current->quantity += quantity;
            return;
        }
        current = current->next;
    }

    CartNode* node = new CartNode(id, quantity);
    if (!head) {
        head = node;
    } else {
        current = head;
        while (current->next) current = current->next;
        current->next = node;
    }
}

bool Cart::empty() const {
    return head == nullptr;
}

void Cart::display(const Inventory& inventory) const {
    if (!head) {
        cout << "\nShopping cart is empty.\n";
        return;
    }

    double total = 0;
    cout << "\nYOUR SHOPPING CART\n";
    for (CartNode* node = head; node; node = node->next) {
        int index = inventory.findIndexById(node->productId);
        if (index == -1) continue;

        const Product& product = inventory.at(index);
        double subtotal = product.price * node->quantity;
        cout << Inventory::colorCodeForBrand(product.brand)
             << product.name << " x " << node->quantity << " = P"
             << fixed << setprecision(2) << subtotal
             << Inventory::colorResetCode() << "\n";
        total += subtotal;
    }
    cout << "Cart total: P" << fixed << setprecision(2) << total << "\n";
}

void Cart::transferToQueue(queue<CartNode*>& checkoutQueue) {
    CartNode* current = head;
    while (current) {
        CartNode* next = current->next;
        current->next = nullptr;
        checkoutQueue.push(current);
        current = next;
    }
    head = nullptr;
}

bool Cart::checkout(Inventory& inventory, queue<CartNode*>& checkoutQueue) {
    if (!head) {
        cout << "Cart is empty.\n";
        return false;
    }

    for (CartNode* node = head; node; node = node->next) {
        int index = inventory.findIndexById(node->productId);
        if (index == -1 || node->quantity > inventory.at(index).stockQuantity) {
            cout << "Checkout failed: insufficient stock for product ID "
                      << node->productId << ". No stock was changed.\n";
            return false;
        }
    }

    transferToQueue(checkoutQueue);
    double total = 0.0;
    cout << "\nCHECKOUT - FIFO PROCESSING ORDER\n";

    while (!checkoutQueue.empty()) {
        CartNode* order = checkoutQueue.front();
        checkoutQueue.pop();

        int index = inventory.findIndexById(order->productId);
        Product& product = inventory.at(index);
        double subtotal = product.price * order->quantity;
        product.stockQuantity -= order->quantity;
        total += subtotal;

        cout << "Processed: " << Inventory::colorCodeForBrand(product.brand)
             << product.name << " x " << order->quantity
             << " = P" << fixed << setprecision(2) << subtotal
             << " | Remaining stock: " << product.stockQuantity
             << Inventory::colorResetCode() << "\n";
        delete order;
    }

    cout << "CHECKOUT COMPLETE. Total: P" << fixed << setprecision(2)
              << total << "\n";
    return true;
}

void Cart::clear() {
    while (head) {
        CartNode* temp = head;
        head = head->next;
        delete temp;
    }
}