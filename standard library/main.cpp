#include <iostream>
#include <algorithm>
#include <numeric>
#include <queue>
#include <deque>
#include <stack>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <array>
#include <list>

using namespace std;

// Non-modifying sequence operations:
void testNonModify() {
    vector<int> a = {1, 2, 3, 4, 5};
    cout << all_of(a.begin(), a.end(), [](int i) -> bool { return i > 4; });
    cout << all_of(a.begin(), a.end(), [](int i) -> bool { return i > 0; });

    cout << any_of(a.begin(), a.end(), [](int i) -> bool { return i > 4; });
    cout << any_of(a.begin(), a.end(), [](int i) -> bool { return i > 0; });

    auto cmp = [](int i) -> bool { return i < -4; };
    cout << none_of(a.begin(), a.end(), [](int i) -> bool { return i < -4; });
    cout << none_of(a.begin(), a.end(), [](int i) -> bool { return i > 3; });
    cout << none_of(a.begin(), a.end(), cmp);

    int sum = 0;
    for_each(a.begin(), a.end(), [&](int i) { sum += i; });
    cout << "sum: " << sum << endl;

    array<int, 5> b = {1, 2, 2, 3, 5};
    cout << distance(b.begin(), find(b.begin(), b.end(), 2)) << endl;
    cout << *find(b.begin(), b.end(), 2) << endl;
    cout << *find_if(b.begin(), b.end(), cmp) << endl;
    cout << *find_if_not(b.begin(), b.end(), [](int i) { return i < 5; }) << endl;

    int myints[] = {1, 2, 3, 4, 5, 1, 2, 3, 4, 5};
    vector<int> haystack(myints, myints + 10);
    string pattern = "abc";
    string text = "aaaaabcbcabc";

    int needle1[] = {1, 2, 3};
    auto it = find_end(haystack.begin(), haystack.end(), needle1, needle1 + 3, [](int i, int j) { return i == j; });
    cout << "it is:" << distance(haystack.begin(), it) << *it << endl;
    int needle2[] = {1, 2, 3, 5};
    auto it2 = find_end(haystack.begin(), haystack.end(), needle2, needle2 + 4, [](int i, int j) { return i == j; });
    cout << "it2 is:" << distance(haystack.begin(), it2) << " " << *it2 << endl;
    int needle3[] = {2, 3, 4, 5};
    // 只需要找到第一个
    auto it3 = find_first_of(haystack.begin(), haystack.end(), needle3, needle3 + 4,
                             [](int i, int j) { return i == j; });
    cout << "it3 is:" << distance(haystack.begin(), it3) << " " << *it3 << endl;
    // 找到最后一个全匹配的开头字母
    auto it4 = find_end(text.begin(), text.end(), pattern.begin(), pattern.end(),
                        [](char i, char j) { return i == j; });
    cout << "it4 is:" << distance(text.begin(), it4) << " " << *it4 << endl;

    // An iterator to the first element of the first pair of matching consecutive elements in the range [first,last).
    // If no such pair is found, the function returns last.
    vector<int> myints2 = {5, 20, 5, 30, 30, 20, 10, 10, 20};
    auto mycmp = [](int i, int j) -> bool { return i == j; };
    it = std::adjacent_find(myints2.begin(), myints2.end(), mycmp); // 可以加上自定义mycmp，参数两个, 也可以省略
    cout << distance(myints2.begin(), it) << ": " << *it << endl;

    int mycount = count(myints2.begin(), myints2.end(), 20);
    cout << "mycount:" << mycount << endl;
    int mycount2 = count_if(myints2.begin(), myints2.end(), [](int i) -> bool { return i > 30; });
    cout << "mycount:" << mycount2 << endl;

    vector<int> myvector;
    for (int i = 1; i < 6; i++) myvector.push_back(i * 10); // myvector: 10 20 30 40 50

    int myints3[] = {10, 20, 80, 320, 1024};                //   myints: 10 20 80 320 1024
    auto mypair = mismatch(myvector.begin(), myvector.end(), myints3, mycmp);// 可以加上自定义mycmp，参数两个, 也可以省略
    cout << "First mismatching elements: " << *mypair.first;
    cout << " and " << *mypair.second << '\n';

    // true if all the elements in the range [first1,last1) compare equal to those of the range starting at first2,
    // and false otherwise.
    cout << equal(myvector.begin(), myvector.end(), myints3, mycmp) << endl;

    // search: Searches the range [first,last) for a sequence of count elements, each comparing equal
    // to val (or for which pred returns true).
    // The function returns an iterator to the first of such elements, or last if no such sequence is found.
    cout << distance(text.begin(), search(text.begin(), text.end(), pattern.begin(), pattern.end(),
                                          [](char i, char j) {
                                              return i == j;
                                          })) << endl; // last function is pred, can be omitted
    // search_n: The function returns an iterator to the first of such elements, or last if no such sequence is found.
    cout << distance(myints2.begin(), search_n(myints2.begin(), myints2.end(), 2, 30)) << endl;

    // is_permutation, O(n^2)
    array<int, 5> foo = {1, 2, 3, 4, 5};
    array<int, 5> bar = {3, 1, 4, 5, 2};
    if (is_permutation(foo.begin(), foo.end(), bar.begin()))
        cout << "foo and bar contain the same elements.\n";


}

void testBinarySearch() {
    vector<int> a = {0, 1, 2, 3, 3, 4, 4, 4, 4, 4, 5, 6, 7, 7, 9, 9};
    cout << distance(a.begin(), lower_bound(a.begin(), a.end(), 4)) << endl; // the first >= target
    cout << distance(a.begin(), upper_bound(a.begin(), a.end(), 4)) << endl; // the first > target
    cout << distance(a.begin(), lower_bound(a.begin(), a.end(), 3) - 1) << endl; // the last < target
    cout << distance(a.begin(), upper_bound(a.begin(), a.end(), 7) - 1) << endl; // the last <= target

    reverse(a.begin(), a.end());

    cout << distance(a.begin(), lower_bound(a.begin(), a.end(), 4, greater<int>())) << endl; // the first <= target
    cout << distance(a.begin(), upper_bound(a.begin(), a.end(), 4, greater<int>())) << endl; // the first < target
    cout << distance(a.begin(), lower_bound(a.begin(), a.end(), 3, greater<int>()) - 1) << endl; // the last > target
    cout << distance(a.begin(), upper_bound(a.begin(), a.end(), 7, greater<int>()) - 1) << endl; // the last <= target

    // equal_range: first is the lower_bound, second is the upper_bound
    a = {0, 1, 2, 3, 3, 4, 4, 4, 4, 4, 5, 6, 7, 7, 9, 9};
    auto range = equal_range(a.begin(), a.end(), 4, [](int i, int j) { return i < j; });
    cout << range.first - a.begin() << " " << range.second - a.begin() << endl;// 差就是个数

    // only used for judge whether an element exists
    cout << binary_search(a.begin(), a.end(), 8, [](int i, int j) -> bool { return i < j; }) << endl;
    cout << binary_search(a.begin(), a.end(), 6) << endl;
}


int main() {
    testBinarySearch();
    return 0;
}