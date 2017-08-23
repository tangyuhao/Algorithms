#include <iostream>
#include <array>
#include <vector>


/*
 * Define the definite mode in an array:
 * 1. is a mode
 * 2. has appeared more than n/2 times
 *
 * Know: the input array has a definite mode.
 * Return: INT_MIN if definite mode not existing
 *
 */
int definiteMode(std::vector<int> arr) {
    int cnt = 0;
    int candidate;
    for (int i = 0; i < arr.size(); i++) {
        if (cnt != 0) {
            if (arr[i] == candidate) {
                cnt++;
            } else {
                cnt--;
            }
        } else {
            candidate = arr[i];
            cnt++;
        }
    }
    return candidate;
}

void testDefiniteMode() {
    std::vector<int> arr({1, 2, 1, 3, 1});
    std::cout << definiteMode(arr) << std::endl;
}

/*
 * assume that the neighbors in the array are not duplicate
 * return: a local maximum of the array
 * time complexity: O(log n)
 */
int findLocalMaximum(std::vector<int> arr) {
    arr.push_back(INT_MIN);
    arr.insert(arr.begin(), INT_MIN);
    int leftIdx = 0, rightIdx = arr.size();
    int mid;
    while (leftIdx < rightIdx - 1) {
        mid = (leftIdx + rightIdx) / 2;
        if (arr[mid] > arr[mid + 1]) {
            rightIdx = mid;
        } else {
            leftIdx = mid;
        }
    }
    return *std::max(arr.begin() + leftIdx, arr.begin() + rightIdx);
}

void testFindLocalMaximum() {
    std::vector<int> arr({1, 2, 3, 4, 5, 6, 5, 4, 3, 2, 1, 3, 5, 6, 4, 3, 4, 3});
    std::cout << findLocalMaximum(arr);
}

int main() {
    testFindLocalMaximum();
    return 0;
}