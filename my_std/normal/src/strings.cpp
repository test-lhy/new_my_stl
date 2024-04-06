//
// Created by lhy on 24-4-6.
//
#include"strings.h"
template <>
class TypeTraits<string> {
 public:
  static HashFuncType<string> hash_func;
};
HashFuncType<string> TypeTraits<string>::hash_func = [](const string& x, const size_t& mod) -> Index {
  size_t hash = 0;
  // todo:改成可自定义multiplier
  const int multiplier = 233;
  for (auto c : x) {
    hash = (hash * multiplier + c) % mod;
  }
  return hash;
};
