//
// Created by lhy on 24-8-26.
//
#include <cassert>
#include <iostream>

#include "shared_ptr.h"
#include "weak_ptr.h"

class MyClass : public lhy::enable_shared_from_this<MyClass> {
 public:
  MyClass(int val) : value(val) { std::cout << "MyClass constructor\n"; }
  ~MyClass() { std::cout << "MyClass destructor\n"; }
  int getValue() const { return value; }
  lhy::shared_ptr<MyClass> getShared() { return shared_from_this(); }

 private:
  int value;
};

void TestEnableSharedFromThis() {
  // Create a shared_ptr managing a MyClass object
  lhy::shared_ptr<MyClass> sp1 = lhy::make_shared<MyClass>(42);
  assert(sp1->getValue() == 42);
  std::cout << "Initial use_count: " << sp1.use_count() << "\n";

  // Obtain another shared_ptr from within the object using shared_from_this
  lhy::shared_ptr<MyClass> sp2 = sp1->getShared();
  assert(sp1.use_count() == 2);
  assert(sp2.use_count() == 2);
  std::cout << "Use count after shared_from_this: " << sp1.use_count() << "\n";

  // Both shared_ptrs should point to the same object
  assert(sp1.get() == sp2.get());

  // Reset one of the shared_ptrs and check that the use count decreases
  sp2.reset();
  assert(sp1.use_count() == 1);
  std::cout << "Use count after resetting sp2: " << sp1.use_count() << "\n";

  // Ensure that shared_from_this throws an exception when called without an owner
  try {
    MyClass stackObject(24);
    stackObject.shared_from_this();
    assert(false);  // This should not be reached
  } catch (const lhy::bad_weak_ptr&) {
    std::cout << "Correctly caught bad_weak_ptr exception\n";
  }

  // Test weak_ptr interaction
  lhy::weak_ptr<MyClass> wp1(sp1);
  assert(!wp1.expired());
  lhy::shared_ptr<MyClass> sp3 = wp1.lock();
  assert(sp3.use_count() == 2);
  assert(sp1.get() == sp3.get());

  // After resetting the last shared_ptr, the object should be destroyed
  sp1.reset();
  sp3.reset();
  assert(wp1.expired());
  std::cout << "All tests passed!\n";
}

int main() {
  TestEnableSharedFromThis();
  return 0;
}
