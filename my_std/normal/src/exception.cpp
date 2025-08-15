//
// Created by lhy on 24-8-18.
//

#include "exception.h"

namespace lhy {
exception::exception(std::string&& what) noexcept { what_ = what; }
std::string exception::what() const noexcept { return what_; }
}  // namespace lhy