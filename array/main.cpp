#include <iostream>
#include <vector>
#include <numeric>


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
 * time complexity: O(log n), not definitely due to preprocessing for the array
 */
int findLocalMaximum(std::vector<int> arr) {
    arr.push_back(INT_MIN);
    arr.insert(arr.begin(), INT_MIN); // cost O(N)
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

/*
 * find the first lost integer start from 1
 * example: 1 2 3 4 5 7 8, then the first lost positive integer should be 6
 * note: the array is not garanteed to be sorted
 *
 * time complexity: O(N)
 * space complexity: O(1)
 */
int findFirstLostInteger(std::vector<int> arr) {
    int len = arr.size();
    int i;
    for (i = 0; i < len;) {
        if (arr[i] == i + 1) {
            i++;
        } else if (arr[i] < i + 1 || arr[i] > len - 1 || arr[i] == arr[arr[i] - 1]) {
            // delete
            arr[i] = arr[--len];
        } else { // arr[i] < i + 1
            //swap
            std::swap(arr[arr[i] - 1], arr[i]);
        }
    }
    return i + 1;
}

void testFindFirstLostInteger() {
    std::vector<int> arr({3, 5, 1, 2, -3, 7, 4, 8});
    std::cout << findFirstLostInteger(arr);
}

template<typename T>
std::vector<size_t> sort_indexes(const std::vector<T> &v) {
    std::vector<size_t> idx(v.size());
    std::iota(idx.begin(), idx.end(), 0);

    // sort indexes based on comparing values in v
    std::sort(idx.begin(), idx.end(), [&v](size_t i1, size_t i2) { return v[i1] < v[i2]; });
    return idx;
}

/*
 * find the subarray whose sum is most close to 0
 */
std::vector<int> findZeroSubArray(std::vector<int> arr) {
    std::vector<int> sumArr(arr.size(), 0);
    sumArr[0] = arr[0];
    for (int i = 1; i < arr.size(); i++) {
        sumArr[i] = sumArr[i - 1] + arr[i];
    }
    std::vector<size_t> idx = sort_indexes(sumArr);
    int min = std::abs(sumArr[idx[0]]), minIdx = 0;

    for (int i = 1; i < sumArr.size(); i++) {
        if (min > std::abs(sumArr[idx[i]] - sumArr[idx[i - 1]])) {
            minIdx = i;
            min = std::abs(sumArr[idx[i]] - sumArr[idx[i - 1]]);
        }
    }
    if (minIdx == 0) {
        return std::vector<int>(arr.begin(), arr.begin() + idx[0] + 1);
    } else {
        size_t i1 = std::min(idx[minIdx], idx[minIdx - 1]);
        size_t i2 = std::max(idx[minIdx], idx[minIdx - 1]);
        return std::vector<int>(arr.begin() + i1 + 1, arr.begin() + i2 + 1);
    }
}

void testFindZeroSubArray() {
    std::vector<int> arr({3, 5, 1, 2, -3, 7, 4, 8});
    std::vector<int> subArr = findZeroSubArray(arr);
    std::for_each(subArr.begin(), subArr.end(), [=](int i) { std::cout << i << " "; });
}

int main() {
    testFindZeroSubArray();
    return 0;
}