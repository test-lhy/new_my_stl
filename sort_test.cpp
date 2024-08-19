//
// Created by lhy31 on 2023/8/29.
//
#include "sort.h"

#include <time.h>  //引入头文件

#include <iostream>
#include <vector>

using namespace lhy;
int main() {
  auto enum_list = {QUICK_SORT,  INSERT_SORT, SHELL_SORT, MERGE_SORT, TIM_SORT,
                    BUCKET_SORT, COUNT_SORT,  HEAP_SORT,  RADIX_SORT, TOURNAMENT_SORT};
  for (auto &each : enum_list) {
    clock_t start, end;  // 定义clock_t变量
    vector<int> test{};
    for (int i = 0; i < 10000; ++i) {
      test.push_back(rand(-10000, 10000) * 1.0);
    }

    start = clock();  // 开始时间
    Sort(test.begin(), test.end(), each);
    end = clock();                                                                       // 结束时间
    std::cerr << "time = " << double(end - start) / CLOCKS_PER_SEC << "s" << std::endl;  // 输出时间（单位：ｓ）
    for (int i = 0; i < test.size() - 1; i++) {
      std::cerr << test[i] << "|" << test[i + 1] << " ";
      if (test[i] > test[i + 1]) {
        std::cerr << "Count Sort failed!" << std::endl;
    }
  }
  }
}