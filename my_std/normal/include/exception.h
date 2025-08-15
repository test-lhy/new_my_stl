//
// Created by lhy on 24-8-18.
//

#ifndef EXCEPTION_H
#define EXCEPTION_H
#include <exception>
#include <iostream>
#include <string>
namespace lhy {
#define CREATE_EXCEPTION_CLASS(class_name)                                                                       \
  class class_name : public exception {                                                                          \
   public:                                                                                                       \
    using exception::exception;                                                                                  \
    class_name& operator=(const class_name& other) noexcept = default;                                           \
    [[nodiscard]] std::string what() const noexcept override { return std::string{#class_name} + ":" + what_; }; \
  };
class exception {
 public:
  exception() noexcept = default;
  explicit exception(std::string&&) noexcept;
  exception(const exception& other) noexcept = default;
  virtual ~exception() noexcept = default;
  exception& operator=(const exception& other) noexcept = default;
  [[nodiscard]] virtual std::string what() const noexcept;

 protected:
  std::string what_;
};

CREATE_EXCEPTION_CLASS(logic_error);
CREATE_EXCEPTION_CLASS(out_of_range);
CREATE_EXCEPTION_CLASS(invalid_argument);
CREATE_EXCEPTION_CLASS(range_error);
CREATE_EXCEPTION_CLASS(bad_optional_access);
CREATE_EXCEPTION_CLASS(bad_weak_ptr);
#undef CREATE_EXCEPTION_CLASS
}  // namespace lhy

#endif  // EXCEPTION_H
