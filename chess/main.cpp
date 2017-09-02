#include <iostream>
#include <vector>

class NQueen {
private:
    int n;
    std::vector<bool> column;
    std::vector<bool> mainDiagonal;
    std::vector<bool> minorDiagonal;
    std::vector<std::vector<int>> paths;

public:
    NQueen(int num) : n(num) {
        column.resize(n, false);
        mainDiagonal.resize(2 * n - 1, false);
        minorDiagonal.resize(2 * n - 1, false);

    }

    void calcNQueen();

    int getNumOfCases();

private:
    void _calcNQueen(std::vector<int> &path, int row);
};

void NQueen::calcNQueen() {
    std::vector<int> path(n, -1);
    _calcNQueen(path, 0);
}

void NQueen::_calcNQueen(std::vector<int> &path, int row) {
    if (row == n)
        paths.push_back(path);
    for (int col = 0; col < n; col++) {
        if (!column[col] && !mainDiagonal[row + col] && !minorDiagonal[n + row - col]) {
            path[row] = col;
            column[col] = true;
            mainDiagonal[row + col] = true;
            minorDiagonal[n + row - col] = true;
            _calcNQueen(path, row + 1);

            column[col] = false;
            mainDiagonal[row + col] = false;
            minorDiagonal[n + row - col] = false;
        }
    }
}

int NQueen::getNumOfCases() {
    return paths.size();
}

void testNQueen() {
    NQueen queen(8);
    queen.calcNQueen();
    std::cout << queen.getNumOfCases() << std::endl;
}

/*
 * The array s is a 9 * 9 array, all unfilled places are marked as '0', others are pre-filled blocks (with 1 to 9)
 */
bool sodoku(std::vector<std::vector<int>> &s) {
    int bias_i[] = {-1, 0, 1, -1, 0, 1, -1, 0, 1};
    int bias_j[] = {-1, -1, -1, 0, 0, 0, 1, 1, 1};
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (s[i][j] == 0) {
                // find the possible number for this (i,j)
                int ci = i / 3 * 3 + 1;
                int cj = j / 3 * 3 + 1;
                std::vector<bool> occupy(9, false);
                for (int k = 0; k < 9; k++)
                    if (s[i][k] != 0)
                        occupy[s[i][k] - 1] = true;
                for (int k = 0; k < 9; k++)
                    if (s[k][j] != 0)
                        occupy[s[k][j] - 1] = true;
                for (int k = 0; k < 9; k++)
                    if (s[ci + bias_i[k]][cj + bias_j[k]] != 0)
                        occupy[s[ci + bias_i[k]][cj + bias_j[k]] - 1] = true;
                for (int k = 0; k < 9; k++) {
                    s[i][j] = k + 1;
                    if (!occupy[k] && sodoku(s)) {
                        return true;
                    }
                    s[i][j] = 0; // recall
                }
                return false;
            }
        }
    }
    return true;
}


void testSodoku() {
    std::vector<std::vector<int>> chess = {{0, 4, 2, 0, 6, 3, 0, 0, 9},
                                           {6, 0, 0, 0, 1, 0, 0, 0, 5},
                                           {3, 0, 0, 0, 2, 0, 4, 8, 0},
                                           {1, 0, 0, 5, 0, 2, 6, 0, 8},
                                           {4, 0, 0, 0, 0, 7, 0, 0, 1},
                                           {9, 0, 5, 6, 0, 0, 0, 0, 7},
                                           {0, 3, 6, 0, 5, 0, 0, 0, 2},
                                           {2, 0, 0, 0, 7, 0, 0, 0, 4},
                                           {7, 0, 0, 2, 9, 0, 8, 5, 0}};
    bool solve = sodoku(chess);
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            std::cout << chess[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

/*
 * The array s is a 9 * 9 array, all unfilled places are marked as '0', others are pre-filled blocks (with 1 to 9)
 */
int a = 1;

bool sodokuAll(std::vector<std::vector<int>> &s, std::vector<std::vector<std::vector<int>>> &allSolution) {
    int bias_i[] = {-1, 0, 1, -1, 0, 1, -1, 0, 1};
    int bias_j[] = {-1, -1, -1, 0, 0, 0, 1, 1, 1};
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (s[i][j] == 0) {
                // find the possible number for this (i,j)
                int ci = i / 3 * 3 + 1;
                int cj = j / 3 * 3 + 1;
                std::vector<bool> occupy(9, false);
                for (int k = 0; k < 9; k++)
                    if (s[i][k] != 0)
                        occupy[s[i][k] - 1] = true;
                for (int k = 0; k < 9; k++)
                    if (s[k][j] != 0)
                        occupy[s[k][j] - 1] = true;
                for (int k = 0; k < 9; k++)
                    if (s[ci + bias_i[k]][cj + bias_j[k]] != 0)
                        occupy[s[ci + bias_i[k]][cj + bias_j[k]] - 1] = true;
                for (int k = 0; k < 9; k++) {
                    s[i][j] = k + 1;
                    if (!occupy[k]) {
                        if (sodokuAll(s, allSolution))
                            allSolution.push_back(s);
                    }
                    s[i][j] = 0; // recall
                }
                return false;
            }
        }
    }
    return true;
}

void testSodokuAll() {
    std::vector<std::vector<int>> chess = {{0, 4, 2, 0, 6, 3, 0, 0, 9},
                                           {6, 0, 0, 0, 1, 0, 0, 0, 5},
                                           {0, 0, 0, 0, 2, 0, 4, 8, 0},
                                           {0, 0, 0, 5, 0, 2, 6, 0, 8},
                                           {0, 0, 0, 0, 0, 7, 0, 0, 1},
                                           {0, 0, 5, 6, 0, 0, 0, 0, 7},
                                           {0, 3, 6, 0, 5, 0, 0, 0, 2},
                                           {2, 0, 0, 0, 7, 0, 0, 0, 4},
                                           {7, 0, 0, 2, 9, 0, 8, 5, 0}};
    std::vector<std::vector<std::vector<int>>> allSolution;
    bool haveSolution = sodokuAll(chess, allSolution);
    for (int k = 0; k < allSolution.size(); k++) {
        std::vector<std::vector<int>> solution = allSolution[k];
        std::cout << "The " << k + 1 << "th solution:" << std::endl;
        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 9; j++) {
                std::cout << solution[i][j] << " ";
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }
}

bool _traverseChessBoard(std::vector<std::vector<int>> &chess, int row, int col, int steps) {
    int bias_row[] = {-1, -2, -2, -1, 1, 2, 2, 1};
    int bias_col[] = {-2, -1, 1, 2, 2, 1, -1, -2};
    int rows = chess.size();
    int cols = chess.size();
    if (steps == rows * cols - 1) {
        return true;
    }

    for (int k = 0; k < 8; k++) {
        int i = row + bias_row[k];
        int j = col + bias_col[k];
        if (i >= 0 && i < rows && j >=0 && j < cols && chess[i][j] == -1) {
            chess[i][j] = steps + 1;
            if (_traverseChessBoard(chess, i, j, steps + 1)) {
                return true;
            }
            chess[i][j] = -1;
        }
    }
    return false;
}

/*
 * A game solution, use a knight to traverse all chess board start from the top-left corner
 */
bool traverseChessBoard(std::vector<std::vector<int>> &chess) {
    return _traverseChessBoard(chess, 0, 0, 0);
}

void testTraverseChessBoard() {
    int rows = 8, cols = 8;
    std::vector<std::vector<int>> chess(rows, std::vector<int>(cols, -1));
    chess[0][0] = 0;
    traverseChessBoard(chess);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            std::cout << chess[i][j] << " ";
        }
        std::cout << std::endl;
    }
}


class chessPosition {
public:
    int row;
    int col;
    int childrenNum;

    chessPosition(int row, int col) : row(row), col(col), childrenNum(0) {};
};

std::vector<chessPosition> sortNext(std::vector<std::vector<int>> chess,
                                    std::vector<chessPosition> positions, int steps) {
    int bias_row[] = {-1, -2, -2, -1, 1, 2, 2, 1};
    int bias_col[] = {-2, -1, 1, 2, 2, 1, -1, -2};
    int len = positions.size();
    int rows = chess.size();
    int cols = chess[0].size();
    if (steps == rows * cols - 2)
        return positions;
    for (int n = 0; n < len; n++) {
        for (int k = 0; k < 8; k++) {
            int i = positions[n].row + bias_row[k];
            int j = positions[n].col + bias_col[k];
            if (i >= 0 && i < rows && j >= 0 && j < cols && chess[i][j] == -1) {
                positions[n].childrenNum++;
            }
        }
    }
    std::sort(positions.begin(), positions.end(),
              [](chessPosition pos1, chessPosition pos2) { return pos1.childrenNum < pos2.childrenNum; });
    positions.erase(std::remove_if(positions.begin(), positions.end(), [](chessPosition p) { return p.childrenNum == 0; }),
                    positions.end());
    return positions;
}

bool _traverseChessBoard2(std::vector<std::vector<int>> &chess, int row, int col, int steps) {
    int bias_row[] = {-1, -2, -2, -1, 1, 2, 2, 1};
    int bias_col[] = {-2, -1, 1, 2, 2, 1, -1, -2};
    int rows = chess.size();
    int cols = chess.size();

    if (steps == rows * cols - 1) {
        return true;
    }

    std::vector<chessPosition> positions;
    for (int k = 0; k < 8; k++) {
        int i = row + bias_row[k];
        int j = col + bias_col[k];
        if (i >= 0 && i < rows && j >= 0 && j < cols && chess[i][j] == -1) {
            positions.push_back(chessPosition(i, j));
        }
    }

    std::vector<chessPosition> validPositions = sortNext(chess, positions, steps);
    for (auto it = validPositions.begin(); it != validPositions.end(); it++) {
        int i = it->row;
        int j = it->col;
        chess[i][j] = steps + 1;
        if (_traverseChessBoard2(chess, i, j, steps + 1)) {
            return true;
        }
        chess[i][j] = -1;
    }
    return false;
}

/*
 * A game solution, use a knight to traverse all chess board start from the top-left corner
 * version 2: it is much faster since I have used heuristic search
 */
bool traverseChessBoard2(std::vector<std::vector<int>> &chess) {
    return _traverseChessBoard2(chess, 0, 0, 0);
}

void testTraverseChessBoard2() {
    int rows = 8, cols = 8;
    std::vector<std::vector<int>> chess(rows, std::vector<int>(cols, -1));
    chess[0][0] = 0;
    traverseChessBoard2(chess);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            std::cout << chess[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

int main() {
    testTraverseChessBoard2();
    return 0;
}
