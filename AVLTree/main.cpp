#include <iostream>
#include <vector>
#include <string>

template<class T>
class TreeNode {
public:
    TreeNode(T value) : value(value), height(1), freq(1), pLeft(NULL), pRight(NULL) {};

    friend class AVLTree;

private:
    T value;
    int height;
    int freq;
    TreeNode<T> *pLeft;
    TreeNode<T> *pRight;
};

template<class T>
class AVLTree {
private:
    TreeNode<T> *pRoot;

    void destroy(TreeNode<T> *node) {
        if (node) {
            destroy(node->pLeft);
            destroy(node->pRight);
            delete node;
        }
    }

    void insertNode(TreeNode<T> *node, T value);

public:
    AVLTree() {
        pRoot = NULL;
    }

    ~AVLTree() {
        destroy(pRoot);
    }

    inline static int getHeight(TreeNode<T> *root) {
        return root->height;
    }

    void singleRotateLeft(TreeNode<T> *&k2);

    void singleRotateRight(TreeNode<T> *&k2);

    void doubleRotateLR(TreeNode<T> *&k3);

    void doubleRotateRL(TreeNode<T> *&k3);

    void insert(T value) {
        insertNode(pRoot, value);
    }

};

template<class T>
void AVLTree<T>::singleRotateLeft(TreeNode<T> *&k2) {
    TreeNode<T> *k1 = k2->pLeft;
    k2->pLeft = k1->pRight;
    k1->pRight = k2;
    k2->height = std::max(getHeight(k2->pLeft), getHeight(k2->pRight)) + 1;
    k1->height = std::max(getHeight(k1->pLeft), getHeight(k2)) + 1;
    k2 = k1;
}

template<class T>
void AVLTree<T>::singleRotateRight(TreeNode<T> *&k2) {
    TreeNode<T> *k1 = k2->pRight;
    k2->pRight = k1->pLeft;
    k1->pLeft = k2;
    k2->height = std::max(getHeight(k2->pLeft), getHeight(k2->pRight)) + 1;
    k1->height = std::max(getHeight(k1->pRight), getHeight(k2));
    k2 = k1;
}

template<class T>
void AVLTree<T>::doubleRotateLR(TreeNode<T> *&k3) {
    singleRotateRight(k3->pLeft);
    singleRotateLeft(k3);
}

template<class T>
void AVLTree<T>::doubleRotateRL(TreeNode<T> *&k3) {
    singleRotateLeft(k3->pRight);
    singleRotateRight(k3);
}

template<class T>
void AVLTree<T>::insertNode(TreeNode<T> *node, T value) {
    if (node == NULL) {
        node = new TreeNode<T>(value);
        return;
    }
    if (node->value > value) {
        insertNode(node->pLeft, value);
        if (2 == getHeight(node->pLeft) - getHeight(node->pRight)) {
            if (value < node->pLeft->value)
                singleRotateLeft(node);
            else
                doubleRotateLR(node);
        }
    } else if (node->value < value) {
        insertNode(node->pRight, value);
        if (2 == getHeight(node->pRight) - getHeight(node->pLeft)) {
            if (value > node->pRight->value)
                singleRotateRight(node);
            else
                doubleRotateRL(node);
        }
    } else
        ++(node->freq);
    node->height = std::max(getHeight(node->pLeft), getHeight(node->pRight)) + 1;
}
