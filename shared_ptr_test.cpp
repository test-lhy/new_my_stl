#include "shared_ptr.h"

#include <cassert>
#include <iostream>

class TestClass {
 public:
  TestClass() { std::cout << "TestClass constructed\n"; }
  ~TestClass() { std::cout << "TestClass destructed\n"; }
  void sayHello() { std::cout << "Hello from TestClass\n"; }
};

void testConstructorsAndAssignment() {
  // Test default constructor
  lhy::shared_ptr<TestClass> sp1;
  assert(!sp1);  // Should be false

  // Test constructor with pointer
  auto rawPtr = new TestClass();
  lhy::shared_ptr<TestClass> sp2(rawPtr);
  assert(sp2);  // Should be true

  // Test copy constructor
  lhy::shared_ptr<TestClass> sp3 = sp2;
  assert(sp3.use_count() == 2);

  // Test move constructor
  lhy::shared_ptr<TestClass> sp4 = std::move(sp3);
  assert(sp4.use_count() == 2);
  assert(!sp3);  // sp3 should be null after move

  // Test copy assignment
  lhy::shared_ptr<TestClass> sp5;
  sp5 = sp4;
  assert(sp5.use_count() == 3);

  // Test move assignment
  lhy::shared_ptr<TestClass> sp6;
  sp6 = std::move(sp5);
  assert(sp6.use_count() == 3);
  assert(!sp5);  // sp5 should be null after move
}

void testResetAndGet() {
  auto rawPtr = new TestClass();
  lhy::shared_ptr<TestClass> sp(rawPtr);
  assert(sp.get() == rawPtr);

  auto newPtr = new TestClass();
  sp.reset(newPtr);
  assert(sp.get() == newPtr);
  assert(sp.use_count() == 1);  // New pointer, so count should be 1
}

void testArrayHandling() {
  auto rawArray = new int[5]{1, 2, 3, 4, 5};
  lhy::shared_ptr<int[]> sp(rawArray);
  assert(sp[0] == 1);
  assert(sp[4] == 5);
}

void testCustomDeleter() {
  auto rawPtr = new TestClass();
  bool customDeleterCalled = false;
  auto customDeleter = [&customDeleterCalled](TestClass* ptr) {
    delete ptr;
    customDeleterCalled = true;
  };
  { lhy::shared_ptr<TestClass> sp(rawPtr, customDeleter); }
  assert(customDeleterCalled);  // Custom deleter should be called on destruction
}

void testCustomGetValue() {
  auto rawPtr = new TestClass();
  lhy::shared_ptr<TestClass> sp1(rawPtr);
  auto newPtr = new TestClass();

  // Test copy constructor with custom get_value
  lhy::shared_ptr<TestClass> sp2(sp1, newPtr);
  assert(sp2.get() == newPtr);
  assert(sp2.use_count() == 2);
  assert(sp1.use_count() == 2);

  // Test move constructor with custom get_value
  lhy::shared_ptr<TestClass> sp3(std::move(sp1), newPtr);
  assert(sp3.get() == newPtr);
  assert(sp3.use_count() == 2);
  assert(!sp1);  // sp1 should be null after move
  delete newPtr;
}

int main() {
  testConstructorsAndAssignment();
  testResetAndGet();
  testArrayHandling();
  testCustomDeleter();
  testCustomGetValue();
  std::cout << "All tests passed!\n";
  return 0;
}
