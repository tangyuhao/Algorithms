#include <iostream>
#include <queue>
#include <stack>

#define N 5
#define N2 16

// use topologic sort to look for cycles in a graph
void topologic(int *toposort, int *indegree, int graph[][N]) {
    std::queue<int> q;
    for (int i = 0; i < N; i++) {
        if (indegree[i] == 0) {
            q.push(i);
        }
    }
    int curPoint, outCnt = 0;
    while (!q.empty()) {
        curPoint = q.front();
        q.pop();
        toposort[outCnt++] = curPoint;
        for (int i = 0; i < N; i++) {
            if (graph[i][curPoint] != 0) {
                indegree[i]--;
                if (indegree[i] == 0) {
                    q.push(i);
                }
            }
        }
    }
    for (int k = 0; k < outCnt; k++) {
        std::cout << toposort[k] << " ";
    }
    std::cout << std::endl;
    if (outCnt < N)
        std::cout << "this graph has cycles." << std::endl;
}

void testTopologic() {
    // graph[i][j] means j to i
    int graph[N][N]{{0, 1, 1, 0, 0},
                    {0, 0, 0, 1, 0},
                    {1, 1, 0, 0, 1},
                    {0, 0, 0, 0, 0},
                    {1, 1, 1, 1, 0}};
    int indegree[N] = {0, 0, 0, 0, 0};
    int toporesult[N];
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            indegree[i] += graph[i][j];
        }
    }
    topologic(toporesult, indegree, graph);
}

int calMinRoutesV2(int graph[N2][N2]) {
    int step[N2];
    int numMinRoutes[N2];
    for (int i = 0; i < N2; i++) {
        step[i] = 0;
    }
    for (int i = 0; i < N2; i++) {
        numMinRoutes[i] = 0;
    }
    numMinRoutes[0] = 1;
    int min, from;
    std::queue<int> q;
    q.push(0);

    while (!q.empty()) {
        from = q.front();
        q.pop();
        min = step[from] + 1;

        for (int i = 1; i < N2; i++) {
            if (graph[from][i] == 1) {
                if (step[i] == 0 || step[i] > min) {
                    step[i] = min;
                    numMinRoutes[i] = numMinRoutes[from];
                    q.push(i);
                } else if (step[i] == min) {
                    numMinRoutes[i] += numMinRoutes[from];
                }
            }
        }
    }
    return numMinRoutes[N2 - 1];
}

void testCalNumMinRoutes() {
    int graph[N2][N2];
    for (int i = 0; i < N2; i++)
        for (int j = 0; j < N2; j++)
            graph[i][j] = 0;

    graph[0][1] = graph[0][4] = 1;
    graph[1][5] = graph[1][0] = graph[1][2] = 1;
    graph[2][1] = graph[2][6] = graph[2][3] = 1;
    graph[3][2] = graph[3][7] = 1;
    graph[4][0] = graph[4][5] = 1;
    graph[5][1] = graph[5][4] = graph[5][6] = graph[5][9] = 1;
    graph[6][2] = graph[6][7] = graph[6][5] = graph[6][10] = 1;
    graph[7][3] = graph[7][6] = 1;
    graph[8][9] = graph[8][12] = 1;
    graph[9][8] = graph[9][13] = graph[9][10] = 1;
    graph[10][9] = graph[10][14] = graph[10][11] = 1;
    graph[11][10] = graph[11][15] = 1;
    graph[12][8] = graph[12][13] = 1;
    graph[13][9] = graph[13][12] = graph[13][14] = 1;
    graph[14][10] = graph[14][13] = graph[14][15] = 1;
    graph[15][11] = graph[15][14] = 1;

    std::cout << calMinRoutesV2(graph) << std::endl;
}

/*
 * get the length of longest parentheses
 * Time complexity: O(n), Space Complexity: O(n)
 */
int getLongestParenthese(const char *p) {
    std::stack<int> q;
    int answer = 0;
    int start = -1;
    int size = (int) strlen(p);
    for (int i = 0; i < size; i++) {
        if (p[i] == '(') {
            q.push(i);
        } else {
            if (q.empty()) {
                start = i;
            } else {
                q.pop();
                if (q.empty()) {
                    answer = std::max(answer, i - start);
                } else {
                    answer = std::max(answer, i - q.top());
                }
            }
        }
    }
    return answer;
}

int getLongestParenthese2(const char *p) {
    int deep = 0;
    int start = -1, answer = 0;
    int size = (int) strlen(p);

    for (int i = 0; i < size; i++) {
        if (p[i] == '(') {
            deep++;
        } else {
            deep--;
            if (deep == 0) {
                answer = std::max(answer, i - start);
            } else if (deep < 0) {
                deep = 0;
                start = i;
            }
        }
    }

    deep = 0;
    start = size;

    for (int i = size - 1; i >= 0; i--) {
        if (p[i] == ')') {
            deep++;
        } else {
            deep--;
            if (deep == 0) {
                answer = std::max(answer, start - i);
            } else if (deep < 0) {
                deep = 0;
                start = i;
            }
        }
    }
    return answer;
}

void testGetLongestParenthese() {
    char *str = new char[100];
    std::cout << "Please input the parentheses" << std::endl;
    std::cin >> str;
    std::cout << "The longest parenthese is: " << getLongestParenthese2(str) << std::endl;
}

int main() {
    testGetLongestParenthese();
    return 0;
}