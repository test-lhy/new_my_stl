//
// Created by lhy on 24-6-1.
//

#ifndef PTRCONTROLLER_H
#define PTRCONTROLLER_H
#include <functional>
#include <iostream>
namespace lhy {
template <typename T>
class ptrController {
 public:
  explicit ptrController(T value, const std::function<void(T)>& deleter)
      : weak_count_(0), shared_count_(1), value_(value), deleter_(deleter) {}
  ptrController(const ptrController& other) = delete;
  ptrController(ptrController&& other) noexcept = delete;
  ptrController& operator=(const ptrController& other) = delete;
  ptrController& operator=(ptrController&& other) noexcept = delete;
  void IncreaseWeak();
  void IncreaseShared();
  void DecreaseWeak();
  void DecreaseShared();
  void Delete();
  void CheckDelete();
  void CheckSelf() const;
  [[nodiscard]] int weak_count() const { return weak_count_; }
  [[nodiscard]] int shared_count() const { return shared_count_; }
  ~ptrController() = default;

 private:
  int weak_count_;
  int shared_count_;
  T value_;
  std::function<void(T)> deleter_;
};
template <typename T>
void ptrController<T>::IncreaseWeak() {
  weak_count_++;
}
template <typename T>
void ptrController<T>::IncreaseShared() {
  shared_count_++;
}
template <typename T>
void ptrController<T>::DecreaseWeak() {
  weak_count_--;
  CheckSelf();
}
template <typename T>
void ptrController<T>::DecreaseShared() {
  shared_count_--;
  CheckDelete();
  CheckSelf();
}
template <typename T>
void ptrController<T>::Delete() {
  deleter_(value_);
}
template <typename T>
void ptrController<T>::CheckDelete() {
  if (shared_count_ == 0) {
    Delete();
  }
}
template <typename T>
void ptrController<T>::CheckSelf() const {
  if (weak_count_ == 0 && shared_count_ == 0) {
    delete this;
  }
}

}  // namespace lhy

#endif  // PTRCONTROLLER_H
