#include <iostream>
#include <vector>
#include <unordered_map>

/*
 * sort an array in increasing order
 *
 */
class SortAlgorithms {
public:
    std::vector<int> arr;
    int count; // this is the number of reverse order pairs in the original array

    SortAlgorithms(std::vector<int> arr) : arr(arr), count(0) {}

    void mergeSort() {
        _mergeSort(arr);
    }

    void countingSort();

    void bubbleSort();

    /*
     * Use minHeap
     */
    void heapSort() {
        _heapSort(arr, arr.size());
    }

private:
    void _mergeSort(std::vector<int> &array) {
        int len = array.size();
        if (len < 2) {
            return;
        }
        int mid = len / 2;
        std::vector<int> a(array.begin(), array.begin() + mid);
        std::vector<int> b(array.begin() + mid, array.end());
        _mergeSort(a);
        _mergeSort(b);
        array = _merge(a, b, count);
    }

    std::vector<int> _merge(std::vector<int> a, std::vector<int> b, int &count);

    void _heapSort(std::vector<int> &a, int k); // the top kth biggest
    void _maxHeap(std::vector<int> &a, int n, int size);
};


void SortAlgorithms::_maxHeap(std::vector<int> &a, int n, int size) {
    int nChild = 2 * n + 1; // left child
    int t;
    while (nChild < size) {
        if (nChild + 1 < size && a[nChild + 1] > a[nChild])
            nChild++;
        if (a[nChild] < a[n]) // children are smaller than parent, finished
            break;
        std::swap(a[nChild], a[n]);
        n = nChild;
        nChild = 2 * n + 1;
    }
}

void SortAlgorithms::_heapSort(std::vector<int> &a, int k) {
    int size = (int) a.size();
    // make maxheap
    for (int i = size / 2 - 1; i >= 0; i--) {
        _maxHeap(a, i, size);
    }
    int s = size - k;
    while (size > s) {
        std::swap(a[0], a[size - 1]);
        size--;
        _maxHeap(a, 0, size);
    }
}

void SortAlgorithms::bubbleSort() {
    int size = (int) arr.size();
    for (int i = 0; i < size - 1; i++) {
        bool bubbled = false;
        for (int j = size - 1; j > i; j--) {
            if (arr[j] < arr[j - 1]) {
                std::swap(arr[j], arr[j - 1]);
                bubbled = true;
            }
        }
        if (!bubbled)
            break;
    }
}

void SortAlgorithms::countingSort() {
    int min = INT_MAX, max = INT_MIN;
    int size = (int) arr.size();
    std::for_each(arr.begin(), arr.end(), [&](int i) {
        min = min > i ? i : min;
        max = max < i ? i : max;
    });
    std::vector<int> cnt(max - min + 1, 0);
    std::for_each(arr.begin(), arr.end(), [&](int k) {
        cnt[k - min]++;
    });

    int sum = 0;
    std::for_each(cnt.begin(), cnt.end(), [&](int &i) {
        sum += i;
        i = sum;
    });
    std::vector<int> result(size, -1);
    for (int i = size - 1; i >= 0; i--) {
        result[--cnt[arr[i] - min]] = arr[i];
    }
    arr = result;
}

std::vector<int> SortAlgorithms::_merge(std::vector<int> a, std::vector<int> b, int &count) {
    int size_a = (int) a.size();
    int size_b = (int) b.size();
    std::vector<int> result(size_a + size_b, INT_MAX);
    int i = 0, j = 0, cnt = 0;
    while (i != size_a && j != size_b) {
        if (a[i] < b[j])
            result[cnt++] = a[i++];
        else {
            result[cnt++] = b[j++];
            count += size_a - i;
        }
    }
    while (i != size_a) {
        result[cnt++] = a[i++];
    }
    while (j != size_b) {
        result[cnt++] = b[j++];
    }
    return result;
}

void testMergeSort() {
    SortAlgorithms algorithms({2, 4, 6, 34, 9, 7, 5, 44, 3});
    algorithms.mergeSort();
    std::for_each(algorithms.arr.begin(), algorithms.arr.end(), [](int i) { std::cout << i << " "; });
    std::cout << std::endl << algorithms.count;

}

void testCountingSort() {
    SortAlgorithms algorithms({2, 4, 6, 34, 9, 7, 5, 44, 3});
    algorithms.countingSort();
    std::for_each(algorithms.arr.begin(), algorithms.arr.end(), [](int i) { std::cout << i << " "; });
}

void testBubbleSort() {
    SortAlgorithms algorithms({2, 343434, 4, 6, 34, 9, 7, 5, 44, 3});
    algorithms.bubbleSort();
    std::for_each(algorithms.arr.begin(), algorithms.arr.end(), [](int i) { std::cout << i << " "; });
}

void testHeapSort() {
    SortAlgorithms algorithms({2, 343434, 4, 6, 34, 9, 7, 5, 44, 3});
    algorithms.heapSort();
    std::for_each(algorithms.arr.begin(), algorithms.arr.end(), [](int i) { std::cout << i << " "; });
}

void twoSum(std::vector<int> arr, int sum) {
    SortAlgorithms algorithms(arr);
    algorithms.mergeSort();
    arr = algorithms.arr;
    int size = (int) arr.size();
    int i = 0;
    int j = size - 1;
    while (i < j) {
        if (arr[i] + arr[j] > sum) {
            j--;
        } else if (arr[i] + arr[j] < sum) {
            i++;
        } else {
            std::cout << "pair:(" << arr[i] << ", " << arr[j] << ")" << std::endl;
            i++;
            j--;
        }
    }
}

void testTwoSum() {
    twoSum({3, 11, 9, 7, 0, 14, 17, 16}, 20);
}


int main() {
    testHeapSort();
    return 0;
}