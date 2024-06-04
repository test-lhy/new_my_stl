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
  CheckDelete();
  // note:写在Delete的后面是为了避免多次delete this
  // 存在这样一个情景，node1强引用node2，node2弱引用node1，node1强引用自己
  // 在结束进程的时候，加入node1先析构，那么会准备既delete node1，又delete node2，
  // 然而，在delete node2的过程中，由于对于node1的shared_ptr已经等于0了，而此时弱引用-1，
  // 又准备delete掉node1，然后就烂掉了
  shared_count_--;
  CheckSelf();
}
void ptrController::CheckDelete() {
  if (shared_count_ == 1) {
    Delete();
  }
}
void ptrController::CheckSelf() const {
  if (weak_count_ == 0 && shared_count_ == 0) {
    delete this;
  }
}
}  // namespace lhy