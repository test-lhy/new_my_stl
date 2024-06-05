//
// Created by lhy on 24-6-5.
//

#ifndef UNIQUE_PTR_H
#define UNIQUE_PTR_H
#include "basic.h"
#include "ptr_base.h"
namespace lhy {
template <typename T>
class unique_ptr {
 public:
  using RealT = typename ptrType<T>::Type;
  unique_ptr() = default;
  unique_ptr(RealT ptr, Deleter<RealT> deleter = ptrType<T>::default_deleter_);
  unique_ptr(const unique_ptr& other) = delete;
  unique_ptr(unique_ptr&& other);
  template <convertiable_pointer_c<RealT> U>
  unique_ptr(unique_ptr<U>&& other);
  unique_ptr& operator=(const unique_ptr& other) = delete;
  unique_ptr& operator=(unique_ptr&& other);
  template <convertiable_pointer_c<RealT> U>
  unique_ptr& operator=(unique_ptr<U>&& other);
  RealT release();
  void reset(RealT ptr = nullptr);
  void swap(unique_ptr& other);
  RealT get() const;
  Deleter<RealT>& get_deleter();
  Deleter<RealT>& get_deleter() const;
  operator bool() const;
  std::remove_extent_t<T>& operator[](Index index);
  T& operator*() { return *get(); }
  const T& operator*() const { return *get(); }
  RealT operator->() { return get(); }
  RealT operator->() const { return get(); }
  ~unique_ptr();

 private:
  RealT value_{};
  Deleter<RealT> deleter_{};
};

template <typename T>
unique_ptr<T>::unique_ptr(RealT ptr, Deleter<RealT> deleter) : value_(ptr), deleter_(deleter) {}
template <typename T>
unique_ptr<T>& unique_ptr<T>::operator=(unique_ptr&& other) {
  if (&other != this) {
    if (*this) {
      reset();
    }
    swap(other);
  }
  return *this;
}
template <typename T>
template <convertiable_pointer_c<typename ptrType<T>::Type> U>
unique_ptr<T>& unique_ptr<T>::operator=(unique_ptr<U>&& other) {
  if (&other != this) {
    if (*this) {
      reset();
    }
    value_ = other.value_;
    deleter_ = other.deleter_;
    other.value_ = nullptr;
  }
  return *this;
}
template <typename T>
unique_ptr<T>::unique_ptr(unique_ptr&& other) {
  this->swap(other);
}
template <typename T>
template <convertiable_pointer_c<typename ptrType<T>::Type> U>
unique_ptr<T>::unique_ptr(unique_ptr<U>&& other) {
  this->swap(other);
}
template <typename T>
typename unique_ptr<T>::RealT unique_ptr<T>::release() {
  RealT ret = value_;
  value_ = nullptr;
  return ret;
}
template <typename T>
void unique_ptr<T>::reset(RealT ptr) {
  if (*this) {
    deleter_(value_);
    value_ = ptr;
  }
}
template <typename T>
void unique_ptr<T>::swap(unique_ptr& other) {
  std::swap(value_, other.value_);
  std::swap(deleter_, other.deleter_);
}
template <typename T>
typename unique_ptr<T>::RealT unique_ptr<T>::get() const {
  return value_;
}
template <typename T>
Deleter<typename unique_ptr<T>::RealT>& unique_ptr<T>::get_deleter() {
  return deleter_;
}
template <typename T>
Deleter<typename unique_ptr<T>::RealT>& unique_ptr<T>::get_deleter() const {
  return deleter_;
}
template <typename T>
unique_ptr<T>::operator bool() const {
  return value_ != nullptr;
}
template <typename T>
std::remove_extent_t<T>& unique_ptr<T>::operator[](Index index) {
  return *(get() + index);
}
template <typename T>
unique_ptr<T>::~unique_ptr() {
  if (*this) {
    deleter_(value_);
  }
}
template <not_array_c T, typename... Args>
unique_ptr<T> make_unique(Args&&... args) {
  return unique_ptr<T>(new T(std::forward<Args>(args)...));
}
template <unbounded_array_c T>
unique_ptr<T> make_unique(size_t N) {
  return unique_ptr<T>(new std::remove_extent_t<T>[N]);
}
template <not_array_c T>
unique_ptr<T> make_unique_for_overwrite() {
  return unique_ptr<T>(new T());
}
template <unbounded_array_c T>
unique_ptr<T> make_unique_for_overwrite(size_t N) {
  return unique_ptr<T>(new std::remove_extent_t<T>[N]());
}
}  // namespace lhy

#endif  // UNIQUE_PTR_H
