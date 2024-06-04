//
// Created by lhy on 24-6-4.
//
#include "ptrController.h"
namespace lhy {
void ptrController::IncreaseWeak() { weak_count_++; }
void ptrController::IncreaseShared() { shared_count_++; }
void ptrController::DecreaseWeak() {
  weak_count_--;
  CheckSelf();
}
void ptrController::DecreaseShared() {
  shared_count_--;
  CheckDelete();
  CheckSelf();
}
void ptrController::CheckDelete() {
  if (shared_count_ == 0) {
    Delete();
  }
}
void ptrController::CheckSelf() const {
  if (weak_count_ == 0 && shared_count_ == 0) {
    delete this;
  }
}
}  // namespace lhy