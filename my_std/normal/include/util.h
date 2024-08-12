//
// Created by lhy on 24-8-8.
//

#ifndef UTIL_H
#define UTIL_H
#include "meta.h"
#include "string_view.h"
#include "strings.h"
namespace lhy {
template <typename T>
string get_type() {
  string type_name = __PRETTY_FUNCTION__;
  const string_view type_name_view(type_name);
  return string{type_name.begin() + *type_name_view.find("T =") + 4, type_name.begin() + *type_name_view.find("]")};
}
template <typename T, T N>
string get_type() {
  string type_name = __PRETTY_FUNCTION__;
  const string_view type_name_view(type_name);
  return string{type_name.begin() + *type_name_view.find("N =") + 4, type_name.begin() + *type_name_view.find("]")};
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
