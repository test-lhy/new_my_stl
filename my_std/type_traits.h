//
// Created by lhy31 on 2023/9/4.
//

#ifndef MY_STL_TYPE_TRAITS_H
#define MY_STL_TYPE_TRAITS_H
#include <string>

#include "basic.h"
namespace lhy {
template <typename T>
class TypeTraits {
};
class TypeTraitsForInt{
 public:
  static bool isInteger;
};
template <>
class TypeTraits<int>: public TypeTraitsForInt{
 public:
  static HashFuncType<int> hash_func;
};
template <>
class TypeTraits<long>: public TypeTraitsForInt{
 public:
  static HashFuncType<long> hash_func;
};
template <>
class TypeTraits<long long>: public TypeTraitsForInt{
 public:
  static HashFuncType<long long> hash_func;
};
template <>
class TypeTraits<unsigned int>: public TypeTraitsForInt{
 public:
  static HashFuncType<unsigned int> hash_func;
};
template <>
class TypeTraits<unsigned long>: public TypeTraitsForInt{
 public:
  static HashFuncType<unsigned long> hash_func;
};
template <>
class TypeTraits<unsigned long long>: public TypeTraitsForInt{
 public:
  static HashFuncType<unsigned long long> hash_func;
};
template <>
class TypeTraits<std::string> {
 public:
  static HashFuncType<std::string> hash_func;
};
}  // namespace lhy
#endif  // MY_STL_TYPE_TRAITS_H