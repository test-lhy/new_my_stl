//
// Created by lhy on 23-10-8.
//
#ifndef MY_STL_STRINGS_H
#define MY_STL_STRINGS_H
#include <unordered_map>
#include <unordered_set>

#include "vector.h"
namespace lhy {
class string : public vector<char> {
 public:
  using vector::vector;
  string(const string&) = default;
  string(string&&) noexcept = default;
  string& operator=(const string&);
  string& operator=(string&&) noexcept;
  string& operator=(const std::initializer_list<char>&);
  string& operator+=(const char&);
  string& operator+=(const string&);
  string& operator&=(const string&);
  string& operator|=(const string&);
};
void getline(std::istream& istream_, string& obj);
std::istream& operator>>(std::istream& istream_, string& obj);
string operator+(string a, const string& b);
string operator&(string a, const string& b);
string operator|(string a, const string& b);
/**
 * @brief n^2时间复杂度的前缀函数
 * @tparam T vector元素的类型
 * @param vec 被求vector/string
 * @return 前缀数组
 * @deprecated 用GetPrefix
 */
template <typename T>
[[deprecated("use GetPrefix instead of GetPrefixBad")]] [[nodiscard]] vector<int> GetPrefixBad(const vector<T>& vec) {
  vector<int> prefix{0};
  for (int i = 1; i < vec.size(); ++i) {
    for (int j = prefix[i - 1] + 1; j >= 0; --j) {
      bool flag{false};
      for (int k = 0; k < j; ++k) {
        if (vec[k] != vec[i - j + 1 + k]) {
          flag = true;
          break;
        }
      }
      if (!flag) {
        prefix.push_back(j);
        break;
      }
    }
  }
  return prefix;
}
/**
 * @brief n+m时间复杂度的KMP
 * @tparam T vector元素的类型
 * @param target 模式串
 * @param source 匹配串
 * @param split 在模式串和匹配串中都不会出现的一个T类型元素
 * @return 所有匹配的首字符下标
 * @paragraph 将模式串+不存在的字符+匹配串拼接，则对于匹配串部分得出的前缀函数如果等于模式串长度则匹配上，统计即可
 * @deprecated 请使用GetKMP，这个函数使用了DFA
 */
template <typename T>
[[deprecated("please use GetKMP which use KmpDFA")]] [[nodiscard]] vector<int> GetBadKMP(vector<T> target,
                                                                                         const vector<T>& source,
                                                                                         T split = '|') {
  int target_size = target.size();
  target += split;
  target += source;
  vector<int> prefix = GetPrefix(target);
  vector<int> ans;
  for (int i = target.size() - source.size(); i < prefix.size(); ++i) {
    if (prefix[i] == target_size) {
      ans.push_back(i - 2 * target_size);
    }
  }
  return ans;
}
/**
 * @warning 未测试
 * @brief n时间复杂度的求border
 * @tparam T vector元素的类型
 * @param vec 被求vector/string
 * @return vector<int> 所有border
 * @paragraph 长度为border的前缀与后缀相等,对于一个border,字符串长度-border为字符串的周期长度
 */
template <typename T>
[[nodiscard]] vector<int> GetBorder(const vector<T>& vec) {
  auto prefix = GetPrefix(vec);
  vector<int> ans;
  int prefix_now = vec.size() - 1;
  while (prefix_now > 0) {
    ans.push_back(prefix_now);
    prefix_now = prefix[prefix - 1];
  }
  return ans;
}
/**
 * @warning 未测试
 * @brief n^2时间复杂度的求本质不同的子串
 * @tparam T vector元素的类型
 * @param vec 被求vector/string
 * @return 本质不同的字串的个数
 * @paragraph
 * 对于当前在首位加入的新字符，这个新的串的新产生的不同的字串肯定是包括首字符的，也就是前缀，找到最长的出现过第二次的前缀，总长-这个就是新增加的数量
 */
template <typename T>
[[nodiscard]] long long GetDiff(const vector<T>& vec) {
  vector<T> vec_temp;
  long long ans = 0;
  vector<T> prefix;
  for (auto it = vec.rbegin(); it != vec.rend(); it++) {
    vec_temp.push_back(*it);
    prefix = GetPrefix(vec_temp);
    ans += vec_temp.size() + 1 - max(prefix);
  }
  return ans;
}
}  // namespace lhy
#endif  // MY_STL_STRINGS_H