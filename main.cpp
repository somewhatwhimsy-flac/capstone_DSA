#include "checkout.h"
#include "data_structures.h"
#include "inventory.h"
#include <algorithm>
#include <cctype>
#include <iostream>
#include <limits>
#include <queue>
#include <stack>
#include <string>

using namespace std;

// DSA FINAL CAPSTONE: Brand-Affiliated E-Commerce Inventory System

string lowerText(string value) {
    transform(value.begin(), value.end(), value.begin(),
              [](unsigned char c) { return static_cast<char>(tolower(c)); });
    return value;
}

int readInt(const string& prompt) {
    int value;
    while (true) {
        cout << prompt;
        if (cin >> value) return value;
        cout << "Invalid input. Please enter a whole number.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

double readDouble(const string& prompt) {
    double value;
    while (true) {
        cout << prompt;
        if (cin >> value && value >= 0) return value;
        cout << "Invalid input. Enter a non-negative number.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

void readLineAfterNumber(string& value) {
    getline(cin >> ws, value);
}

void seedInventory(Inventory& inventory, BrandCircle& brands, ProductBST& bst) {
    Product sample[] = {
        {1001, "ROG Gaming Mouse", "Peripherals", "ASUS", 2499.00, 12},
        {1002, "TUF Gaming Keyboard", "Peripherals", "ASUS", 3299.00, 8},
        {1003, "Ryzen 5 5600", "Processor", "AMD", 6499.00, 10},
        {1004, "Ryzen 7 5700X", "Processor", "AMD", 8999.00, 5},
        {1005, "Core i5 12400F", "Processor", "Intel", 7899.00, 7},
        {1006, "Core i7 12700K", "Processor", "Intel", 14999.00, 4},
        {1007, "G102 Lightsync", "Peripherals", "Logitech", 1199.00, 20},
        {1008, "G Pro Headset", "Headset", "Logitech", 4999.00, 6},
        {1009, "Starfield II", "IEM", "Moondrop", 7499.00, 6},
        {1010, "Moondrop x Crinacle DUSK", "IEM", "Moondrop", 19999.00, 3},
        {1011, "Kasier", "IEM", "Tanchjim", 12999.00, 4},
        {1012, "Space Pro Snowy Blossom", "DAC", "Tanchjim", 6999.00, 5}
    };

    for (const Product& product : sample) {
        inventory.add(product);
        bst.insert(product.id);
    }

    brands.add("ASUS");
    brands.add("AMD");
    brands.add("Intel");
    brands.add("Logitech");
    brands.add("Moondrop");
    brands.add("Tanchjim");
}

void filterByBrand(const Inventory& inventory) {
    string brand;
    cout << "Enter brand (ASUS, AMD, Intel, Logitech, Moondrop, Tanchjim): ";
    readLineAfterNumber(brand);
    string wanted = lowerText(brand);
    bool found = false;
    Inventory::printHeader();
    for (int i = 0; i < inventory.size(); ++i) {
        if (lowerText(inventory.at(i).brand) == wanted) {
            Inventory::printProduct(inventory.at(i));
            found = true;
        }
    }
    if (!found) cout << "No products found for that brand.\n";
}

void searchKeyword(const Inventory& inventory) {
    string keyword;
    cout << "Enter product name/category/brand keyword: ";
    readLineAfterNumber(keyword);
    keyword = lowerText(keyword);
    bool found = false;
    for (int i = 0; i < inventory.size(); ++i) {
        const Product& product = inventory.at(i);
        if (lowerText(product.name).find(keyword) != string::npos ||
            lowerText(product.category).find(keyword) != string::npos ||
            lowerText(product.brand).find(keyword) != string::npos) {
            if (!found) Inventory::printHeader();
            Inventory::printProduct(product);
            found = true;
        }
    }
    if (!found) cout << "No matching products found.\n";
}

void filterByPrice(const Inventory& inventory) {
    double minimum = readDouble("Minimum price: P");
    double maximum = readDouble("Maximum price: P");
    if (maximum < minimum) {
        cout << "Maximum price must be greater than or equal to minimum price.\n";
        return;
    }

    bool found = false;
    for (int i = 0; i < inventory.size(); ++i) {
        const Product& product = inventory.at(i);
        if (product.price >= minimum && product.price <= maximum) {
            if (!found) Inventory::printHeader();
            Inventory::printProduct(product);
            found = true;
        }
    }
    if (!found) cout << "No products within that price range.\n";
}

void addToCart(const Inventory& inventory, Cart& cart) {
    int id = readInt("Enter product ID: ");
    int index = inventory.findIndexById(id);
    if (index == -1) {
        cout << "Product ID not found.\n";
        return;
    }

    int quantity = readInt("Quantity: ");
    if (quantity <= 0) {
        cout << "Quantity must be at least 1.\n";
        return;
    }
    if (quantity > inventory.at(index).stockQuantity) {
        cout << "Not enough stock available. Current stock: "
             << inventory.at(index).stockQuantity << "\n";
        return;
    }

    cart.add(id, quantity);
    cout << "Added to cart.\n";
}

int main() {
    Inventory inventory;
    BrandCircle brands;
    ProductBST productTree;
    Cart cart;
    stack<UndoAction> undoStack;
    queue<CartNode*> checkoutQueue;

    seedInventory(inventory, brands, productTree);

    const string menuDivider(64, '=');
    bool running = true;
    while (running) {
        cout << "\n" << menuDivider << "\n"
             << "GEEKED OUT STORE 🤟\n"
             << menuDivider << "\n"
             << "1. View Complete Catalog\n"
             << "2. Filter Products by Brand\n"
             << "3. Search Products by Keyword\n"
             << "4. Filter Products by Price Range\n"
             << "5. Add Item to Shopping Cart\n"
             << "6. Process Customer Checkout\n"
             << "7. Perform Admin Undo Action\n"
             << "8. Display Partner Brand Banner\n"
             << "9. Sort Inventory Display (Price: High to Low)\n"
             << "10. Search Product by ID (BST + Binary Search)\n"
             << "11. Admin: Restock Product\n"
             << "12. View Shopping Cart\n"
             << "0. Exit\n"
             << menuDivider << "\n";

        int choice = readInt("Select an option: ");

        switch (choice) {
        case 1:
            inventory.display();
            break;
        case 2:
            filterByBrand(inventory);
            break;
        case 3:
            searchKeyword(inventory);
            break;
        case 4:
            filterByPrice(inventory);
            break;
        case 5:
            addToCart(inventory, cart);
            break;
        case 6:
            cart.checkout(inventory, checkoutQueue);
            break;
        case 7:
            if (undoStack.empty()) {
                cout << "No admin action to undo.\n";
            } else {
                UndoAction action = undoStack.top();
                undoStack.pop();
                int index = inventory.findIndexById(action.productId);
                if (index != -1) {
                    inventory.at(index).stockQuantity = action.previousStock;
                    cout << "Undo successful. Previous stock restored for product "
                         << action.productId << ".\n";
                }
            }
            break;
        case 8:
            brands.showTwoRounds();
            break;
        case 9:
            inventory.sortByPriceDescending();
            cout << "Inventory sorted by price (highest to lowest).\n";
            inventory.display();
            break;
        case 10: {
            int id = readInt("Enter product ID to search: ");
            if (productTree.search(id)) {
                inventory.binarySearchId(id);
            } else {
                cout << "BST: Product ID not found.\n";
            }
            break;
        }
        case 11: {
            int id = readInt("Enter product ID to restock: ");
            int index = inventory.findIndexById(id);
            if (index == -1) {
                cout << "Product ID not found.\n";
                break;
            }

            int amount = readInt("Quantity to add: ");
            if (amount <= 0) {
                cout << "Quantity must be at least 1.\n";
                break;
            }

            undoStack.push({id, inventory.at(index).stockQuantity});
            inventory.at(index).stockQuantity += amount;
            cout << "Restock successful. New stock: "
                 << inventory.at(index).stockQuantity << "\n";
            break;
        }
        case 12:
            cart.display(inventory);
            break;
        case 0:
            running = false;
            break;
        default:
            cout << "Invalid option. Try again.\n";
        }
    }

    while (!checkoutQueue.empty()) {
        delete checkoutQueue.front();
        checkoutQueue.pop();
    }

    cout << "\nThank you for visiting Geeked Out Store!\n";
    return 0;
}