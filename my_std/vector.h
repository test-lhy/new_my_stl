//
// Created by lhy31 on 2023/8/21.
//

#ifndef MY_STL_VECTOR_H
#define MY_STL_VECTOR_H

#include <algorithm>
#include <cstdint>
#include <stdexcept>

namespace lhy {
template <typename T>
class vector {
public:
  vector(); //vector的默认构造函数

  explicit vector(const size_t&); //为什么要加一个&？ C++ 支持把引用作为参数传给函数，这比传一般的参数更安全。
  //explicit：“禁止单参数构造函数”被用于自动型别转换，其中比较典型的例子就是容器类型。在这种类型的构造函数中你可以将初始长度作为参数传递给构造函数。

  vector(const std::initializer_list<T>&); //初始化{}构造函数
  //std::initializer_list 通常用于构造函数和函数重载，使得对象可以使用大括号 {} 初始化语法。

  ~vector(); //vector的默认虚构函数

  vector<T>& operator=(const std::initializer_list<T>&);
  //初始化列表构造函数，如果没有为 std::vector<T> 提供接受 std::initializer_list<T> 的赋值运算符重载，你将无法使用初始化列表的语法进行赋值，而需要依赖于其他赋值方法。

  void push_back(const T&); //可以使用 push_back 方法向 vector 中添加元素：

  T& operator[](const size_t&);
  //一个 size_t 类型的常量引用，这表示索引值必须是正整数，并且在整个函数调用过程中保持不变。返回类型是 T&，这意味着返回值是一个引用，允许对 vector 中的元素进行直接修改。
  //允许你通过索引来访问 vector 中的元素，并且返回一个引用，这样就可以直接修改 vector 中元素的值。

  T& At(const size_t&); //大写小写有区别吗？
  // at 函数提供了一种安全的方式来访问向量中的元素，因为它会在索引越界时抛出异常，而不是默默地产生未定义行为。这使得 at 在需要范围检查的情况下是一个更好的选择。

  void clear();

  size_t size() const;

  bool empty() const;

  T* begin() const;

  T* end() const;

  const T* cend() const;

  const T* cbegin() const;

private:
  using size_T = int64_t;
  //使用using关键字可以创建一个类型别名。

  T* start_;
  T* end_;
  T* volume_;

  void check_volume();

  void check_index(const size_t&) const;
};

template <typename T>
T& vector<T>::operator[](const size_t& index) {
  return At(index);
}

template <typename T>
void vector<T>::check_index(const size_t& index) const {
  if (index < 0 || index >= size()) {
    throw std::logic_error("out of range");
  }
}

template <typename T>
T& vector<T>::At(const size_t& index) {
  check_index(index);
  return *(start_ + index);
}

template <typename T>
vector<T>::vector(const std::initializer_list<T>& element_list) : vector() {
  for (auto& element : element_list) {
    push_back(element);
  }
}

//使用了 : vector() 语法，这是一种委托构造函数的用法。这行代码调用了 vector 类的另一个构造函数，这个构造函数不接受任何参数，通常用于创建一个空向量。这样，即使 element_list 是空的，这个构造函数也能正确地创建一个向量，并且不会因为初始化列表为空而抛出异常。
//上面for述句的第一部分定义被用来做范围迭代的变量，就像被声明在一般for循环的变量一样，其作用域仅只于循环的范围。而在":"之后的第二区块，代表将被迭代的范围。
//auto 类型也是 C++11 新标准中的，用来自动获取变量的类型

template <typename T>
vector<T>& vector<T>::operator=(const std::initializer_list<T>& element_list) {
  clear();
  for (auto& element : element_list) {
    push_back(element);
  }
  return *this;
}
//在重载赋值运算符时，返回 this 的引用是常见的做法，因为它提供了语法上的便利和灵活性。

template <typename T>
vector<T>::vector(const size_t& size) {
  start_ = new T[size];
  //new 关键字在堆上动态分配一个 T 类型元素的数组，数组的大小由参数 size 指定。start_ 是一个指向 T 类型元素的指针，它被设置为分配的内存块的起始地址。
  end_ = start_;
  //将 end_ 成员变量设置为 start_，这意味着当前向量中没有元素。在 std::vector 的实现中，end_ 指针通常指向向量中最后一个元素的下一个位置，或者初始时与 start_ 相同。
  volume_ = start_ + size;
  //volume_ 指针通常指向分配的内存块的末尾，即 start_ 加上 size 乘以 T 类型元素的大小。
}

template <typename T>
const T* vector<T>::cbegin() const {
  return const_cast<const T*>(end_);
}
//const_cast 运算符来去除 start_ 指针的常量性。

template <typename T>
const T* vector<T>::cend() const {
  return const_cast<const T*>(start_);
}

template <typename T>
T* vector<T>::end() const {
  return end_;
}

template <typename T>
T* vector<T>::begin() const {
  return start_;
}

template <typename T>
bool vector<T>::empty() const {
  return size() == 0;
}
//== 运算符被用来比较 size() 函数的返回值和整数值 0。这里，size() 函数应该返回向量中的元素数量。如果 size() 返回 0，则 size() == 0 的比较结果为 true，这意味着向量中没有元素，empty() 函数因此返回 true，表示向量是空的。如果 size() 返回的值不是 0，则比较结果为 false，empty() 函数返回 false，表示向量不是空的。

template <typename T>
size_t vector<T>::size() const {
  return end_ - start_;
}

template <typename T>
void vector<T>::clear() {
  delete[] start_;
  start_ = new T[1];
  end_ = start_;
  volume_ = start_ + 1;
}

/*
template <typename T>
void vector<T>::clear() {
end_ = start_;
volume_ = start_ ;
}
*/

template <typename T>
void vector<T>::check_volume() {
  if (end_ == volume_) {
    int64_t size = (volume_ - start_);
    T* start_substitute = new T[size * 2];

    //当前分配的内存空间不足以容纳更多的元素时，通常的策略是分配一个新的、更大的内存块，并将现有元素复制到这个新块中。选择新块大小为当前长度的两倍
    end_ = start_substitute + size;
    volume_ = start_substitute + size * 2;
    for (int i = 0; i < size; ++i) {
      *(start_substitute + i) = std::move(*(start_ + i));
      //这行使用std::move将旧内存块中的元素移动到新内存块的相应位置。如果T类型的元素不可移动，这将调用拷贝构造函数。
      //一旦一个对象被 std::move，它应该被视为已转移状态，不应再被使用，除非它被重新初始化或赋值。
    }
    delete[] start_;
    start_ = start_substitute;
  }
}

template <typename T>
void vector<T>::push_back(const T& element) {
  check_volume();
  *end_ = element;
  end_++;
}
} // namespace lhy
namespace lhy {
template <typename T>
vector<T>::vector() : vector(1) {
}

//vector<T>::vector() 表示一个 vector 类的默认构造函数，它不接受任何参数。冒号 : 后面的 vector(1) 表示这个默认构造函数委托给了一个接受单个 size_t 类型参数的构造函数，这个参数的值是 1。

template <typename T>
vector<T>::~vector() {
  delete[] start_; //动态分配内存，防止内存泄漏
}
} // namespace lhy
#endif  // MY_STL_VECTOR_H

//at 大小写
//