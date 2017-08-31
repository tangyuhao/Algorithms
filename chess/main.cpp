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
    int i = 0;
    std::for_each(paths.begin(), paths.end(), [&](std::vector<int> p) { i++; });
    return i;
}

void testNQueen() {
    NQueen queen(8);
    queen.calcNQueen();
    std::cout << queen.getNumOfCases() << std::endl;
}

int main() {
    testNQueen();
    return 0;
}
