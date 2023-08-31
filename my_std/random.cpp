//
// Created by lhy31 on 2023/8/22.
//
#include "random.h"
namespace lhy {
int64_t rand(int64_t a, int64_t b) {
  std::random_device r;
  std::mt19937 ran(r());
  return ran() % (b - a) + a;
}

}  // namespace lhy