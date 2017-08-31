#include <iostream>
#include <vector>
#include <numeric>
#include <unordered_map>
#include <list>
#include <tuple>
#include <deque>
#include <set>
#include <stack>

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
        parents[p.first] = curNode;
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


class Edge {
public:
    int i, j;
    int weight;

    Edge(int i, int j, int weight) : i(i), j(j), weight(weight) {};

    bool operator<(const Edge &e) const {
        return this->weight < e.weight;
    }


    bool operator==(const Edge &e) const {
        if (this->i == e.i && this->j == e.j)
            return true;
        else
            return false;
    }

    static void addEdgeFromNode(int from, std::vector<bool> visited, std::vector<Edge> &e,
                                std::vector<std::vector<int>> adjMatrix) {
        int size = adjMatrix.size();
        for (int i = 0; i < size; i++) {
            if (adjMatrix[from][i] < INT_MAX && !visited[i]) {
                e.push_back(Edge(from, i, adjMatrix[from][i]));
            }
        }
    }

    static void deleteEdgeToNode(int to, std::vector<bool> visited, std::vector<Edge> &e,
                                 std::vector<std::vector<int>> adjMatrix) {
        int size = adjMatrix.size();
        auto iter = e.begin();
        while (iter != e.end()) {
            if (iter->j == to)
                iter = e.erase(iter);
            else
                iter++;
        }
    }
};

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

    void shortestPath_BellmanFord(int start, std::vector<int> &shortestLen, std::vector<int> &pre);

    void printShortestPath_BellmanFord(int start, std::vector<int> shortestLen, std::vector<int> pre);

    bool MST_Prim(std::vector<Edge> &result);

    void MST_Kruskal(std::vector<Edge> &result);

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

/*
 * Cannot deal with the graph with negative weights
 */
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

/*
 * This algorithm can judge whether the negative cycle exists and it does not have the limitation only applying for
 * positive weights
 */
void Graph::shortestPath_BellmanFord(int start, std::vector<int> &shortestLen, std::vector<int> &pre) {
    shortestLen = std::vector<int>(num, INT_MAX);
    pre = std::vector<int>(num, -1);
    shortestLen[start] = 0;
    for (int i = 0; i < num - 1; i++) {
        for (int j = 0; j < num; j++) {
            for (int k = 0; k < num; k++) {
                if (adjMatrix[j][k] < INT_MAX && shortestLen[j] < INT_MAX) {
                    if (shortestLen[k] > shortestLen[j] + adjMatrix[j][k]) {
                        shortestLen[k] = shortestLen[j] + adjMatrix[j][k];
                        pre[k] = j;
                    }
                }
            }
        }
    }
}

void Graph::printShortestPath_BellmanFord(int start, std::vector<int> shortestLen, std::vector<int> pre) {
    for (int i = 0; i < num; i++) {
        if (shortestLen[i] < INT_MAX) {
            std::vector<int> path;
            int e = i;
            while (e != start) {
                path.push_back(e);
                e = pre[e];
            }
            std::reverse(path.begin(), path.end());
            std::cout << i << ": " << shortestLen[i] << " " << start;
            std::for_each(path.begin(), path.end(), [](int k) { std::cout << " -> " << k; });
            std::cout << std::endl;
        }
    }
}

bool Graph::MST_Prim(std::vector<Edge> &result) {
    std::vector<Edge> e;
    std::vector<bool> visited(num, false);
    visited[0] = true;
    Edge::addEdgeFromNode(0, visited, e, adjMatrix);
    for (int i = 1; i < num; i++) {
        if (e.empty())
            return false;
        Edge addedEdge = *std::min_element(e.begin(), e.end());
        result.push_back(addedEdge);
        visited[addedEdge.j] = true;
        Edge::deleteEdgeToNode(addedEdge.j, visited, e, adjMatrix);
        Edge::addEdgeFromNode(addedEdge.j, visited, e, adjMatrix);
    }
    return true;
}

/*
 * Time Complexity = O(ElogE)
 */
void Graph::MST_Kruskal(std::vector<Edge> &result) {
    std::vector<Edge> edge;
    for (int i = 0; i < num; i++) {
        for (int j = 1; j < num; j++) {
            if (adjMatrix[i][j] != INT_MAX)
                edge.push_back(Edge(i, j, adjMatrix[i][j]));
        }
    }
    std::sort(edge.begin(), edge.end());
    UnionFindSet s(num);
    int edgeSize = edge.size();
    int k = 0;
    for (int i = 0; i < edgeSize; i++) {
        int ri = s.find(edge[i].i);
        int rj = s.find(edge[i].j);
        if (ri == rj) continue;
        s.makeUnion(edge[i].i, edge[i].j);
        result.push_back(edge[i]);
        k++;
        if (k == num - 1)
            break;
    }
}

bool isNeighbor(std::string m, std::string n) {
    if (m.length() != n.length())
        return false;
    int len = m.length();
    int cntDiff = 0;
    for (int i = 0; i < len; i++) {
        if (m[i] != n[i])
            cntDiff++;
    }
    return cntDiff == 1;
}

std::vector<std::string> findWordLadderPath(std::string start, std::string end,
                                            std::unordered_map<std::string, std::vector<std::string>> neighborMap) {
    std::stack<std::string> s;
    std::vector<std::string> list;
    s.push(start);
    while (!s.empty()) {
        std::string &cur = s.top();
        s.pop();
        list.push_back(cur);
        if (cur == end)
            return list;
        if (neighborMap[cur].empty()) {
            list.pop_back();
            continue;
        }
        std::vector<std::string> tmp = neighborMap[cur];
        std::for_each(tmp.begin(), tmp.end(), [&](std::string str) { s.push(str); });
    }

}

int wordLadder(std::string start, std::string end, std::set<std::string> dict, std::vector<std::string> &path) {
    std::deque<std::string> q;
    std::list<std::string> result;
    std::unordered_map<std::string, std::vector<std::string>> neighborMap;
    if (start == end)
        return 0;
    q.push_front(start);
    int depth = 1;
    int children = 1;
    std::vector<std::string> childrenList;
    while (!q.empty()) {
        std::string &cur = q.back();
        q.pop_back();
        if (isNeighbor(cur, end)) {
            neighborMap[cur] = std::vector<std::string>({end});
            path = findWordLadderPath(start, end, neighborMap);
            return depth;
        }
        if (--children == 0) {
            for (auto it = dict.begin(); it != dict.end();) {
                if (isNeighbor(*it, cur)) {
                    q.push_front(*it);
                    childrenList.push_back(*it);
                    it = dict.erase(it);
                    children++;
                } else
                    it++;
            }
            neighborMap[cur] = childrenList;
            childrenList.clear();
            depth++;
        }
    }
    return -1;
}

void BFS_Filling(std::vector<std::vector<char>> &graph, int x, int y) {
    std::deque<std::pair<int, int>> q; // pair of (x,y)
    int size_x = graph[0].size();
    int size_y = graph.size();
    q.push_front(std::make_pair(y, x));
    while (!q.empty()) {
        std::pair<int, int> &pos = q.back();
        q.pop_back();
        int curY = pos.first;
        int curX = pos.second;
        graph[curY][curX] = 'Y';
        if (curX != size_x - 1 && graph[curY][curX + 1] == 'O') {
            q.push_front(std::make_pair(curY, curX + 1));
        }
        if (curY != size_y - 1 && graph[curY + 1][curX] == 'O') {
            q.push_front(std::make_pair(curY + 1, curX));
        }
        if (curX != 0 && graph[curY][curX - 1] == 'O') {
            q.push_front(std::make_pair(curY, curX - 1));
        }
        if (curY != 0 && graph[curY - 1][curX] == 'O') {
            q.push_front(std::make_pair(curY - 1, curX));
        }
    }
}

/*
 * The graph is using 'X' and 'O' to represent earth and lake respectively, this function is to find the lakes inside
 * the map and fill them with 'X'
 */
void surroundedRegion(std::vector<std::vector<char>> &graph) {
    int x, y;
    int size_x = graph[0].size();
    int size_y = graph.size();

    for (x = 0; x < size_x - 1; x++) {
        if (graph[0][x] == 'O')
            BFS_Filling(graph, x, 0);
        if (graph[size_y - 1][x] == 'O')
            BFS_Filling(graph, x, size_y - 1);
    }
    for (y = 0; y < size_y - 1; y++) {
        if (graph[y][0] == 'O')
            BFS_Filling(graph, 0, y);
        if (graph[y][size_x - 1] == 'O')
            BFS_Filling(graph, size_x - 1, y);
    }

    for (x = 0; x < size_x; x++) {
        for (y = 0; y < size_y; y++) {
            if (graph[y][x] == 'O')
                graph[y][x] = 'X';
            else if (graph[y][x] == 'Y')
                graph[y][x] = 'O';
        }
    }
}

void testSurroundedRegion() {
    std::vector<std::vector<char>> graph = {{'X', 'X', 'X', 'X'},
                                            {'X', 'O', 'O', 'X'},
                                            {'X', 'X', 'O', 'X'},
                                            {'X', 'O', 'X', 'X'}};
    surroundedRegion(graph);
}

void testWordLadder() {
    std::set<std::string> dict = {"hot", "dot", "dog", "lot", "log"};
    std::vector<std::string> path;
    std::cout << wordLadder("hit", "cog", dict, path) << std::endl;
    std::for_each(path.begin(), path.end(), [](std::string str) { std::cout << str << " "; });
}

void testGraphMST_Prim() {
    Graph graph(6, false);
    graph.addLinesWithWeights({{0, 1, 10},
                               {0, 4, 45},
                               {0, 3, 30},
                               {3, 5, 20},
                               {1, 5, 25},
                               {1, 4, 40},
                               {4, 5, 55},
                               {2, 4, 35},
                               {1, 2, 50},
                               {2, 5, 15}});
    std::vector<Edge> result;
    bool suc = graph.MST_Prim(result);
    std::for_each(result.begin(), result.end(),
                  [](Edge e) { std::cout << "(" << e.i + 1 << ", " << e.j + 1 << "): " << e.weight << std::endl; });
}

void testGraphMST_Kruskal() {
    Graph graph(6, false);
    graph.addLinesWithWeights({{0, 1, 10},
                               {0, 4, 45},
                               {0, 3, 30},
                               {3, 5, 20},
                               {1, 5, 25},
                               {1, 4, 40},
                               {4, 5, 55},
                               {2, 4, 35},
                               {1, 2, 50},
                               {2, 5, 15}});
    std::vector<Edge> result;
    graph.MST_Kruskal(result);
    std::for_each(result.begin(), result.end(),
                  [](Edge e) { std::cout << "(" << e.i + 1 << ", " << e.j + 1 << "): " << e.weight << std::endl; });
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

    std::cout << std::endl;
    std::vector<int> shortestLen_Bellman, pre_Bellman;
    graph.shortestPath_BellmanFord(1, shortestLen_Bellman, pre_Bellman);
    graph.printShortestPath_BellmanFord(1, shortestLen_Bellman, pre_Bellman);
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
    testSurroundedRegion();
    return 0;
}
