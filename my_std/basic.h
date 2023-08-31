//
// Created by lhy31 on 2023/8/24.
//

#ifndef MY_STL_BASIC_H
#define MY_STL_BASIC_H

#include <cstdint>
namespace lhy {
using Index = int64_t;
using size_t = int64_t;
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
};
template <>
class TypeTraits<long> {
 public:
  using WhetherInterger = IsInteger;
};
template <>
class TypeTraits<long long> {
 public:
  using WhetherInterger = IsInteger;
};
template <>
class TypeTraits<unsigned int> {
 public:
  using WhetherInterger = IsInteger;
};
template <>
class TypeTraits<unsigned long> {
 public:
  using WhetherInterger = IsInteger;
};
template <>
class TypeTraits<unsigned long long> {
 public:
  using WhetherInterger = IsInteger;
};
template <typename T>
using CmpType = std::function<bool(const T&, const T&)>;
}  // namespace lhy
#endif  // MY_STL_BASIC_H