#include <iostream>
#include <vector>
#include <numeric>
#include <unordered_map>
#include <list>
#include <tuple>
#include <deque>

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

typedef void (*Visit)(int node);


class Graph {
private:
    int num;
    bool directional;
    // adjMatrix[i][j] means the arrow from i to j
    std::vector<std::vector<int>> adjMatrix;

public:
    Graph(int n, bool directional = true) : num(n), directional(directional) {
        adjMatrix = std::vector<std::vector<int>>(num);
        for (int i = 0; i < num; i++) {
            adjMatrix[i] = std::vector<int>(num);
        }
        for (int i = 0; i < num; i++) {
            for (int j = 0; j < num; j++) {
                adjMatrix[i][j] = INT_MAX;
            }
        }
    }

    void addLinesNoWeight(const std::vector<std::pair<int, int>> lines) {
        if (directional) {
            for (auto i = lines.begin(); i != lines.end(); i++) {
                adjMatrix[i->first][i->second] = 1;
            }
        } else {
            for (auto i = lines.begin(); i != lines.end(); i++) {
                adjMatrix[i->first][i->second] = 1;
                adjMatrix[i->second][i->first] = 1;
            }
        }
    }

    void addLinesWithWeights(const std::vector<std::tuple<int, int, int>> lines) {
        if (directional) {
            for (auto i = lines.begin(); i != lines.end(); i++) {
                adjMatrix[std::get<0>(*i)][std::get<1>(*i)] = std::get<2>(*i);
            }
        } else {
            for (auto i = lines.begin(); i != lines.end(); i++) {
                adjMatrix[std::get<0>(*i)][std::get<1>(*i)] = std::get<2>(*i);
                adjMatrix[std::get<1>(*i)][std::get<0>(*i)] = std::get<2>(*i);
            }
        }
    }

    void bfs(int start, Visit visit);

    void dfs(int start, Visit visit);
};

void Graph::bfs(int start, Visit visit) {
    std::deque<int> queue;
    std::vector<bool> visited(num, false);
    queue.push_front(start);
    while (!queue.empty()) {
        int curNode = queue.back();
        queue.pop_back();
        visit(curNode);
        visited[curNode] = true;
        for (int i = 0; i < num; i++) {
            if (adjMatrix[curNode][i] != INT_MAX && !visited[i])
                queue.push_front(i);
        }
    }
}

void Graph::dfs(int start, Visit visit) {
    std::deque<int> stack;
    std::vector<bool> visited(num, false);
    stack.push_back(start);
    while (!stack.empty()) {
        int curNode = stack.back();
        stack.pop_back();
        visit(curNode);
        visited[curNode] = true;
        for (int i = 0; i < num; i++) {
            if (adjMatrix[curNode][i] != INT_MAX && !visited[i])
                stack.push_back(i);
        }
    }
}

void testGraphInitialize() {
    Graph graph(4);
    graph.addLinesNoWeight({{1, 2},
                            {2, 3},
                            {0, 3}});
    graph.addLinesWithWeights({{1, 2, 22},
                               {2, 3, 2232},
                               {0, 3, 222}});
}

void testDFSandBFS() {
    Graph graph(10, false);
    graph.addLinesNoWeight({{1, 2},
                            {2, 3},
                            {2, 0},
                            {4, 6},
                            {5, 8},
                            {8, 7},
                            {7, 9},
                            {9, 2}});
    Visit visit = [](int i) {std::cout << i << " ";};
    graph.bfs(1, visit);
    std::cout << std::endl;
    graph.dfs(1, visit);
}


int main() {
    testDFSandBFS();
    return 0;
}
