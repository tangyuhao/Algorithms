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

    std::vector<bool> findCutPoints(int root);

    void shortestPath_Dijkstra(int source, std::vector<int> &shortestLen, std::vector<int> &pre);

    void printShortestPath_Dijkstra(int start, int end, std::vector<int> pre, std::vector<int> shortestLen);

    void shortestPath_Floyd(std::vector<std::vector<int>> &shortestLen, std::vector<std::vector<int>> &next);

    void printShortestPath_Floyd(std::vector<std::vector<int>> &shortestLen, std::vector<std::vector<int>> &next);

private:
    void _findCutPoints(int curNode, int parent, int root, std::vector<int> dfn, std::vector<int> low, int &cnt,
                        std::vector<bool> isCutPoint);
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

std::vector<bool> Graph::findCutPoints(int root) {
    std::deque<int> stack;
    std::vector<bool> isCutPoint(num, false);
    std::vector<int> dfn(num, -1);
    // this stores the earliest order among the points connected to the target point
    std::vector<int> low(num, -1);
    int cnt = 0;
    _findCutPoints(root, -1, root, dfn, low, cnt, isCutPoint);
    return isCutPoint;
}

void Graph::_findCutPoints(int curNode, int parent, int root, std::vector<int> dfn, std::vector<int> low, int &cnt,
                           std::vector<bool> isCutPoint) {
    dfn[curNode] = low[curNode] = cnt++;
    int child = 0;
    for (int j = 0; j < num; j++) {
        if (adjMatrix[curNode][j] != INT_MAX) {
            if (dfn[j] == -1) {
                child++;
                _findCutPoints(j, curNode, root, dfn, low, cnt, isCutPoint);
                low[curNode] = std::min(low[curNode], low[j]);
                if (curNode != root && low[j] >= dfn[curNode])
                    isCutPoint[curNode] = true;
                else if (curNode == root && child == 2)
                    isCutPoint[curNode] = true;
            } else if (parent != j) {
                low[curNode] = std::min(low[curNode], dfn[j]);
            }
        }
    }
}

void Graph::shortestPath_Dijkstra(int source, std::vector<int> &shortestLen, std::vector<int> &pre) {
    shortestLen = adjMatrix[source];
    pre = std::vector<int>((unsigned int) num, -1);
    for (int i = 0; i < num; i++) {
        if (shortestLen[i] < INT_MAX)
            pre[i] = source;
    }
    std::vector<bool> visited((unsigned int) num, false);
    pre[source] = -1;
    shortestLen[source] = 0;
    std::deque<int> queue;
    queue.push_front(source);
    while (!queue.empty()) {
        int curNode = queue.back();
        queue.pop_back();
        visited[curNode] = true;
        for (int i = 0; i < num; i++) {
            if (adjMatrix[curNode][i] < INT_MAX && visited[i] == false)
                queue.push_front(i);
        }
        for (int i = 0; i < num; i++) {
            if (visited[i]) continue;
            if (shortestLen[curNode] < INT_MAX && adjMatrix[curNode][i] < INT_MAX) {
                if (shortestLen[i] > shortestLen[curNode] + adjMatrix[curNode][i]) {
                    shortestLen[i] = shortestLen[curNode] + adjMatrix[curNode][i];
                    pre[i] = curNode;
                }
            }
        }
    }
}

void Graph::printShortestPath_Dijkstra(int start, int end, std::vector<int> pre, std::vector<int> shortestLen) {
    int len = shortestLen[end];
    int e = end;
    std::vector<int> path;
    while (start != e) {
        e = pre[e];
        path.push_back(e);
    }
    std::reverse(path.begin(), path.end());
    std::cout << start << " to " << end << ": " << len << " ";
    std::for_each(path.begin(), path.end(), [](int i) { std::cout << i << " -> "; });
    std::cout << end << std::endl;
}

void Graph::shortestPath_Floyd(std::vector<std::vector<int>> &shortestLen, std::vector<std::vector<int>> &next) {
    shortestLen = adjMatrix;
    next = std::vector<std::vector<int>>((unsigned int) num, std::vector<int>((unsigned int) num));
    for (int i = 0; i < num; i++) {
        for (int j = 0; j < num; j++) {
            next[i][j] = j;
        }
    }
    for (int k = 0; k < num; k++) {
        for (int i = 0; i < num; i++) {
            for (int j = 0; j < num; j++) {
                if (shortestLen[i][k] == INT_MAX || shortestLen[k][j] == INT_MAX)
                    continue;
                if (shortestLen[i][j] > shortestLen[i][k] + shortestLen[k][j]) {
                    shortestLen[i][j] = shortestLen[i][k] + shortestLen[k][j];
                    next[i][j] = next[i][k];
                }
            }
        }
    }
}

void Graph::printShortestPath_Floyd(std::vector<std::vector<int>> &shortestLen, std::vector<std::vector<int>> &next) {
    for (int start = 0; start < num; start++) {
        for (int end = 0; end < num; end++) {
            if (start == end || shortestLen[start][end] == INT_MAX) continue;
            int len = shortestLen[start][end];
            int s = start;
            std::vector<int> path;
            while (s != end) {
                s = next[s][end];
                path.push_back(s);
            }
            std::cout << start << " to " << end << ": " << len << " ";
            std::cout << start;
            std::for_each(path.begin(), path.end(), [](int i) { std::cout << " -> " << i; });
            std::cout << std::endl;
        }
    }
}

void testGraphShortestPath() {
    Graph graph(9);
    graph.addLinesWithWeights({{1, 6, 24},
                               {1, 5, 70},
                               {1, 3, 47},
                               {6, 4, 120},
                               {6, 3, 25},
                               {3, 4, 88},
                               {3, 5, 23},
                               {3, 2, 55},
                               {3, 7, 66},
                               {5, 7, 42},
                               {5, 2, 31},
                               {2, 7, 74},
                               {2, 4, 31},
                               {2, 8, 79},
                               {4, 8, 29},
                               {7, 8, 66}
                              });
    std::vector<int> shortestLen, pre;
    graph.shortestPath_Dijkstra(1, shortestLen, pre);
    for (int i = 2; i < 9; i++)
        graph.printShortestPath_Dijkstra(1, i, pre, shortestLen);
    std::vector<std::vector<int>> allShortestLen, next;
    std::cout << std::endl;
    graph.shortestPath_Floyd(allShortestLen, next);
    graph.printShortestPath_Floyd(allShortestLen, next);
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
    Visit visit = [](int i) { std::cout << i << " "; };
    graph.bfs(1, visit);
    std::cout << std::endl;
    graph.dfs(1, visit);
}


int main() {
    testGraphShortestPath();
    return 0;
}
