#include <iostream>
#include <vector>


// 循环左移算法， 时间复杂度：O(N), 空间复杂度: O(1)
void ReverseString(std::string &str, size_t from, size_t to) {
    while (from < to) {
        char tmp = str[from];
        str[from++] = str[to];
        str[to--] = tmp;
    }
}

void leftRotateString(std::string &s, size_t m) {
    size_t len = s.length();
    m %= len;
    ReverseString(s, 0, m - 1);
    ReverseString(s, m, len - 1);
    ReverseString(s, 0, len - 1);
}

void testLeftRotateString() {
    std::string test("abc1defg");
    leftRotateString(test, 4); // left rotate by 4 digits
    std::cout << test;
}

// longest common subsequence 最长公共子序列
// using DP, time complexity: O(N^2)
std::string LCS(const std::string str1, const std::string str2) {
    size_t len1 = str1.length();
    size_t len2 = str2.length();
    std::vector<std::vector<int>> chess(len1 + 1, std::vector<int>(len2 + 1));
    for (size_t i = 0; i <= len1; i++) {
        chess[i][0] = 0;
    }
    for (size_t i = 0; i <= len2; i++) {
        chess[0][i] = 0;
    }
    // calculate the chess table
    for (size_t i = 1; i <= len1; i++) {
        for (size_t j = 1; j <= len2; j++) {
            if (str1[i - 1] == str2[j - 1]) {
                chess[i][j] = chess[i][j - 1] + 1;
            } else {
                chess[i][j] = std::max(chess[i][j - 1], chess[i - 1][j]);
            }
        }
    }
    std::string seq;
    size_t i = len1, j = len2;
    while (i != 0 && j != 0) {
        if (str1[i - 1] == str2[j - 1]) {
            seq.push_back(str1[i - 1]);
            i--;
            j--;
        } else {
            if (chess[i - 1][j - 2] < chess[i - 2][j - 1]) {
                i--;
            } else {
                j--;
            }
        }
    }
    std::reverse(seq.begin(), seq.end());
    return seq;
}

// little bug now
void printAllLCS(std::vector<std::vector<int>> chess, std::string curSeq, std::string str1, std::string str2,
                 size_t i, size_t j) {
    while (i != 0 && j != 0) {
        if (str1[i - 1] == str2[j - 1]) {
            curSeq.push_back(str1[i - 1]);
            i--;
            j--;
        } else {
            if (chess[i - 1][j - 2] < chess[i - 2][j - 1]) {
                i--;
            } else if (chess[i - 2][j - 1] < chess[i - 1][j - 2]) {
                j--;
            } else { // chess[i - 2][j - 1] == chess[i - 1][j - 2]
                printAllLCS(chess, curSeq, str1, str2, i - 1, j);
                printAllLCS(chess, curSeq, str1, str2, i, j - 1);
                return;
            }
        }
    }
    std::reverse(curSeq.begin(), curSeq.end());
    std::cout << curSeq << std::endl;
}

void calAllLCS(std::string str1, std::string str2) {
    size_t len1 = str1.length();
    size_t len2 = str2.length();
    std::vector<std::vector<int>> chess(len1 + 1, std::vector<int>(len2 + 1));
    for (size_t i = 0; i <= len1; i++) {
        chess[i][0] = 0;
    }
    for (size_t i = 0; i <= len2; i++) {
        chess[0][i] = 0;
    }
    // calculate the chess table
    for (size_t i = 1; i <= len1; i++) {
        for (size_t j = 1; j <= len2; j++) {
            if (str1[i - 1] == str2[j - 1]) {
                chess[i][j] = chess[i][j - 1] + 1;
            } else {
                chess[i][j] = std::max(chess[i][j - 1], chess[i - 1][j]);
            }
        }
    }
    std::string seq;
    printAllLCS(chess, seq, str1, str2, len1, len2);
}

void testLCS() {
    std::string s1 = "abc";
    std::string s2 = "abc";
    std::cout << LCS(s1, s2) << std::endl;
}

void testAllLCS() {
    std::string s1 = "abcde";
    std::string s2 = "abedc";
    calAllLCS(s1, s2);
}

int main() {
    testAllLCS();
    return 0;
}