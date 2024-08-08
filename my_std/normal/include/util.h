//
// Created by lhy on 24-8-8.
//

#ifndef UTIL_H
#define UTIL_H
#include "strings.h"

namespace lhy {

template <typename T>
string get_type() {
  string type_name = __PRETTY_FUNCTION__;
  const auto pos1 = GetKMP({"T ="}, type_name)[0];
  const auto pos2 = GetKMP({";"}, type_name)[0];
  return string{type_name.begin() + pos1 + 4, type_name.begin() + pos2};
}

}  // namespace lhy

#endif  // UTIL_H
