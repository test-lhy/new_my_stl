//
// Created by lhy31 on 2023/8/26.
//
#include "algorithm.h"

#include "strings.h"

namespace lhy {
[[nodiscard]] string ToString(int64_t origin) {
  string ans;
  bool whether_negative = false;
  if (origin < 0) {
    whether_negative = true;
    origin *= -1;
  }
  while (origin > 0) {
    ans += static_cast<char>(origin % 10 + '0');
    origin /= 10;
  }
  if (whether_negative) {
    ans += '-';
  }
  reverse(ans.begin(), ans.end());
  return ans;
}
}  // namespace lhy