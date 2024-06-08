#include "shared_ptr.h"

#include <cassert>
#include <iostream>

class TestClass {
 public:
  TestClass() {
    a = new int();
    std::cout << "TestClass constructed\n";
  }
  ~TestClass() {
    delete a;
    std::cout << "TestClass destructed\n";
  }
  void sayHello() { std::cout << "Hello from TestClass\n"; }
  int *a;
};
class BaseClass {
 public:
  BaseClass() {
    a = new int();
    std::cout << "BaseClass constructed\n";
  }
  virtual ~BaseClass() {
    delete a;
    std::cout << "BaseClass destructed\n";
  }
  virtual void sayHello() { std::cout << "Hello from BaseClass\n"; }
  int *a;
};

class DerivedClass : public BaseClass {
 public:
  DerivedClass() {
    b = new int();
    std::cout << "DerivedClass constructed\n";
  }
  ~DerivedClass() override {
    delete b;
    std::cout << "DerivedClass destructed\n";
  }
  void sayHello() override { std::cout << "Hello from DerivedClass\n"; }
  int *b;
};

void testDerivedToBaseConversion() {
  // Test creating shared_ptr from derived class pointer
  lhy::shared_ptr<DerivedClass> derivedPtr = lhy::make_shared<DerivedClass>();
  derivedPtr->sayHello();

  // Convert derived shared_ptr to base shared_ptr
  lhy::shared_ptr<BaseClass> basePtr = derivedPtr;
  basePtr->sayHello();
  assert(basePtr.use_count() == 2);  // Both shared_ptr should point to the same object

  // Test reset with derived class pointer
  lhy::shared_ptr<BaseClass> anotherBasePtr;
  anotherBasePtr.reset(new DerivedClass());
  assert(anotherBasePtr.use_count() == 1);
}
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
  auto customDeleter = [&customDeleterCalled](TestClass *ptr) {
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

void testMakeShared() {
  // Test make_shared for non-array type
  auto sp1 = lhy::make_shared<TestClass[]>(10);
  assert(sp1);
  assert(sp1.use_count() == 1);

  // Test make_shared for array type
  auto sp2 = lhy::make_shared<int[]>(10);
  assert(sp2);
  assert(sp2.use_count() == 1);
  for (int i = 0; i < 10; ++i) {
    sp2[i] = i;
  }
  for (int i = 0; i < 10; ++i) {
    assert(sp2[i] == i);
  }

  // Test make_shared_for_overwrite for non-array type
  auto sp3 = lhy::make_shared_for_overwrite<TestClass[]>(100);
  assert(sp3);
  assert(sp3.use_count() == 1);

  // Test make_shared_for_overwrite for array type
  auto sp4 = lhy::make_shared_for_overwrite<int[]>(10);
  assert(sp4);
  assert(sp4.use_count() == 1);
  for (int i = 0; i < 10; ++i) {
    sp4[i] = i + 10;
  }
  for (int i = 0; i < 10; ++i) {
    assert(sp4[i] == i + 10);
  }
}
void testComplexScenario() {
  // Mixed scenario with multiple shared_ptr and different types
  auto basePtr1 = lhy::make_shared<BaseClass>();
  auto derivedPtr1 = lhy::make_shared<DerivedClass>();

  std::cout << "Initial counts: " << basePtr1.use_count() << " " << derivedPtr1.use_count() << "\n";

  lhy::shared_ptr<BaseClass> basePtr2 = derivedPtr1;
  assert(basePtr2.use_count() == 2);
  assert(derivedPtr1.use_count() == 2);

  basePtr1 = basePtr2;
  assert(basePtr1.use_count() == 3);
  assert(basePtr2.use_count() == 3);
  assert(derivedPtr1.use_count() == 3);

  basePtr1.reset();
  assert(basePtr2.use_count() == 2);
  assert(derivedPtr1.use_count() == 2);

  basePtr2.reset();
  assert(derivedPtr1.use_count() == 1);
}

void testMixedScenarios() {
  // Scenario with custom deleter and conversion
  auto rawPtr = new DerivedClass();
  bool customDeleterCalled = false;
  auto customDeleter = [&customDeleterCalled](BaseClass *ptr) {
    delete ptr;
    customDeleterCalled = true;
  };
  { lhy::shared_ptr<BaseClass> basePtr(rawPtr, customDeleter); }
  assert(customDeleterCalled);
}
class ComplexClass {
 public:
  ComplexClass(int a, double b) : a_(a), b_(b) { std::cout << "ComplexClass constructed\n"; }
  ~ComplexClass() { std::cout << "ComplexClass destructed\n"; }
  void display() { std::cout << "ComplexClass values: " << a_ << ", " << b_ << "\n"; }

 private:
  int a_;
  double b_;
};

void testMakeSharedWithArgs() {
  // Test make_shared with arguments
  auto sp1 = lhy::make_shared<ComplexClass>(42, 3.14);
  sp1->display();
  assert(sp1.use_count() == 1);
}

void testSharedPtrAlias() {
  // Test aliasing
  auto basePtr = lhy::make_shared<BaseClass>();
  auto derivedPtr = lhy::make_shared<DerivedClass>();
  basePtr = derivedPtr;  // Aliasing basePtr to derivedPtr
  basePtr->sayHello();
  assert(basePtr.use_count() == 2);
  assert(derivedPtr.use_count() == 2);
}

void testSharedPtrReset() {
  // Test reset with nullptr
  auto sp1 = lhy::make_shared<TestClass>();
  assert(sp1.use_count() == 1);
  sp1.reset();
  assert(!sp1);  // Should be null
}

void testSharedPtrSwap() {
  // Test swap
  auto sp1 = lhy::make_shared<TestClass>();
  auto sp2 = lhy::make_shared<TestClass>();
  sp1.swap(sp2);
  assert(sp1.use_count() == 1);
  assert(sp2.use_count() == 1);
}

void testArrayDeleter() {
  // Test custom deleter with array
  bool customDeleterCalled = false;
  auto customDeleter = [&customDeleterCalled](int *ptr) {
    delete[] ptr;
    customDeleterCalled = true;
  };
  { lhy::shared_ptr<int[]> sp(new int[10], customDeleter); }
  assert(customDeleterCalled);  // Custom deleter should be called
}

void testSharedPtrCircularReference() {
  // // Test circular reference handling
  // struct Node {
  //   lhy::shared_ptr<Node> next;
  //   ~Node() { std::cout << "Node destructed\n"; }
  // };
  //
  // auto node1 = lhy::make_shared<Node>();
  // auto node2 = lhy::make_shared<Node>();
  // node1->next = node2;
  // node2->next = node1;  // Circular reference
  //
  // node1.reset();
  // node2.reset();  // Nodes should be destructed without memory leak
}
int main() {
  testConstructorsAndAssignment();
  testResetAndGet();
  testArrayHandling();
  testCustomDeleter();
  testCustomGetValue();
  testMakeShared();
  testDerivedToBaseConversion();
  testComplexScenario();
  testMixedScenarios();
  testMakeSharedWithArgs();
  testSharedPtrAlias();
  testSharedPtrReset();
  testSharedPtrSwap();
  testArrayDeleter();
  testSharedPtrCircularReference();
  std::cout << "All tests passed!\n";
  return 0;
}
