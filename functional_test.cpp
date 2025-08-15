//
// Created by lhy on 24-8-13.
//
#include "functional.h"  // Replace with the actual path to your header file

#include <cassert>
#include <iostream>
#include <string>
#include <vector>

// Test 1: Basic functionality with a lambda
void testLambda() {
  lhy::function<int(int, int)> add = [](int a, int b) { return a + b; };
  assert(add(2, 3) == 5);
  std::cout << "Test 1 passed: Lambda basic functionality" << std::endl;
}

// Test 2: Function pointer
int multiply(int a, int b) { return a * b; }

void testFunctionPointer() {
  lhy::function<int(int, int)> mul = multiply;
  assert(mul(3, 4) == 12);
  std::cout << "Test 2 passed: Function pointer functionality" << std::endl;
}

// Test 3: Functor
struct Subtract {
  int operator()(int a, int b) { return a - b; }
};

void testFunctor() {
  lhy::function<int(int, int)> sub = Subtract();
  assert(sub(10, 3) == 7);
  std::cout << "Test 3 passed: Functor functionality" << std::endl;
}

// Test 4: Move semantics
void testMoveSemantics() {
  lhy::function<int(int, int)> add = [](int a, int b) { return a + b; };
  lhy::function<int(int, int)> movedAdd = std::move(add);

  assert(!add);  // add should be empty after the move
  assert(movedAdd(1, 2) == 3);
  std::cout << "Test 4 passed: Move semantics" << std::endl;
}

// Test 5: Void return type
void testVoidReturnType() {
  bool called = false;
  lhy::function<void()> f = [&called]() { called = true; };
  f();
  assert(called);
  std::cout << "Test 5 passed: Void return type" << std::endl;
}

// Test 6: Capturing lambda
void testCapturingLambda() {
  int factor = 10;
  lhy::function<int(int)> mulByFactor = [factor](int a) { return a * factor; };
  assert(mulByFactor(3) == 30);
  std::cout << "Test 6 passed: Capturing lambda" << std::endl;
}

// Test 7: No arguments, no return value
void testNoArgsNoReturn() {
  bool called = false;
  lhy::function<void()> f = [&called]() { called = true; };
  f();
  assert(called);
  std::cout << "Test 7 passed: No arguments, no return value" << std::endl;
}

// Test 8: Function chaining
void testFunctionChaining() {
  lhy::function<int(int)> square = [](int x) { return x * x; };
  lhy::function<int(int)> doubleValue = [](int x) { return x * 2; };
  lhy::function<int(int)> composed = [square, doubleValue](int x) { return square(doubleValue(x)); };

  assert(composed(2) == 16);
  std::cout << "Test 8 passed: Function chaining" << std::endl;
}

// Test 9: Statefull functor
struct Stateful {
  int state = 0;
  int operator()(int x) {
    state += x;
    return state;
  }
};

void testStatefulFunctor() {
  Stateful sf;
  lhy::function<int(int)> f = sf;

  assert(f(3) == 3);
  assert(f(5) == 8);
  std::cout << "Test 9 passed: Stateful functor" << std::endl;
}

// Test 10: Reassigning function object
void testReassignFunction() {
  lhy::function<int(int, int)> f = [](int a, int b) { return a + b; };
  assert(f(3, 4) == 7);
  f = [](int a, int b) { return a * b; };
  assert(f(3, 4) == 12);
  std::cout << "Test 10 passed: Reassigning function object" << std::endl;
}

struct teststruct {
  int b;
  void fun(){};
};
int main() {
  testLambda();
  testFunctionPointer();
  testFunctor();
  testMoveSemantics();
  testVoidReturnType();
  testCapturingLambda();
  testNoArgsNoReturn();
  testFunctionChaining();
  testStatefulFunctor();
  testReassignFunction();
  lhy::function<void(teststruct&)> f = &teststruct::fun;
  lhy::function<void(teststruct*)> f_ = &teststruct::fun;
  lhy::function<int(teststruct&)> f__ = &teststruct::b;
  lhy::function<int(teststruct*)> f___ = &teststruct::b;
  teststruct b;
  f(b);
  f_(&b);
  f__(b);
  f___(&b);

  std::cout << "All tests passed!" << std::endl;
  return 0;
}
