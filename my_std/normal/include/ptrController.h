//
// Created by lhy on 24-6-1.
//

#ifndef PTRCONTROLLER_H
#define PTRCONTROLLER_H
#include <functional>

#include "ptr_base.h"
namespace lhy {
class ptrController {
 public:
  explicit ptrController() : weak_count_(0), shared_count_(1) {}
  ptrController(const ptrController& other) = delete;
  ptrController(ptrController&& other) noexcept = delete;
  ptrController& operator=(const ptrController& other) = delete;
  ptrController& operator=(ptrController&& other) noexcept = delete;
  void IncreaseWeak();
  void IncreaseShared();
  void DecreaseWeak();
  void DecreaseShared();
  virtual void Delete() = 0;
  void CheckDelete();
  void CheckSelf() const;
  [[nodiscard]] int weak_count() const { return weak_count_; }
  [[nodiscard]] int shared_count() const { return shared_count_; }
  virtual ~ptrController() = default;

 private:
  int weak_count_;
  int shared_count_;
};
template <typename T>
class ptrControllerImpl : public ptrController {
 public:
  ptrControllerImpl(T value, const Deleter<T>& deleter) : ptrController(), deleter_(deleter), value_(value) {}
  void Delete() override;
  ~ptrControllerImpl() override = default;

 private:
  std::function<void(T)> deleter_;
  T value_;
};
template <typename T>
void ptrControllerImpl<T>::Delete() {
  deleter_(value_);
}

}  // namespace lhy

#endif  // PTRCONTROLLER_H
