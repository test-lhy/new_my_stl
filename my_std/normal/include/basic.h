//
// Created by lhy31 on 2023/8/24.
//

#ifndef MY_STL_BASIC_H
#define MY_STL_BASIC_H

#include <cstdint>
#include <functional>
namespace lhy {
using Index = int64_t;
using size_t = int64_t;
template <typename T>
using HashFuncType = std::function<Index(const T&, const size_t&)>;
template <typename T>
using CmpType = std::function<bool(const T&, const T&)>;
template <typename T, typename U>
concept Deprived = std::is_base_of_v<U, T>;
}  // namespace lhy
#endif  // MY_STL_BASIC_H