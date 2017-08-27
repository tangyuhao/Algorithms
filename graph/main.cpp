#include <iostream>
#include <vector>
#include <numeric>
#include <unordered_map>

class UnionFindSet {
private:
    int n;
    std::vector<int> parents;
public:
    UnionFindSet(int n);

    void makeUnion(int i, int j);

    int find(int i);

    void print() const;
};

UnionFindSet::UnionFindSet(int n) {
    this->n = n;
    parents = std::vector<int>(n);
    std::iota(parents.begin(), parents.end(), 0);
}

int UnionFindSet::find(int i) {
    if (i < 0 || i >= n) {
        return -1;
    }
    int curNode = i;
    std::unordered_map<int, int> visitedNodes;
    while (parents[curNode] != curNode) {
        if (visitedNodes.find(curNode) == visitedNodes.end()) {
            curNode = parents[curNode];
            visitedNodes[curNode] = 1;
        } else { // find a cycle
            break;
        }
    }
    std::for_each(visitedNodes.begin(), visitedNodes.end(), [&](std::pair<int, int> p) {
        visitedNodes[p.first] = curNode;
    });
    return curNode;
}

void UnionFindSet::makeUnion(int i, int j) {
    if (i >= 0 && i < n && j >= 0 && j < n) {
        int ri = find(i);
        int rj = find(j);
        if (ri != rj) {
            parents[ri] = rj;
        }
    } else
        return;
}

void UnionFindSet::print() const {
    for (int i = 0; i < n; i++) {
        std::cout << i << ":" << parents[i] << std::endl;
    }
}

void testUnionFindSet() {
    UnionFindSet unionFindSet(10);
    unionFindSet.makeUnion(2, 6);
    unionFindSet.makeUnion(5, 6);
    unionFindSet.makeUnion(1, 8);
    unionFindSet.makeUnion(2, 9);
    unionFindSet.makeUnion(5, 3);
    unionFindSet.makeUnion(4, 8);
    unionFindSet.makeUnion(4, 0);
    for (int i = 0; i < 10; i++) {
        unionFindSet.find(i);
    }
    unionFindSet.print();
}

int main() {
    testUnionFindSet();
    return 0;
}
