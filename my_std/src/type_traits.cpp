//
// Created by lhy31 on 2023/9/4.
//
#include "type_traits.h"
namespace lhy {
HashFuncType<int> TypeTraits<int>::hash_func = [](const int& x, const size_t& mod) -> Index {
  return ((x % mod) + mod) % mod;
};
HashFuncType<long> TypeTraits<long>::hash_func = [](const long& x, const size_t& mod) -> Index {
  return ((x % mod) + mod) % mod;
};
HashFuncType<long long> TypeTraits<long long>::hash_func = [](const long long& x, const size_t& mod) -> Index {
  return ((x % mod) + mod) % mod;
};
HashFuncType<unsigned int> TypeTraits<unsigned int>::hash_func = [](const unsigned int& x, const size_t& mod) -> Index {
  return ((x % mod) + mod) % mod;
};
HashFuncType<unsigned long> TypeTraits<unsigned long>::hash_func =
    [](const unsigned long& x, const size_t& mod) -> Index { return ((x % mod) + mod) % mod; };
HashFuncType<unsigned long long> TypeTraits<unsigned long long>::hash_func =
    [](const unsigned long long& x, const size_t& mod) -> Index { return ((x % mod) + mod) % mod; };
HashFuncType<std::string> TypeTraits<std::string>::hash_func = [](const std::string& x, const size_t& mod) -> Index {
  size_t hash = 0;
  // todo:改成可自定义multiplier
  const int multiplier = 233;
  for (auto c : x) {
    hash = (hash * multiplier + c) % mod;
  }
  return hash;
};
}  // namespace lhy