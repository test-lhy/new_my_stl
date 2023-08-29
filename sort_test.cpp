//
// Created by lhy31 on 2023/8/29.
//
#include <iostream>
#include <vector>
#include "sort.h"
using namespace lhy;
int main() {

    vector<int> test{5, 3, 1, 4, 2};

    // Test Quick Sort
    Sort(test.begin(),test.end(), lhy::SortType::QUICK_SORT);
    for (int i = 0; i < test.size() - 1; i++) {
        if (test[i] > test[i + 1]) {
            std::cout << "Quick Sort failed!" << std::endl;
            return 1;
        }
    }
}