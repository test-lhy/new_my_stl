//
// Created by lhy on 24-6-1.
//

#ifndef SHARED_PTR_H
#define SHARED_PTR_H
#include <format>
#include <memory>
#include <type_traits>

#include "basic.h"
#include "ptrController.h"

namespace lhy {
template <typename T>
using Deleter = std::function<void(T)>;
template <typename T>
class ptrType {
 public:
  using Type = std::add_pointer_t<T>;
  inline static Deleter<Type> default_deleter_ = [](Type ptr) -> void { delete ptr; };
};
template <typename T>
concept array_c = std::is_array_v<T>;
template <array_c T>
class ptrType<T> {
 public:
  using Type = std::add_pointer_t<std::remove_extent_t<T>>;
  inline static Deleter<Type> default_deleter_ = [](Type ptr) -> void { delete[] ptr; };
};
template <typename T, typename U>
concept convertiable_c = std::is_convertible_v<T, U>;
template <typename T, typename U>
concept convertiable_pointer_c = std::is_convertible_v<typename ptrType<T>::Type, U>;
// note:T和ControlledT都是非指针的
template <typename T, typename ControlledT = T>
class shared_ptr {
 public:
  using RealT = typename ptrType<T>::Type;
  using RealControlledT = typename ptrType<ControlledT>::Type;
  shared_ptr();
  explicit shared_ptr(RealT ptr, Deleter<RealT> deleter = ptrType<T>::default_deleter_);
  void reset(RealT ptr, Deleter<RealT> deleter = ptrType<T>::default_deleter_);
  shared_ptr(const shared_ptr& other);
  shared_ptr(shared_ptr&& other);
  bool InitCopySwap(RealT other);
  shared_ptr& operator=(const shared_ptr& other);
  shared_ptr& operator=(shared_ptr&& other);
  template <convertiable_pointer_c<RealT> U>
  void Copy(const shared_ptr<U>& other);
  template <convertiable_pointer_c<RealT> U>
  explicit shared_ptr(const shared_ptr<U>& other);
  template <convertiable_pointer_c<RealT> U>
  explicit shared_ptr(shared_ptr<U>&& other);
  template <convertiable_pointer_c<RealT> U>
  shared_ptr& operator=(const shared_ptr<U>& other);
  template <convertiable_pointer_c<RealT> U>
  void swap(shared_ptr<U>&& other);
  template <convertiable_pointer_c<RealT> U>
  shared_ptr& operator=(shared_ptr<U>&& other);
  template <convertiable_c<RealT> U>
  shared_ptr(const shared_ptr<ControlledT>& other, U other_get_value);
  template <convertiable_c<RealT> U>
  shared_ptr(shared_ptr<ControlledT>&& other, U other_get_value);
  explicit operator bool() const { return get() != nullptr; }
  std::remove_extent_t<T> operator[](Index index) { return *(get() + index); }
  [[nodiscard]] int use_count() const { return controller_->shared_count(); }
  ~shared_ptr();
  RealT get() const;
  T& operator*() { return *get(); }
  const T& operator*() const { return *get(); }
  RealT operator->() { return get(); }
  RealT operator->() const { return get(); }

 private:
  void reset();

  RealT get_value_;
  ptrController<RealControlledT>* controller_;
};

template <typename T, typename ControlledT>
shared_ptr<T, ControlledT>::shared_ptr() : get_value_(nullptr), controller_(nullptr) {}
// template< class Deleter, class Alloc >
// shared_ptr( std::nullptr_t ptr, Deleter d, Alloc alloc );
// 这大概只是为了避免如我下面的对nullptr进行if判断，而通过编译器判断来提高效率
template <typename T, typename ControlledT>
shared_ptr<T, ControlledT>::shared_ptr(RealT ptr, Deleter<RealT> deleter) : shared_ptr() {
  reset(ptr, deleter);
}
template <typename T, typename ControlledT>
void shared_ptr<T, ControlledT>::reset(RealT ptr, Deleter<RealT> deleter) {
  if (InitCopySwap(ptr)) {
    return;
  }
  get_value_ = ptr;
  controller_ = new ptrController<RealT>(ptr, deleter);
}
template <typename T, typename ControlledT>
shared_ptr<T, ControlledT>::shared_ptr(const shared_ptr& other) {
  if (InitCopySwap(other.get())) {
    return;
  }
  Copy(std::forward<const shared_ptr>(other));
}
template <typename T, typename ControlledT>
shared_ptr<T, ControlledT>::shared_ptr(shared_ptr&& other) {
  if (InitCopySwap(other.get())) {
    return;
  }
  swap(std::forward<shared_ptr>(other));
}
template <typename T, typename ControlledT>
bool shared_ptr<T, ControlledT>::InitCopySwap(RealT other) {
  reset();
  if (other == nullptr) {
    return true;
  }
  return false;
}
template <typename T, typename ControlledT>
shared_ptr<T, ControlledT>& shared_ptr<T, ControlledT>::operator=(const shared_ptr& other) {
  if (&other != this) {
    if (InitCopySwap(other.get())) {
      return *this;
    }
    Copy(std::forward<const shared_ptr>(other));
  }
  return *this;
}
template <typename T, typename ControlledT>
shared_ptr<T, ControlledT>& shared_ptr<T, ControlledT>::operator=(shared_ptr&& other) {
  if (&other != this) {
    if (InitCopySwap(other.get())) {
      return *this;
    }
    swap(std::forward<shared_ptr>(other));
  }
  return *this;
}
template <typename T, typename ControlledT>
template <convertiable_pointer_c<typename ptrType<T>::Type> U>
void shared_ptr<T, ControlledT>::Copy(const shared_ptr<U>& other) {
  get_value_ = other.get_value_;
  controller_ = other.controller_;
  controller_->IncreaseShared();
}
template <typename T, typename ControlledT>
template <convertiable_pointer_c<typename ptrType<T>::Type> U>
shared_ptr<T, ControlledT>::shared_ptr(const shared_ptr<U>& other) {
  if (InitCopySwap(other.get())) {
    return;
  }
  Copy(std::forward<const shared_ptr<U>>(other));
}
template <typename T, typename ControlledT>
template <convertiable_pointer_c<typename ptrType<T>::Type> U>
shared_ptr<T, ControlledT>::shared_ptr(shared_ptr<U>&& other) {
  if (InitCopySwap(other.get())) {
    return;
  }
  swap(std::forward<shared_ptr<U>>(other));
}
template <typename T, typename ControlledT>
template <convertiable_pointer_c<typename ptrType<T>::Type> U>
shared_ptr<T, ControlledT>& shared_ptr<T, ControlledT>::operator=(const shared_ptr<U>& other) {
  if (&other != this) {
    if (InitCopySwap(other.get())) {
      return *this;
    }
    Copy(std::forward<const shared_ptr<U>>(other));
  }
  return *this;
}
template <typename T, typename ControlledT>
template <convertiable_pointer_c<typename ptrType<T>::Type> U>
void shared_ptr<T, ControlledT>::swap(shared_ptr<U>&& other) {
  std::swap(other.controller_, controller_);
  std::swap(other.get_value_, get_value_);
}
template <typename T, typename ControlledT>
template <convertiable_pointer_c<typename ptrType<T>::Type> U>
shared_ptr<T, ControlledT>& shared_ptr<T, ControlledT>::operator=(shared_ptr<U>&& other) {
  if (&other != this) {
    if (InitCopySwap(other.get())) {
      return *this;
    }
    swap(std::forward<shared_ptr<U>>(other));
  }
  return *this;
}
template <typename T, typename ControlledT>
template <convertiable_c<typename ptrType<T>::Type> U>
shared_ptr<T, ControlledT>::shared_ptr(const shared_ptr<ControlledT>& other, U other_get_value) {
  if (InitCopySwap(other.get())) {
    return;
  }
  Copy(std::forward<const shared_ptr<ControlledT>>(other));
  get_value_ = other_get_value;
}
template <typename T, typename ControlledT>
template <convertiable_c<typename ptrType<T>::Type> U>
shared_ptr<T, ControlledT>::shared_ptr(shared_ptr<ControlledT>&& other, U other_get_value) {
  if (InitCopySwap(other.get())) {
    return;
  }
  swap(std::forward<shared_ptr<ControlledT>>(other));
  get_value_ = other_get_value;
}
template <typename T, typename ControlledT>
shared_ptr<T, ControlledT>::~shared_ptr() {
  reset();
}
template <typename T, typename ControlledT>
typename shared_ptr<T, ControlledT>::RealT shared_ptr<T, ControlledT>::get() const {
  return get_value_;
}
template <typename T, typename ControlledT>
void shared_ptr<T, ControlledT>::reset() {
  if (controller_ != nullptr) {
    controller_->DecreaseShared();
    get_value_ = nullptr;
    controller_ = nullptr;
  }
}
}  // namespace lhy

#endif  // SHARED_PTR_H
