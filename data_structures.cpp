#include "data_structures.h"
#include "inventory.h"
#include <iostream>

using namespace std;

BrandNode::BrandNode(const string& value) : brand(value), next(nullptr) {}

BrandCircle::BrandCircle() : tail(nullptr) {}

BrandCircle::~BrandCircle() {
    clear();
}

void BrandCircle::add(const string& brand) {
    BrandNode* node = new BrandNode(brand);
    if (!tail) {
        tail = node;
        node->next = node;
    } else {
        node->next = tail->next;
        tail->next = node;
        tail = node;
    }
}

void BrandCircle::showTwoRounds() const {
    if (!tail) {
        cout << "\nNo partner brands available.\n";
        return;
    }

    BrandNode* current = tail->next;
    cout << "\nPARTNER BRAND SHOWCASE (two rounds)\n";
    int total = 0;
    BrandNode* temp = tail->next;
    do {
        ++total;
        temp = temp->next;
    } while (temp != tail->next);

    for (int i = 0; i < total * 2; ++i) {
        cout << Inventory::colorCodeForBrand(current->brand)
             << current->brand << Inventory::colorResetCode();
        if ((i + 1) % total == 0) cout << "  | end of round";
        cout << "\n";
        current = current->next;
    }
}

void BrandCircle::clear() {
    if (!tail) return;

    BrandNode* start = tail->next;
    tail->next = nullptr;
    while (start) {
        BrandNode* next = start->next;
        delete start;
        start = next;
    }
    tail = nullptr;
}

TreeNode::TreeNode(int id) : productId(id), left(nullptr), right(nullptr) {}

ProductBST::ProductBST() : root(nullptr) {}

ProductBST::~ProductBST() {
    destroy(root);
}

TreeNode* ProductBST::insert(TreeNode* node, int id) {
    if (!node) return new TreeNode(id);
    if (id < node->productId) {
        node->left = insert(node->left, id);
    } else if (id > node->productId) {
        node->right = insert(node->right, id);
    }
    return node;
}

bool ProductBST::search(TreeNode* node, int id) const {
    if (!node) return false;
    if (node->productId == id) return true;
    return id < node->productId ? search(node->left, id) : search(node->right, id);
}

void ProductBST::destroy(TreeNode* node) {
    if (!node) return;
    destroy(node->left);
    destroy(node->right);
    delete node;
}

void ProductBST::insert(int id) {
    root = insert(root, id);
}

bool ProductBST::search(int id) const {
    return search(root, id);
}