//
// Created by lhy on 24-6-1.
//

#ifndef SHARED_PTR_H
#define SHARED_PTR_H
#include <format>
#include <memory>
#include <ranges>
#include <type_traits>

#include "basic.h"
#include "exception.h"
#include "ptrController.h"
#include "ptr_base.h"
#include "unique_ptr.h"

namespace lhy {
template <typename Deprived>
class enable_shared_from_this;
template <typename T>
void enable_shared_from_this_owner_helper(T* self, ptrController* controller);

template <typename T>
class weak_ptr;
// note:T和ControlledT都是非指针的
template <typename T, typename ControlledT = T>
class shared_ptr {
  template <not_array_c T_, class... Args, typename Alloc>
  friend shared_ptr<T_> make_shared(Args&&... args);

 public:
  template <typename T_, typename ControlledT_>
  friend class shared_ptr;
  template <typename T_>
  friend class weak_ptr;
  using RealT = typename ptrType<T>::Type;
  using RealControlledT = typename ptrType<ControlledT>::Type;
  shared_ptr();
  shared_ptr(RealT ptr, Deleter<RealT> deleter = ptrType<T>::default_deleter_);
  void reset(RealT ptr, Deleter<RealT> deleter = ptrType<T>::default_deleter_);
  shared_ptr(const shared_ptr& other);
  shared_ptr(shared_ptr&& other);
  bool InitCopySwap(RealT other);
  shared_ptr& operator=(const shared_ptr& other);
  shared_ptr& operator=(shared_ptr&& other);
  template <convertiable_pointer_c<RealT> U>
  shared_ptr(const shared_ptr<U>& other);
  template <convertiable_pointer_c<RealT> U>
  shared_ptr(const weak_ptr<U>& other);
  template <convertiable_pointer_c<RealT> U>
  shared_ptr(unique_ptr<U>&& other);
  template <convertiable_pointer_c<RealT> U>
  shared_ptr(shared_ptr<U>&& other);
  template <convertiable_pointer_c<RealT> U>
  shared_ptr& operator=(const shared_ptr<U>& other);
  template <convertiable_pointer_c<RealT> U>
  void swap(shared_ptr<U>& other);
  template <convertiable_pointer_c<RealT> U>
  void swap(shared_ptr<U>&& other);
  template <convertiable_pointer_c<RealT> U>
  shared_ptr& operator=(shared_ptr<U>&& other);
  template <convertiable_c<RealT> U>
  shared_ptr(const shared_ptr<ControlledT>& other, U other_get_value);
  template <convertiable_c<RealT> U>
  shared_ptr(shared_ptr<ControlledT>&& other, U other_get_value);
  operator bool() const { return get() != nullptr; }
  std::remove_extent_t<T>& operator[](Index index) { return *(get() + index); }
  std::remove_extent_t<T>& operator[](Index index) const { return *(get() + index); }
  [[nodiscard]] int use_count() const { return controller_->shared_count(); }
  ~shared_ptr();
  RealT get() const;
  RealT& getRef();
  T& operator*() { return *get(); }
  const T& operator*() const { return *get(); }
  RealT operator->() { return get(); }
  RealT operator->() const { return get(); }
  void reset();

 private:
  template <convertiable_pointer_c<RealT> U>
  void Copy(const shared_ptr<U>& other);
  RealT get_value_{};
  ptrController* controller_{};
  template <typename T1>
  friend shared_ptr<T1> enable_shared_from_this_helper(typename shared_ptr<T1>::RealT get_value,
                                                       ptrController* controller);
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
  controller_ = new ptrControllerImpl<RealT>(ptr, deleter);
  enable_shared_from_this_owner_helper(get_value_, controller_);
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
shared_ptr<T, ControlledT>::shared_ptr(const weak_ptr<U>& other) {
  if (other.expired()) {
    throw logic_error("weak_ptr has been expired");
  }
  controller_ = other.controller_;
  get_value_ = other.get_value_;
  controller_->IncreaseShared();
}
template <typename T, typename ControlledT>
template <convertiable_pointer_c<typename ptrType<T>::Type> U>
shared_ptr<T, ControlledT>::shared_ptr(unique_ptr<U>&& other) {
  if (other) {
    controller_ = new ptrControllerImpl<typename unique_ptr<U>::RealT>(other.get(), other.get_deleter());
    get_value_ = other.release();
    enable_shared_from_this_owner_helper(get_value_, controller_);
  }
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
  if ((!std::is_same_v<U, T>) || (reinterpret_cast<shared_ptr<T>*>(const_cast<shared_ptr<U>*>(&other)) != this)) {
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
  auto temp = reinterpret_cast<RealT>(other.get_value_);
  other.get_value_ = reinterpret_cast<typename shared_ptr<U>::RealT>(get_value_);
  get_value_ = temp;
}
template <typename T, typename ControlledT>
template <convertiable_pointer_c<typename ptrType<T>::Type> U>
void shared_ptr<T, ControlledT>::swap(shared_ptr<U>& other) {
  std::swap(other.controller_, controller_);
  auto temp = dynamic_cast<RealT>(other.get_value_);
  other.get_value_ = dynamic_cast<typename shared_ptr<U>::RealT>(get_value_);
  get_value_ = temp;
}
template <typename T, typename ControlledT>
template <convertiable_pointer_c<typename ptrType<T>::Type> U>
shared_ptr<T, ControlledT>& shared_ptr<T, ControlledT>::operator=(shared_ptr<U>&& other) {
  if ((!std::is_same_v<U, T>) || (reinterpret_cast<shared_ptr<T>*>(const_cast<shared_ptr<U>*>(&other)) != this)) {
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
typename shared_ptr<T, ControlledT>::RealT& shared_ptr<T, ControlledT>::getRef() {
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
// note:make_shared在一般的实现中是只进行一次内存分配，因此被管理的对象的new并没有进行分配内存，因此也不能使用delete来进行销毁，而要通过析构函数进行销毁，似乎有点难以实现，暂时先不实现
// 当控制块要被销毁的时候，整个内存才会被整体释放
template <not_array_c T, class... Args>
shared_ptr<T> make_shared(Args&&... args) {
  return shared_ptr<T>(new T(std::forward<Args>(args)...));
}
template <array_c T>
shared_ptr<T> make_shared(size_t N) {
  return shared_ptr<T>(new std::remove_extent_t<T>[N]());
}
template <bounded_array_c T>
shared_ptr<T> make_shared() {
  return shared_ptr<T>(new std::remove_extent_t<T>[std::extent_v<T>]());
}
template <array_c T>
shared_ptr<T> make_shared_for_overwrite(size_t N) {
  return shared_ptr<T>(new std::remove_extent_t<T>[N]);
}
template <bounded_array_c T>
shared_ptr<T> make_shared_for_overwrite() {
  return shared_ptr<T>(new std::remove_extent_t<T>[std::extent_v<T>]);
}
template <typename T>
shared_ptr<T> enable_shared_from_this_helper(typename shared_ptr<T>::RealT get_value, ptrController* controller) {
  shared_ptr<T> ret;
  ret.controller_ = controller;
  ret.get_value_ = get_value;
  ret.controller_->IncreaseShared();
  return ret;
}
template <typename Deprived>
class enable_shared_from_this {
  class Private {
   public:
    Private() = default;
  };

 public:
  shared_ptr<Deprived> shared_from_this() {
    if (!controller_) {
      throw bad_weak_ptr();
    }
    return enable_shared_from_this_helper<Deprived>(reinterpret_cast<Deprived*>(this), controller_);
  }
  enable_shared_from_this(Private = Private()) {}

 private:
  ptrController* controller_{};
  template <typename T>
  friend void enable_shared_from_this_owner_helper(T* self, ptrController* controller);

};
template<typename T>
void enable_shared_from_this_owner_helper(T* self, ptrController* controller) {
  if constexpr (std::is_base_of_v<enable_shared_from_this<T>, T>) {
    self->controller_ = controller;
  }
}
}  // namespace lhy

#endif  // SHARED_PTR_H
