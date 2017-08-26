#include <iostream>
#include <vector>
#include <string>

template<class T>
class AVLTree;

template<class T>
class TreeNode {
public:
    TreeNode(T value) : value(value), height(1), freq(1), pLeft(NULL), pRight(NULL) {};

    friend class AVLTree<T>;

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

    void insertNode(TreeNode<T> *&node, T value);

    void deleteNode(TreeNode<T> *&node, T value);

public:
    AVLTree() {
        pRoot = NULL;
    }

    ~AVLTree() {
        destroy(pRoot);
    }

    inline static int getHeight(TreeNode<T> *root) {
        if (!root) return 0;
        return root->height;
    }

    void singleRotateLeft(TreeNode<T> *&k2);

    void singleRotateRight(TreeNode<T> *&k2);

    void doubleRotateLR(TreeNode<T> *&k3);

    void doubleRotateRL(TreeNode<T> *&k3);

    void insert(T value) {
        insertNode(pRoot, value);
    }

    void remove(T value) {
        deleteNode(pRoot, value);
    }

    int treeHeight() {
        return getHeight(pRoot);
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
void AVLTree<T>::insertNode(TreeNode<T> *&node, T value) {
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

template<class T>
void AVLTree<T>::deleteNode(TreeNode<T> *&node, T value) {
    if (node == NULL)
        return;
    if (value < node->value) {
        deleteNode(node->pLeft, value);
        if (2 == getHeight(node->pRight) - getHeight(node->pLeft)) {
            if (node->pRight->pRight != NULL && getHeight(node->pRight->pLeft) > getHeight(node->pRight->pRight))
                doubleRotateRL(node);
            else
                singleRotateRight(node);
        }
    } else if (value > node->value) {
        deleteNode(node->pRight, value);
        if (2 == getHeight(node->pLeft) - getHeight(node->pRight)) {
            if (node->pLeft->pRight != NULL && getHeight(node->pLeft->pRight) > getHeight(node->pLeft->pLeft))
                doubleRotateLR(node);
            else
                singleRotateLeft(node);
        }
    } else {
        // if it has two sons
        if (node->pLeft && node->pRight) {
            TreeNode<T> *temp = node->pRight; // temp is the right son
            while (temp->pLeft)
                temp = temp->pLeft; // temp is the left most one
            node->value = temp->value;
            node->freq = temp->freq;
            deleteNode(node->pRight, temp->value); // delete the smallest node of the right branch
            if (2 == getHeight(node->pLeft) - getHeight(node->pRight)) {
                if (node->pLeft->pRight && getHeight(node->pLeft->pRight) > getHeight(node->pLeft->pLeft))
                    doubleRotateLR(node);
                else
                    singleRotateLeft(node);
            }
        } else { // if it has at most 1 son
            TreeNode<T> *temp = node;
            if (node->pLeft == NULL)
                node = node->pRight;
            else if (node->pRight == NULL)
                node = node->pLeft;
            delete temp;
        }
        if (!node) return;
        node->height = std::max(getHeight(node->pLeft), getHeight(node->pRight)) + 1;
        return;
    }
}

void testAVLTree() {
    std::vector<int> arr({1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20});
    AVLTree<int> tree;
    std::for_each(arr.begin(), arr.end(), [&](int i) {
        tree.insert(i);
        std::cout << tree.treeHeight() << " ";
    });
    std::cout << std::endl;
    std::for_each(arr.begin(), arr.end(), [&](int i) {
        tree.remove(i);
        std::cout << tree.treeHeight() << " ";
    });
    std::cout << std::endl;
}

int main() {
    testAVLTree();
    return 0;
}