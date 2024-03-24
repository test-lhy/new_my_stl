//
// Created by lhy31 on 2023/9/4.
//

#ifndef MY_STL_TYPE_TRAITS_H
#define MY_STL_TYPE_TRAITS_H
#include <string>

#include "basic.h"
namespace lhy {
template <typename T>
class TypeTraits {};
class TypeTraitsForInt {
 public:
  static bool isInteger;
};
class TypeTraitsForChar {
 public:
  static bool isChar;
};
template <>
class TypeTraits<int> : public TypeTraitsForInt {
 public:
  static HashFuncType<int> hash_func;
};
template <>
class TypeTraits<long> : public TypeTraitsForInt {
 public:
  static HashFuncType<long> hash_func;
};
template <>
class TypeTraits<long long> : public TypeTraitsForInt {
 public:
  static HashFuncType<long long> hash_func;
};
template <>
class TypeTraits<unsigned int> : public TypeTraitsForInt {
 public:
  static HashFuncType<unsigned int> hash_func;
};
template <>
class TypeTraits<unsigned long> : public TypeTraitsForInt {
 public:
  static HashFuncType<unsigned long> hash_func;
};
template <>
class TypeTraits<unsigned long long> : public TypeTraitsForInt {
 public:
  static HashFuncType<unsigned long long> hash_func;
};
template <>
class TypeTraits<std::string> {
 public:
  static HashFuncType<std::string> hash_func;
};
template <>
class TypeTraits<char> : public TypeTraitsForChar {};
template <typename T>
concept int_type = requires() { TypeTraits<T>::isInteger; };
template <typename T>
concept not_int_type = not int_type<T>;
template <typename T>
concept char_type = requires() { TypeTraits<T>::isChar; };
template <typename T>
concept not_char_type = not char_type<T>;
}  // namespace lhy
#endif  // MY_STL_TYPE_TRAITS_H