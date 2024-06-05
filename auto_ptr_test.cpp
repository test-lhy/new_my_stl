//
// Created by lhy on 24-6-5.
//
#include <cassert>
#include <iostream>

#include "shared_ptr.h"
#include "unique_ptr.h"
#include "weak_ptr.h"

class Test {
 public:
  Test(int value) : value_(value) { std::cout << "Test object created with value: " << value_ << std::endl; }
  ~Test() { std::cout << "Test object with value " << value_ << " destroyed" << std::endl; }
  int getValue() const { return value_; }

 private:
  int value_;
};

void uniquePtrTest() {
  std::cout << "\nStarting unique_ptr tests..." << std::endl;

  lhy::unique_ptr<Test> uPtr1 = lhy::make_unique<Test>(10);
  assert(uPtr1->getValue() == 10);

  lhy::unique_ptr<Test> uPtr2 = std::move(uPtr1);
  assert(!uPtr1 && uPtr2->getValue() == 10);

  uPtr2.reset(new Test(20));
  assert(uPtr2->getValue() == 20);

  Test* rawPtr = uPtr2.release();
  assert(!uPtr2 && rawPtr->getValue() == 20);
  delete rawPtr;

  std::cout << "unique_ptr tests passed.\n" << std::endl;
}

void sharedPtrTest() {
  std::cout << "Starting shared_ptr tests..." << std::endl;

  lhy::shared_ptr<Test> sPtr1 = lhy::make_shared<Test>(30);
  assert(sPtr1->getValue() == 30);
  assert(sPtr1.use_count() == 1);

  lhy::shared_ptr<Test> sPtr2 = sPtr1;
  assert(sPtr1.use_count() == 2);
  assert(sPtr2.use_count() == 2);

  sPtr2.reset(new Test(40));
  assert(sPtr2->getValue() == 40);
  assert(sPtr1.use_count() == 1 && sPtr2.use_count() == 1);

  lhy::shared_ptr<Test> sPtr3 = std::move(sPtr2);
  assert(!sPtr2 && sPtr3->getValue() == 40);

  sPtr3.reset();
  assert(!sPtr3);

  std::cout << "shared_ptr tests passed.\n" << std::endl;
}

void weakPtrTest() {
  std::cout << "Starting weak_ptr tests..." << std::endl;

  lhy::shared_ptr<Test> sPtr = lhy::make_shared<Test>(50);
  lhy::weak_ptr<Test> wPtr1 = sPtr;
  assert(!wPtr1.expired());

  lhy::shared_ptr<Test> sPtr2 = wPtr1.lock();
  assert(sPtr2->getValue() == 50 && sPtr2.use_count() == 2);

  sPtr.reset();

  lhy::shared_ptr<Test> sPtr3 = wPtr1.lock();
  assert(sPtr3);

  std::cout << "weak_ptr tests passed.\n" << std::endl;
}

void mixedTest() {
  std::cout << "Starting mixed tests..." << std::endl;

  lhy::unique_ptr<Test> uPtr = lhy::make_unique<Test>(60);
  lhy::shared_ptr<Test> sPtr = lhy::make_shared<Test>(70);
  lhy::weak_ptr<Test> wPtr = sPtr;

  lhy::shared_ptr<Test> sPtr2 = std::move(uPtr);
  assert(sPtr2->getValue() == 60);

  lhy::shared_ptr<Test> sPtr3 = wPtr.lock();
  assert(sPtr3->getValue() == 70);

  wPtr.reset();
  assert(wPtr.expired());

  sPtr.reset();
  assert(sPtr3.use_count() == 1);

  std::cout << "Mixed tests passed.\n" << std::endl;
}

int main() {
  uniquePtrTest();
  sharedPtrTest();
  weakPtrTest();
  mixedTest();

  std::cout << "All tests passed." << std::endl;
  return 0;
}
