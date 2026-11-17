#pragma once
#include <string>

using namespace std;

struct BrandNode {
    string brand;
    BrandNode* next;

    explicit BrandNode(const string& value);
};

class BrandCircle {
private:
    BrandNode* tail;

public:
    BrandCircle();
    ~BrandCircle();

    BrandCircle(const BrandCircle&) = delete;
    BrandCircle& operator=(const BrandCircle&) = delete;

    void add(const string& brand);
    void showTwoRounds() const;
    void clear();
};

struct UndoAction {
    int productId;
    int previousStock;
};

struct TreeNode {
    int productId;
    TreeNode* left;
    TreeNode* right;

    explicit TreeNode(int id);
};

class ProductBST {
private:
    TreeNode* root;

    TreeNode* insert(TreeNode* node, int id);
    bool search(TreeNode* node, int id) const;
    void destroy(TreeNode* node);

public:
    ProductBST();
    ~ProductBST();

    ProductBST(const ProductBST&) = delete;
    ProductBST& operator=(const ProductBST&) = delete;

    void insert(int id);
    bool search(int id) const;
};