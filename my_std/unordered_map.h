//
// Created by lhy on 23-11-28.
//
#include "double_hash.h"

#ifndef MY_STL_UNORDERED_MAP_H
#define MY_STL_UNORDERED_MAP_H

namespace lhy {
template <typename T, typename U>
using unordered_map = DoubleHash<T, U>;
}  // namespace lhy

#endif  // MY_STL_UNORDERED_MAP_H