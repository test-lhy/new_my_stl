#include "unique_ptr.h"

#include <cassert>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

// 自定义删除器
struct CustomDeleter {
  void operator()(int* ptr) const {
    delete ptr;
    std::cout << "CustomDeleter called for int\n";
  }

  void operator()(std::string* ptr) const {
    delete ptr;
    std::cout << "CustomDeleter called for std::string\n";
  }
};

// 测试类
struct TestClass {
  int value;
  TestClass(int v) : value(v) {}
  ~TestClass() { std::cout << "Destructor called for TestClass with value " << value << "\n"; }
};

// 测试函数
void test_unique_ptr() {
  // 默认构造函数
  lhy::unique_ptr<int> ptr1;
  assert(!ptr1);

  // 带指针的构造函数
  lhy::unique_ptr<int> ptr2(new int(42));
  assert(ptr2);
  assert(*ptr2 == 42);

  // 带指针和自定义删除器的构造函数
  lhy::unique_ptr<int> ptr3(new int(100), CustomDeleter());
  assert(ptr3);
  assert(*ptr3 == 100);

  // 移动构造函数
  lhy::unique_ptr<int> ptr4(std::move(ptr2));
  assert(ptr4);
  assert(*ptr4 == 42);
  assert(!ptr2);

  // 移动赋值操作符
  ptr2 = std::move(ptr4);
  assert(ptr2);
  assert(*ptr2 == 42);
  assert(!ptr4);

  // release 方法
  int* rawPtr = ptr2.release();
  assert(rawPtr);
  assert(!ptr2);
  delete rawPtr;

  // reset 方法
  ptr3.reset(new int(200));
  assert(ptr3);
  assert(*ptr3 == 200);

  // swap 方法
  lhy::unique_ptr<int> ptr5(new int(300));
  ptr3.swap(ptr5);
  assert(ptr3);
  assert(*ptr3 == 300);
  assert(ptr5);
  assert(*ptr5 == 200);

  // operator bool
  assert(ptr3);
  assert(ptr5);

  // 测试数组支持
  lhy::unique_ptr<int[]> arr(new int[3]{1, 2, 3});
  assert(arr);
  assert(arr[0] == 1);
  assert(arr[1] == 2);
  assert(arr[2] == 3);

  // 测试删除器访问
  auto& deleter = ptr3.get_deleter();
  std::cout << "ptr3 deleter address: " << &deleter << std::endl;

  // 测试自定义删除器
  lhy::unique_ptr<std::string> ptr6(new std::string("Hello, World!"), CustomDeleter());
  assert(ptr6);
  assert(*ptr6 == "Hello, World!");

  // 测试移动构造函数和移动赋值操作符的异常安全性
  try {
    std::vector<lhy::unique_ptr<TestClass>> vec;
    vec.reserve(100);
    for (int i = 0; i < 100; ++i) {
      vec.emplace_back(new TestClass(i));
    }

    std::vector<lhy::unique_ptr<TestClass>> vec2;
    vec2.reserve(100);
    for (int i = 0; i < 100; ++i) {
      vec2.emplace_back(std::move(vec[i]));
    }

    std::vector<lhy::unique_ptr<TestClass>> vec3;
    vec3 = std::move(vec2);
  } catch (...) {
    assert(false);  // 如果发生异常,测试失败
  }

  std::cout << "All tests passed!" << std::endl;
}

int main() {
  test_unique_ptr();
  lhy::unique_ptr<int> a=lhy::make_unique<int>(1);
  lhy::unique_ptr<int> b=lhy::make_unique<int>(std::move(a));
  return 0;
}