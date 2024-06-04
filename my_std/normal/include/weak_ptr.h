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
  using RealT = typename shared_ptr<T>::RealT;
  weak_ptr();
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
  weak_ptr& operator=(const weak_ptr<RealT>& other);
  weak_ptr& operator=(weak_ptr&& other);
  template <convertiable_pointer_c<RealT> U>
  weak_ptr& operator=(weak_ptr<RealT>&& other);
  template <convertiable_pointer_c<RealT> U>
  weak_ptr& operator=(const shared_ptr<RealT>& other);
  ~weak_ptr();
  void reset();
  [[nodiscard]] int use_count() const { return controller_->shared_count(); }
  [[nodiscard]] bool expired() const { return use_count() == 0; }
  [[nodiscard]] shared_ptr<T> lock() const { return expired() ? shared_ptr<T>() : shared_ptr<T>(*this); }
  void swap(const weak_ptr& other);

 private:
  ptrController<RealT>* controller_;
};
template <typename T>
weak_ptr<T>::weak_ptr() : controller_(nullptr) {}
template <typename T>
weak_ptr<T>::weak_ptr(const weak_ptr& other) {
  controller_ = other.controller_;
}
template <typename T>
template <convertiable_pointer_c<typename shared_ptr<T>::RealT> U>
weak_ptr<T>::weak_ptr(const weak_ptr<U>& other) {
  controller_ = other.controller_;
}
template <typename T>
template <convertiable_pointer_c<typename shared_ptr<T>::RealT> U>
weak_ptr<T>::weak_ptr(const shared_ptr<U>& other) {}
template <typename T>
weak_ptr<T>::weak_ptr(weak_ptr&& other) {}
template <typename T>
template <convertiable_pointer_c<typename shared_ptr<T>::RealT> U>
weak_ptr<T>::weak_ptr(weak_ptr<U>&& other) {}
template <typename T>
weak_ptr<T>& weak_ptr<T>::operator=(const weak_ptr& other) {}
template <typename T>
template <convertiable_pointer_c<typename shared_ptr<T>::RealT> U>
weak_ptr<T>& weak_ptr<T>::operator=(const weak_ptr<RealT>& other) {}
template <typename T>
weak_ptr<T>& weak_ptr<T>::operator=(weak_ptr&& other) {}
template <typename T>
template <convertiable_pointer_c<typename shared_ptr<T>::RealT> U>
weak_ptr<T>& weak_ptr<T>::operator=(weak_ptr<RealT>&& other) {}
template <typename T>
template <convertiable_pointer_c<typename shared_ptr<T>::RealT> U>
weak_ptr<T>& weak_ptr<T>::operator=(const shared_ptr<RealT>& other) {}
template <typename T>
weak_ptr<T>::~weak_ptr() {}
template <typename T>
void weak_ptr<T>::reset() {}
template <typename T>
void weak_ptr<T>::swap(const weak_ptr& other) {}

}  // namespace lhy

#endif  // WEAK_PTR_H
