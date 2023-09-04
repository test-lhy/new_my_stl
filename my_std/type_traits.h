//
// Created by lhy31 on 2023/9/4.
//

#ifndef MY_STL_TYPE_TRAITS_H
#define MY_STL_TYPE_TRAITS_H
#include <string>

#include "basic.h"
namespace lhy {
struct IsInteger {};
struct IsNotInteger {};
template <typename T>
class TypeTraits {
 public:
  using WhetherInterger = IsNotInteger;
};
template <>
class TypeTraits<int> {
 public:
  using WhetherInterger = IsInteger;
  static HashFuncType<int> hash_func;
};
template <>
class TypeTraits<long> {
 public:
  using WhetherInterger = IsInteger;
  static HashFuncType<long> hash_func;
};
template <>
class TypeTraits<long long> {
 public:
  using WhetherInterger = IsInteger;
  static HashFuncType<long long> hash_func;
};
template <>
class TypeTraits<unsigned int> {
 public:
  using WhetherInterger = IsInteger;
  static HashFuncType<unsigned int> hash_func;
};
template <>
class TypeTraits<unsigned long> {
 public:
  using WhetherInterger = IsInteger;
  static HashFuncType<unsigned long> hash_func;
};
template <>
class TypeTraits<unsigned long long> {
 public:
  using WhetherInterger = IsInteger;
  static HashFuncType<unsigned long long> hash_func;
};
template <>
class TypeTraits<std::string> {
 public:
  using WhetherInterger = IsNotInteger;
  static HashFuncType<std::string> hash_func;
};
}  // namespace lhy
#endif  // MY_STL_TYPE_TRAITS_H