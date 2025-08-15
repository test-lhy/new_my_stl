//
// Created by lhy on 24-8-9.
//
#include "string_view.h"
std::optional<int> lhy::string_view::find(const string_view& str, const Index start) const {
  return GetKMPFirst(str, {begin() + start, end()});
}
std::optional<int> lhy::string_view::find(const string& str, const Index start) const {
  return GetKMPFirst(str, {begin() + start, end()});
}