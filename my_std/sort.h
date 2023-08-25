//
// Created by lhy31 on 2023/8/25.
//

#ifndef MY_STL_SORT_H
#define MY_STL_SORT_H
#include <cmath>
#include <functional>
#include <list>
#include <stdexcept>

#include "../basic.h"
#include "priority_queue.h"
#include "random.h"
#include "vector.h"

namespace lhy {
enum SortType {
  QUICK_SORT,
  INSERT_SORT,
  MERGE_SORT,
  COUNT_SORT,
  BUCKET_SORT,
  HEAP_SORT,
  INTRO_SORT,
  SHELL_SORT,
  TOURNAMENT_SORT,
  RADIX_SORT
};
class ShellGapGenerationBase {
 public:
  ShellGapGenerationBase() = default;
  virtual ~ShellGapGenerationBase() = default;
  virtual Index Get() = 0;
  virtual void Up() = 0;
  virtual void Down() = 0;
  virtual bool DownIf() = 0;
};
class ShellNormalGapGeneration : public ShellGapGenerationBase {
 public:
  static Index number;
  ShellNormalGapGeneration() { number = 1; }
  Index Get() override { return number; }
  void Up() override { number = number * 3 + 1; }
  void Down() override { number = (number - 1) / 3; }
  bool DownIf() override {
    if (number >= 1) {
      return true;
    } else {
      return false;
    }
  }
};
}
#endif  // MY_STL_SORT_H