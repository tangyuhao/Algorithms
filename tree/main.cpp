#include <iostream>
#include <vector>
#include <deque>

class BSTNode {
public:
    int value;
    BSTNode *pLeft;
    BSTNode *pRight;

    BSTNode(int v) : value(v), pLeft(NULL), pRight(NULL) {}
};

typedef void (*Visit)(int value);

void visitNode(int value) {
    std::cout << value << " ";
}

class BinarySearchTree {
private:
    BSTNode *pRoot;

public:
    BinarySearchTree();

    ~BinarySearchTree();

    bool insertNode(int value);

    bool deleteNode(int value);

    BSTNode *findNode(int value) const;

    void preOrder(Visit visit) const;

    void inOrder(Visit visit) const;

    void postOrder(Visit visit) const;

private:
    void destory(BSTNode *);

    void deleteSingleSon(BSTNode *curNode, BSTNode *parentNode) {
        BSTNode *sonNode = curNode->pRight ? curNode->pRight : curNode->pLeft;
        if (curNode == pRoot) {
            pRoot = sonNode;
            delete curNode;
        } else {
            if (curNode == parentNode->pLeft) {
                parentNode->pLeft = sonNode;
            } else {
                parentNode->pRight = sonNode;
            }
            delete curNode;
        }
    }

    void deleteChildless(BSTNode *curNode, BSTNode *parentNode) {
        if (curNode == pRoot) {
            pRoot = NULL;
        } else {
            if (curNode == parentNode->pLeft) {
                parentNode->pLeft = NULL;
            } else {
                parentNode->pRight = NULL;
            }
        }
        delete curNode;
    }
};

void BinarySearchTree::destory(BSTNode *node) {
    if (node) {
        destory(node->pLeft);
        destory(node->pRight);
        delete node;
    }
}

BinarySearchTree::~BinarySearchTree() {
    if (pRoot) {
        destory(pRoot);
        pRoot = NULL;
    }
};

BinarySearchTree::BinarySearchTree() {
    pRoot = NULL;
}

bool BinarySearchTree::insertNode(int value) {
    if (pRoot == NULL) {
        pRoot = new BSTNode(value);
        return true;
    }
    BSTNode *curNode = pRoot;
    BSTNode *parentNode = pRoot;
    while (curNode) {
        parentNode = curNode;
        if (value < curNode->value) {
            curNode = curNode->pLeft;
        } else if (value > curNode->value) {
            curNode = curNode->pRight;
        } else {
            return false;
        }
    }
    if (value < parentNode->value) {
        parentNode->pLeft = new BSTNode(value);
    } else {
        parentNode->pRight = new BSTNode(value);
    }
    return true;
}

BSTNode *BinarySearchTree::findNode(int value) const {
    if (pRoot == NULL)
        return NULL;
    BSTNode *curNode = pRoot;
    while (curNode) {
        if (value < curNode->value) {
            curNode = curNode->pLeft;
        } else if (value > curNode->value) {
            curNode = curNode->pRight;
        } else {
            return curNode;
        }
        return NULL;
    }
}

bool BinarySearchTree::deleteNode(int value) {
    if (pRoot == NULL) {
        return false;
    }
    BSTNode *curNode = pRoot;
    BSTNode *parentNode = NULL;
    while (curNode) {
        if (value < curNode->value) {
            parentNode = curNode;
            curNode = curNode->pLeft;
        } else if (value > curNode->value) {
            parentNode = curNode;
            curNode = curNode->pRight;
        } else {
            break;
        }
    }
    if (curNode == NULL) {
        return false;
    }
    if (curNode->pRight && curNode->pLeft) {
        BSTNode *tmpNode = curNode->pRight;
        BSTNode *tmpParentNode = curNode;
        while (tmpNode->pLeft) {
            tmpParentNode = tmpNode;
            tmpNode = tmpNode->pLeft;
        }
        curNode->value = tmpNode->value;
        if (!tmpNode->pRight) {
            deleteChildless(tmpNode, tmpParentNode);
        } else {
            deleteSingleSon(tmpNode, tmpParentNode);
        }

    } else if (curNode->pRight || curNode->pLeft) {
        deleteSingleSon(curNode, parentNode);
    } else {
        deleteChildless(curNode, parentNode);
    }
    return true;
}

void BinarySearchTree::preOrder(Visit visit) const {
    if (!pRoot) {
        return;
    }
    std::deque<BSTNode *> stack;
    stack.push_back(pRoot);
    while (!stack.empty()) {
        BSTNode *node = stack.back();
        stack.pop_back();
        visit(node->value);
        if (node->pLeft) {
            stack.push_back(node->pLeft);
        }
        if (node->pRight) {
            stack.push_back(node->pRight);
        }
    }
}

void BinarySearchTree::inOrder(Visit visit) const {
    if (!pRoot)
        return;
    std::deque<std::pair<BSTNode *, int>> stack;
    stack.push_back(std::make_pair(pRoot, 0));
    while (!stack.empty()) {
        BSTNode *curNode = stack.back().first;
        int times = stack.back().second;
        stack.pop_back();
        if (times == 0) {
            if (curNode->pRight) {
                stack.push_back(std::make_pair(curNode->pRight, 0));
            }
            stack.push_back(std::make_pair(curNode, 1));
            if (curNode->pLeft) {
                stack.push_back(std::make_pair(curNode->pLeft, 0));
            }
        } else {
            visit(curNode->value);
        }
    }
}

void BinarySearchTree::postOrder(Visit visit) const {
    if (!pRoot)
        return;
    std::deque<std::pair<BSTNode *, int>> stack;
    stack.push_back(std::make_pair(pRoot, 0));
    while (!stack.empty()) {
        BSTNode *curNode = stack.back().first;
        int times = stack.back().second;
        stack.pop_back();
        if (times == 0) {
            stack.push_back(std::make_pair(curNode, 1));
            if (curNode->pRight) {
                stack.push_back(std::make_pair(curNode->pRight, 0));
            }
            if (curNode->pLeft) {
                stack.push_back(std::make_pair(curNode->pLeft, 0));
            }
        } else {
            visit(curNode->value);
        }
    }
}

void testBinarySearchTree() {
    BinarySearchTree t;
    std::vector<int> nodeValues({2, 5, 2, 4, 6, 8, 9, 3, 4, 6, 8, 9, 3, 34, 45, 6, 34, 3, 56});
    std::for_each(nodeValues.begin(), nodeValues.end(), [&t](int i) { t.insertNode(i); });
    std::for_each(nodeValues.begin(), nodeValues.end(), [&t](int i) { t.deleteNode(i); });
    std::for_each(nodeValues.begin(), nodeValues.end(), [&t](int i) { t.insertNode(i); });
    t.inOrder(visitNode);
    std::cout << std::endl;
    t.preOrder(visitNode);
    std::cout << std::endl;
    t.postOrder(visitNode);
    std::cout << std::endl;
}

int main() {
    testBinarySearchTree();
    return 0;
}


