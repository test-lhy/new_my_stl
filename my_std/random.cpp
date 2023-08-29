//
// Created by lhy31 on 2023/8/22.
//
#include "random.h"
namespace lhy {
int64_t rand(int64_t a, int64_t b) {
  std::mt19937 ran(time(nullptr));
  return ran() % (b - a) + a;
}

}  // namespace lhy