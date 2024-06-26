//
// Created by lhy31 on 2023/8/24.
//

#ifndef MY_STL_MATH_H
#define MY_STL_MATH_H
#include <cstdint>
namespace lhy {
[[nodiscard]] int64_t Pow(int64_t, int64_t);
[[nodiscard]] int64_t Pow(int64_t, int64_t, int64_t);
[[nodiscard]] int64_t PowModPrime(int64_t, int64_t, int64_t);
}  // namespace lhy
#endif  // MY_STL_MATH_H