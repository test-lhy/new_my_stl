//
// Created by lhy on 24-8-8.
//

#ifndef UTIL_H
#define UTIL_H
#include "strings.h"

namespace lhy {
class IndexAddOne {
 public:
  template <int i>
  static constexpr int value = std::integral_constant<int, i + 1>::value;
};
class IndexMinusOne {
 public:
  template <int i>
  static constexpr int value = std::integral_constant<int, i - 1>::value;
};
template <typename T>
concept ConstexprNumericFunc = requires() { T::template value<0>; };
template <int Beg, int End, ConstexprNumericFunc IndexChangeFunc = IndexAddOne, typename Func = void>
void static_for(Func fun) {
  fun.template operator()<Beg>();
  if constexpr (Beg != End) {
    static_for<IndexChangeFunc::template value<Beg>, End, IndexChangeFunc>(fun);
  }
}
template <typename T>
string get_type() {
  string type_name = __PRETTY_FUNCTION__;
  const auto pos1 = GetKMP({"T ="}, type_name)[0];
  const auto pos2 = GetKMP({"]"}, type_name)[0];
  return string{type_name.begin() + pos1 + 4, type_name.begin() + pos2};
}
template <typename T, T N>
string get_type() {
  string type_name = __PRETTY_FUNCTION__;
  const auto pos1 = GetKMP({"N ="}, type_name)[0];
  const auto pos2 = GetKMP({"]"}, type_name)[0];
  return string{type_name.begin() + pos1 + 4, type_name.begin() + pos2};
}
template <typename T>
string get_type(T enum_type)
  requires std::is_enum_v<T>
{
  string type_name = "not found";
  auto func = [&]<int i>() {
    if (static_cast<T>(i) == enum_type) {
      type_name = get_type<T, static_cast<T>(i)>();
    }
  };
  static_for<0, 256>(func);
  return type_name;
}

}  // namespace lhy

#endif  // UTIL_H
