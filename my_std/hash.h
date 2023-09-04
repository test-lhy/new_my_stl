//
// Created by lhy31 on 2023/9/1.
//

#ifndef MY_STL_HASH_H
#define MY_STL_HASH_H
#include "basic.h"
#include "list.h"
#include "type_traits.h"
#include "vector.h"
namespace lhy {
const vector<size_t> kmod_values{53,        97,        193,       389,       769,       1543,     3079,
                                 6151,      12289,     24593,     49157,     98317,     196613,   393241,
                                 786433,    1572869,   3145739,   6291469,   12582917,  25165843, 50331653,
                                 100663319, 201326611, 402653189, 805306457, 1610612741};
size_t GetModValue(const size_t& size) {
  auto* iter = kmod_values.begin();
  size_t up_limit = 32;
  while (size > up_limit && iter != kmod_values.end()) {
    up_limit *= 2;
    ++iter;
  }
  if (iter == kmod_values.end()) {
    throw std::invalid_argument("size too large");
  }
  return *iter;
}
template <typename T, typename U>
class hash {
 private:
  struct Element {
    T key;
    U value;
    bool operator==(const Element& other) const { return key == other.key; }
    bool operator!=(const Element& other) const { return key != other.key; }
  };

 public:
  explicit hash(size_t,size_t mod_value=196613, HashFuncType<T>& hash_func = TypeTraits<T>::hash_func);
  explicit hash(HashFuncType<T>& hash_func = TypeTraits<T>::hash_func);
  void insert(const T& key, const U& value);
  U& operator[](const T& key);
  ~hash();
  // note:指hash函数得出的结果相等
  bool equal(const T&, const T&);

 private:
  size_t mod_value_ = 196613;
  vector<list<Element>> hash_table_;
  HashFuncType<T> hash_func_;
};
template <typename T, typename U>
U& hash<T, U>::operator[](const T& key) {
  list<Element>& hash_list = hash_table_[hash_func_(key, mod_value_)];
  auto* answer = hash_list.find({key, U()});
  if (answer != hash_list.end()) {
    return answer->content_.value;
  } else {
    hash_list.push_back({key, U()});
    auto* new_answer = hash_list.find({key, U()});
    return new_answer->content_.value;
  }
}
template <typename T, typename U>
bool hash<T, U>::equal(const T& a, const T& b) {
  return hash_func_(a, mod_value_) == hash_func_(b, mod_value_);
}
template <typename T, typename U>
hash<T, U>::~hash() = default;
template <typename T, typename U>
hash<T, U>::hash(HashFuncType<T>& hash_func) {
  hash_table_.reserve(mod_value_ + 5);
  hash_func_ = hash_func;
}

template <typename T, typename U>
hash<T, U>::hash(size_t size,size_t mod_value, HashFuncType<T>& hash_func) : hash(hash_func) {
  if (mod_value!=mod_value_){
    mod_value_ = mod_value;
  }else{
    mod_value_ = GetModValue(size);
  }
  hash_table_.reserve(mod_value_ + 5);
  hash_func_ = hash_func;
}
template <typename T, typename U>
void hash<T, U>::insert(const T& key, const U& value) {
  hash_table_[hash_func_(key, mod_value_)].push_back({key, value});
}
};      // namespace lhy
#endif  // MY_STL_HASH_H