//
// Created by lhy on 24-6-4.
//

#ifndef WEAK_PTR_H
#define WEAK_PTR_H
#include <cmath>

#include "ptr_base.h"
#include "shared_ptr.h"
namespace lhy {
template <typename T>
class weak_ptr {
 public:
  template <typename T_>
  friend class weak_ptr;
  template <typename T_, typename ControlledT_>
  friend class shared_ptr;
  using RealT = typename shared_ptr<T>::RealT;
  weak_ptr() = default;
  weak_ptr(const weak_ptr& other);
  template <convertiable_pointer_c<RealT> U>
  weak_ptr(const weak_ptr<U>& other);
  template <convertiable_pointer_c<RealT> U>
  weak_ptr(const shared_ptr<U>& other);
  weak_ptr(weak_ptr&& other);
  template <convertiable_pointer_c<RealT> U>
  weak_ptr(weak_ptr<U>&& other);
  weak_ptr& operator=(const weak_ptr& other);
  template <convertiable_pointer_c<RealT> U>
  weak_ptr& operator=(const weak_ptr<U>& other);
  weak_ptr& operator=(weak_ptr&& other);
  template <convertiable_pointer_c<RealT> U>
  weak_ptr& operator=(weak_ptr<U>&& other);
  template <convertiable_pointer_c<RealT> U>
  weak_ptr& operator=(const shared_ptr<U>& other);
  ~weak_ptr();
  void reset();
  [[nodiscard]] int use_count() const { return controller_ == nullptr ? 0 : controller_->shared_count(); }
  [[nodiscard]] bool expired() const { return use_count() == 0; }
  [[nodiscard]] shared_ptr<T> lock() const { return expired() ? shared_ptr<T>() : shared_ptr<T>(*this); }
  void swap(weak_ptr& other);

 private:
  RealT get_value_{};
  ptrController* controller_{};
};
template <typename T>
weak_ptr<T>::weak_ptr(const weak_ptr& other) {
  operator=(std::forward<const weak_ptr&>(other));
}
template <typename T>
template <convertiable_pointer_c<typename shared_ptr<T>::RealT> U>
weak_ptr<T>::weak_ptr(const weak_ptr<U>& other) {
  operator=(std::forward<const weak_ptr<U>&>(other));
}
template <typename T>
template <convertiable_pointer_c<typename shared_ptr<T>::RealT> U>
weak_ptr<T>::weak_ptr(const shared_ptr<U>& other) {
  operator=(std::forward<const shared_ptr<U>&>(other));
}
template <typename T>
weak_ptr<T>::weak_ptr(weak_ptr&& other) {
  operator=(std::forward<weak_ptr&&>(other));
}
template <typename T>
template <convertiable_pointer_c<typename shared_ptr<T>::RealT> U>
weak_ptr<T>::weak_ptr(weak_ptr<U>&& other) {
  operator=(std::forward<weak_ptr<U>&&>(other));
}
template <typename T>
weak_ptr<T>& weak_ptr<T>::operator=(const weak_ptr& other) {
  if (&other == this) {
    return *this;
  }
  reset();
  if (other.expired()) {
    return *this;
  }
  controller_ = other.controller_;
  get_value_ = other.get_value_;
  controller_->IncreaseWeak();
  return *this;
}
template <typename T>
template <convertiable_pointer_c<typename shared_ptr<T>::RealT> U>
weak_ptr<T>& weak_ptr<T>::operator=(const weak_ptr<U>& other) {
  if ((!std::is_same_v<U, T>) || (reinterpret_cast<weak_ptr*>(const_cast<weak_ptr<U>*>(&other)) != this)) {
    reset();
    if (other.expired()) {
      return *this;
    }
    controller_ = other.controller_;
    get_value_ = other.get_value_;
    controller_->IncreaseWeak();
  }
  return *this;
}
// note:不写判断自赋值了，不会真的有人会写出a=std::move(a)这种东西把
template <typename T>
weak_ptr<T>& weak_ptr<T>::operator=(weak_ptr&& other) {
  reset();
  if (other.expired()) {
    return *this;
  }
  controller_ = other.controller_;
  get_value_ = other.get_value_;
  other.controller_ = nullptr;
  other.get_value_ = nullptr;
  return *this;
}
template <typename T>
template <convertiable_pointer_c<typename shared_ptr<T>::RealT> U>
weak_ptr<T>& weak_ptr<T>::operator=(weak_ptr<U>&& other) {
  reset();
  if (other.expired()) {
    return *this;
  }
  swap(std::forward<weak_ptr&&>(other));
  return *this;
}
template <typename T>
template <convertiable_pointer_c<typename shared_ptr<T>::RealT> U>
weak_ptr<T>& weak_ptr<T>::operator=(const shared_ptr<U>& other) {
  reset();
  if (other.get() == nullptr) {
    return *this;
  }
  controller_ = other.controller_;
  get_value_ = other.get();
  controller_->IncreaseWeak();
  return *this;
}
template <typename T>
weak_ptr<T>::~weak_ptr() {
  reset();
}
template <typename T>
void weak_ptr<T>::reset() {
  if (controller_ != nullptr) {
    controller_->DecreaseWeak();
    controller_ = nullptr;
    get_value_ = nullptr;
  }
}
template <typename T>
void weak_ptr<T>::swap(weak_ptr& other) {
  std::swap(controller_, other.controller_);
  std::swap(get_value_, other.get_value_);
}

}  // namespace lhy

#endif  // WEAK_PTR_H
