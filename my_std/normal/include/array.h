//
// Created by lhy on 24-8-14.
//

#ifndef ARRAY_H
#define ARRAY_H
#include <cassert>

#include "data_structure.h"
#include "tuple.h"
namespace lhy {
template <typename T, size_t N>
class array : public DataStructure<T> {
 public:
  using typename DataStructure<T>::Pointer;
  using iterator = NormIterator<T>;
  using reversed_iterator = ReversedNormIterator<T>;
  array() = default;
  array(const array& other) = default;
  array(array&& other) = default;
  array& operator=(const array& other) = default;
  array& operator=(array&& other) = default;
  array(std::initializer_list<T>);
  // 元素访问
  [[nodiscard]] T& at(Index pos);
  [[nodiscard]] const T& at(Index pos) const;

  T& operator[](Index pos);
  const T& operator[](Index pos) const;

  [[nodiscard]] T& front();
  [[nodiscard]] const T& front() const;

  [[nodiscard]] T& back();
  [[nodiscard]] const T& back() const;

  [[nodiscard]] T* data();
  [[nodiscard]] const T* data() const;

  // 迭代器
  [[nodiscard]] iterator begin();
  [[nodiscard]] iterator begin() const;
  [[nodiscard]] iterator cbegin() const;

  [[nodiscard]] iterator end();
  [[nodiscard]] iterator end() const;
  /// todo以后c系列肯定要大改
  [[nodiscard]] iterator cend() const;

  [[nodiscard]] reversed_iterator rbegin();
  [[nodiscard]] reversed_iterator rbegin() const;
  [[nodiscard]] reversed_iterator crbegin() const;

  [[nodiscard]] reversed_iterator rend();
  [[nodiscard]] reversed_iterator rend() const;
  [[nodiscard]] reversed_iterator crend() const;

  // 容量
  [[nodiscard]] constexpr bool empty() const;
  [[nodiscard]] constexpr size_t size() const;
  [[nodiscard]] constexpr size_t max_size() const;

  // 操作
  void fill(const T& value);
  void swap(array& other);
  template <size_t pos>
    requires(pos < N && pos >= 0)
  [[nodiscard]] constexpr T& get() & {
    return elems[pos];
  }
  template <size_t pos>
    requires(pos < N && pos >= 0)
  [[nodiscard]] constexpr T&& get() && {
    return elems[pos];
  }
  template <size_t pos>
    requires(pos < N && pos >= 0)
  [[nodiscard]] constexpr const T& get() const& {
    return elems[pos];
  }
  template <size_t pos>
    requires(pos < N && pos >= 0)
  [[nodiscard]] constexpr const T&& get() const&& {
    return elems[pos];
  }

 private:
  [[nodiscard]] typename DataStructure<T>::Pointer getBegin() override;
  [[nodiscard]] typename DataStructure<T>::Pointer getEnd() override;
  T elems[N];  // 底层存储
};
template <size_t I, typename T, size_t N>
  requires(I < N)
T& get(array<T, N>& arr) {
  return arr.template get<I>();
}

template <size_t I, typename T, size_t N>
  requires(I < N)
T&& get(array<T, N>&& arr) {
  return arr.template get<I>();
}
template <size_t I, typename T, size_t N>
  requires(I < N)
const T& get(const array<T, N>& arr) {
  return arr.template get<I>();
}
template <size_t I, typename T, size_t N>
  requires(I < N)
const T&& get(const array<T, N>&& arr) {
  return arr.template get<I>();
}
template <typename T, size_t N>
void swap(array<T, N>& lhs, array<T, N>& rhs) {
  lhs.swap(rhs);
}

template <typename T, size_t N>
array<T, N> to_array(T (&arr)[N]) {
  array<T, N> result;
  for (Index i = 0; i < N; ++i) {
    result[i] = arr[i];
  }
  return result;
}
template <typename T, size_t N>
array<T, N> to_array(T (&&arr)[N]) {
  array<T, N> result;
  for (Index i = 0; i < N; ++i) {
    result[i] = std::move(arr[i]);
  }
  return result;
}

// 辅助类：获得 array 的大小
template <typename T, size_t N>
class tuple_size<array<T, N>> {
 public:
  constexpr operator size_t() const { return value; }
  static constexpr size_t value = N;
};

// 辅助类：获得 array 元素的类型
template <size_t I, typename T, size_t N>
class tuple_element<I, array<T, N>> {
 public:
  using type = T;
};
template <typename T, size_t N>
array<T, N>::array(std::initializer_list<T> other) {
  assert(other.size() <= N);
  for (Index i = 0; i < other.size(); ++i) {
    /// 不太清楚initilize_list到底要不要移动
    elems[i] = *(other.begin() + i);
  }
}
template <typename T, size_t N>
T& array<T, N>::at(Index pos) {
  if (pos >= N) {
    throw std::range_error("out of range");
  }
  return elems[pos];
}
template <typename T, size_t N>
const T& array<T, N>::at(Index pos) const {
  if (pos >= N) {
    throw std::range_error("out of range");
  }
  return elems[pos];
}
template <typename T, size_t N>
T& array<T, N>::operator[](Index pos) {
  return elems[pos];
}
template <typename T, size_t N>
const T& array<T, N>::operator[](Index pos) const {
  return elems[pos];
}
template <typename T, size_t N>
T& array<T, N>::front() {
  return elems[0];
}
template <typename T, size_t N>
const T& array<T, N>::front() const {
  return elems[0];
}
template <typename T, size_t N>
T& array<T, N>::back() {
  return elems[N - 1];
}
template <typename T, size_t N>
const T& array<T, N>::back() const {
  return elems[N - 1];
}
template <typename T, size_t N>
T* array<T, N>::data() {
  return elems;
}
template <typename T, size_t N>
const T* array<T, N>::data() const {
  return elems;
}
template <typename T, size_t N>
typename array<T, N>::iterator array<T, N>::begin() {
  return elems;
}
template <typename T, size_t N>
typename array<T, N>::iterator array<T, N>::begin() const {
  return const_cast<T*>(elems);
}
template <typename T, size_t N>
typename array<T, N>::iterator array<T, N>::cbegin() const {
  return begin();
}
template <typename T, size_t N>
typename array<T, N>::iterator array<T, N>::end() {
  return elems + N;
}
template <typename T, size_t N>
typename array<T, N>::iterator array<T, N>::end() const {
  return const_cast<T*>(elems + N);
}
template <typename T, size_t N>
typename array<T, N>::iterator array<T, N>::cend() const {
  return end();
}
template <typename T, size_t N>
typename array<T, N>::reversed_iterator array<T, N>::rbegin() {
  return elems + N - 1;
}
template <typename T, size_t N>
typename array<T, N>::reversed_iterator array<T, N>::rbegin() const {
  return const_cast<T*>(elems + N - 1);
}
template <typename T, size_t N>
typename array<T, N>::reversed_iterator array<T, N>::crbegin() const {
  return rbegin();
}
template <typename T, size_t N>
typename array<T, N>::reversed_iterator array<T, N>::rend() {
  return elems - 1;
}
template <typename T, size_t N>
typename array<T, N>::reversed_iterator array<T, N>::rend() const {
  return const_cast<T*>(elems - 1);
}
template <typename T, size_t N>
typename array<T, N>::reversed_iterator array<T, N>::crend() const {
  return rend();
}
template <typename T, size_t N>
constexpr bool array<T, N>::empty() const {
  return false;
}
template <typename T, size_t N>
constexpr size_t array<T, N>::size() const {
  return N;
}
template <typename T, size_t N>
constexpr size_t array<T, N>::max_size() const {
  return N;
}
template <typename T, size_t N>
void array<T, N>::fill(const T& value) {
  for (auto& elem : elems) {
    elem = value;
  }
}
template <typename T, size_t N>
void array<T, N>::swap(array& other) {
  for (auto i = 0; i < N; ++i) {
    std::swap(elems[i], other.elems[i]);
  }
}
template <typename T, size_t N>
typename DataStructure<T>::Pointer array<T, N>::getBegin() {
  return make_shared<iterator>(begin());
}
template <typename T, size_t N>
typename DataStructure<T>::Pointer array<T, N>::getEnd() {
  return make_shared<iterator>(end());
}
template <typename T>
class array<T, 0> {
 public:
  using iterator = Iterator<T>;
  using reversed_iterator = Iterator<T>;
  [[nodiscard]] T* data();
  [[nodiscard]] const T* data() const;

  // 迭代器
  [[nodiscard]] iterator begin();
  [[nodiscard]] iterator begin() const;
  [[nodiscard]] iterator cbegin() const;

  [[nodiscard]] iterator end();
  [[nodiscard]] iterator end() const;
  [[nodiscard]] iterator cend() const;

  [[nodiscard]] reversed_iterator rbegin();
  [[nodiscard]] reversed_iterator rbegin() const;
  [[nodiscard]] reversed_iterator crbegin() const;

  [[nodiscard]] reversed_iterator rend();
  [[nodiscard]] reversed_iterator rend() const;
  [[nodiscard]] reversed_iterator crend() const;

  // 容量
  [[nodiscard]] constexpr bool empty() const;
  [[nodiscard]] constexpr size_t size() const;
  [[nodiscard]] constexpr size_t max_size() const;

  // 操作
  void fill(const T& value);
  void swap(array& other);
};
template <typename T>
T* array<T, 0>::data() {
  return nullptr;
}
template <typename T>
const T* array<T, 0>::data() const {
  return nullptr;
}
template <typename T>
typename array<T, 0>::iterator array<T, 0>::begin() {
  return nullptr;
}
template <typename T>
typename array<T, 0>::iterator array<T, 0>::begin() const {
  return nullptr;
}
template <typename T>
typename array<T, 0>::iterator array<T, 0>::cbegin() const {
  return nullptr;
}
template <typename T>
typename array<T, 0>::iterator array<T, 0>::end() {
  return nullptr;
}
template <typename T>
typename array<T, 0>::iterator array<T, 0>::end() const {
  return nullptr;
}
template <typename T>
typename array<T, 0>::iterator array<T, 0>::cend() const {
  return nullptr;
}
template <typename T>
typename array<T, 0>::reversed_iterator array<T, 0>::rbegin() {
  return nullptr;
}
template <typename T>
typename array<T, 0>::reversed_iterator array<T, 0>::rbegin() const {
  return nullptr;
}
template <typename T>
typename array<T, 0>::reversed_iterator array<T, 0>::crbegin() const {
  return nullptr;
}
template <typename T>
typename array<T, 0>::reversed_iterator array<T, 0>::rend() {
  return nullptr;
}
template <typename T>
typename array<T, 0>::reversed_iterator array<T, 0>::rend() const {
  return nullptr;
}
template <typename T>
typename array<T, 0>::reversed_iterator array<T, 0>::crend() const {
  return nullptr;
}
template <typename T>
constexpr bool array<T, 0>::empty() const {
  return true;
}
template <typename T>
constexpr size_t array<T, 0>::size() const {
  return 0;
}
template <typename T>
constexpr size_t array<T, 0>::max_size() const {
  return 0;
}
template <typename T>
void array<T, 0>::fill(const T& value) {}
template <typename T>
void array<T, 0>::swap(array& other) {}
template <typename Arg0, typename... Args>
array(Arg0,Args...)->array<std::common_type_t<Arg0,Args...>,sizeof...(Args)+1>;
}

#endif  // ARRAY_H
