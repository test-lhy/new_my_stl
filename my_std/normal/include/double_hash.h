//
// Created by lhy31 on 2023/9/4.
//

#ifndef MY_STL_DOUBLE_HASH_H
#define MY_STL_DOUBLE_HASH_H
#include "hash.h"
namespace lhy {
template <hashable T, hashable U>
class DoubleHash {
 public:
  explicit DoubleHash(size_t mod_value_1 = 196613, size_t mod_value_2 = 393241,
                      HashFuncType<T>& hash_func = TypeTraits<T>::hash_func);
  ~DoubleHash();
  void insert(const T&, const U&);
  [[nodiscard]] bool equal(const T&, const T&) const;

 private:
  Hash<T, U> hash_1;
  Hash<T, U> hash_2;
};
template <hashable T, hashable U>
DoubleHash<T, U>::DoubleHash(size_t mod_value_1, size_t mod_value_2, HashFuncType<T>& hash_func) {
  hash_1.mod_value_set(mod_value_1);
  hash_2.mod_value_set(mod_value_2);
  hash_1.hash_func_set(hash_func);
  hash_2.hash_func_set(hash_func);
}
template <hashable T, hashable U>
DoubleHash<T, U>::~DoubleHash() = default;
template <hashable T, hashable U>
void DoubleHash<T, U>::insert(const T& key, const U& value) {
  hash_1.insert(key, value);
  hash_2.insert(key, value);
}
template <hashable T, hashable U>
bool DoubleHash<T, U>::equal(const T& a, const T& b) const {
  return hash_1.equal(a, b) && hash_2.equal(a, b);
}
}  // namespace lhy
#endif  // MY_STL_DOUBLE_HASH_H