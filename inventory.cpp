#include "inventory.h"
#include <algorithm>
#include <cctype>
#include <iomanip>
#include <iostream>

using namespace std;

void Inventory::resize() {
    capacity *= 2;
    Product* bigger = new Product[capacity];
    for (int i = 0; i < count; ++i) {
        bigger[i] = items[i];
    }
    delete[] items;
    items = bigger;
}

Inventory::Inventory() : items(new Product[5]), count(0), capacity(5) {}

Inventory::~Inventory() {
    delete[] items;
}

int Inventory::size() const {
    return count;
}

Product& Inventory::at(int index) {
    return items[index];
}

const Product& Inventory::at(int index) const {
    return items[index];
}

int Inventory::findIndexById(int id) const {
    for (int i = 0; i < count; ++i) {
        if (items[i].id == id) return i;
    }
    return -1;
}

bool Inventory::add(const Product& product) {
    if (findIndexById(product.id) != -1) return false;
    if (count == capacity) resize();
    items[count++] = product;
    return true;
}

void Inventory::display() const {
    if (count == 0) {
        cout << "\nInventory is empty.\n";
        return;
    }
    printHeader();
    for (int i = 0; i < count; ++i) {
        printProduct(items[i]);
    }
}

void Inventory::printHeader() {
    cout << "\n" << left << setw(6) << "ID"
              << setw(32) << "Product" << setw(18) << "Category"
              << setw(14) << "Brand" << right << setw(12) << "Price"
              << setw(10) << "Stock" << "\n";
    cout << string(92, '-') << "\n";
}

const char* Inventory::colorCodeForBrand(const string& brand) {
    string normalizedBrand = brand;
    transform(normalizedBrand.begin(), normalizedBrand.end(), normalizedBrand.begin(),
              [](unsigned char c) { return static_cast<char>(tolower(c)); });

    if (normalizedBrand == "amd") return "\033[31m";
    if (normalizedBrand == "intel") return "\033[34m";
    if (normalizedBrand == "asus") return "\033[33m";
    if (normalizedBrand == "logitech") return "\033[37m";
    if (normalizedBrand == "moondrop") return "\033[95m";
    if (normalizedBrand == "tanchjim") return "\033[96m";
    return "\033[0m";
}

const char* Inventory::colorResetCode() {
    return "\033[0m";
}

void Inventory::printProduct(const Product& product) {
    cout << colorCodeForBrand(product.brand)
         << left << setw(6) << product.id
         << setw(25) << product.name.substr(0, 31)
         << setw(18) << product.category.substr(0, 17)
         << setw(14) << product.brand.substr(0, 13)
         << right << setw(12) << fixed << setprecision(2)
         << product.price << setw(10) << product.stockQuantity
         << colorResetCode() << "\n";
}

void Inventory::sortByPriceDescending() {
    for (int i = 1; i < count; ++i) {
        Product key = items[i];
        int j = i - 1;
        while (j >= 0 && items[j].price < key.price) {
            items[j + 1] = items[j];
            --j;
        }
        items[j + 1] = key;
    }
}

// Binary search uses an ID-sorted copy, so the inventory display order stays unchanged.
void Inventory::binarySearchId(int target) const {
    Product* copy = new Product[count];
    for (int i = 0; i < count; ++i) {
        copy[i] = items[i];
    }

    sort(copy, copy + count, [](const Product& a, const Product& b) {
        return a.id < b.id;
    });

    int low = 0;
    int high = count - 1;
    bool found = false;
    while (low <= high) {
        int mid = low + (high - low) / 2;
        if (copy[mid].id == target) {
            cout << "\nProduct found:\n";
            printHeader();
            printProduct(copy[mid]);
            found = true;
            break;
        }
        if (copy[mid].id < target) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }

    if (!found) {
        cout << "\nNo product found with ID " << target << ".\n";
    }
    delete[] copy;
}