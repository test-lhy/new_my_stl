//
// Created by lhy on 24-8-9.
//

#ifndef STRING_VIEW_H
#define STRING_VIEW_H
#include <unordered_set>

#include "span.h"
#include "strings.h"
#include "vector.h"

namespace lhy {

class string_view : public span<char> {
 public:
  using span::span;
  [[nodiscard]] std::optional<int> find(const string_view& str, Index start = 0) const;
  [[nodiscard]] std::optional<int> find(const string& str, Index start = 0) const;
};

template <typename T>
[[nodiscard]] vector<int> GetPrefix(const vector<T>& vec) {
  return GetPrefix(span<T>{vec});
}
/**
 * @brief n时间复杂度的前缀函数
 * @tparam T vector元素的类型
 * @param vec 被求vector/string
 * @return vector<int> 前缀数组
 */
template <typename T>
[[nodiscard]] vector<int> GetPrefix(const span<T>& vec) {
  vector<int> prefix{0};
  for (int i = 1; i < vec.size(); ++i) {
    int prefix_comp = prefix[i - 1];
    while (prefix_comp > 0 && vec[i] != vec[prefix_comp]) {
      // 对于前面的最长前缀匹配，每次寻找次小的前缀匹配，直到后一位匹配，每次寻找次小的方式是找0-当前最长前缀匹配的长度-1的字符串的前缀，因为两个部分相同，因此后部与前部要相同相当于找前部的最长前缀
      prefix_comp = prefix[prefix_comp - 1];
    }
    if (vec[i] == vec[prefix_comp]) {
      // 对于comp非0来说必相等，也就必加1,而对于comp=0来说，由于可能是由于prefix_comp>0退出的，所以并不知道，需要进行判断
      prefix_comp++;
    }
    prefix.push_back(prefix_comp);
  }
  return prefix;
}
template <typename T>
[[nodiscard]] vector<std::unordered_map<T, int>> GetKmpDFA(vector<T> vec, T split = '|') {
  return GetKmpDFA(span<T>{vec}, split);
}
/**
 * @warning:总感觉用unordered_map就很慌
 * @brief n*元素种类的KMP自动机预处理自动机
 * @tparam T vector元素的类型
 * @param vec 模式串
 * @param split 用于使得其能在匹配成功后继续匹配而在结尾加入的必然不会在模式串和匹配串中出现的分隔符
 * @return KMP的自动机
 * @paragraph
 * 实际上就是进行了路径压缩，保证对于每一个状态，其经过不同的新的输入都只需要走一步走到另一个状态，而原本可能要多步，这里使用了一点dp
 */
template <typename T>
[[nodiscard]] vector<std::unordered_map<T, int>> GetKmpDFA(span<T> vec, T split = '|') {
  vector<int> prefix(GetPrefix(vec));
  std::unordered_set<T> all_element(vec.begin(), vec.end());
  ///< 为了让他必须在结尾不等,来使得在匹配成功后也能继续跳回匹配下一个可能匹配的
  vector<std::unordered_map<T, int>> ans(vec.size() + 1);
  for (int i = 0; i < vec.size() + 1; ++i) {
    for (auto& each : all_element) {
      if (i == vec.size()) {
        ans[i][each] = ans[prefix[i - 1]][each];  ///< 让当前跳转到他同前后缀的状态通过each会跳转到的地方
        continue;
      }
      if (i > 0 && each != vec[i]) {
        ans[i][each] = ans[prefix[i - 1]][each];  ///< 让当前跳转到他同前后缀的状态通过each会跳转到的地方
      } else {
        ans[i][each] = i + (vec[i] == each);  ///< 当是0或者相等的时候，要么状态向前一步(相等)，要么原地停滞(0)
      }
    }
  }
  return ans;
}
template <typename T>
[[nodiscard]] vector<int> GetKMP(const vector<T>& source, const vector<T>& target) {
  return GetKMP(GetKmpDFA(source), source.size(), target);
}
template <typename T>
[[nodiscard]] vector<int> GetKMP(const span<T>& source, const vector<T>& target) {
  return GetKMP(GetKmpDFA(source), source.size(), target);
}
template <typename T>
[[nodiscard]] vector<int> GetKMP(const vector<T>& source, const span<T>& target) {
  return GetKMP(GetKmpDFA(source), source.size(), target);
}
/**
 * @brief KMP自动机
 * @tparam T vector元素的类型
 * @param source 模式串
 * @param target 匹配串
 * @return 所有匹配的首字符下标
 */
template <typename T>
[[nodiscard]] vector<int> GetKMP(const span<T>& source, const span<T>& target) {
  return GetKMP(GetKmpDFA(source), source.size(), target);
}
template <typename T>
[[nodiscard]] vector<int> GetKMP(const vector<std::unordered_map<T, int>>& DFA, const size_t source_size,
                                 const vector<T>& target) {
  return GetKMP(DFA, source_size, span<T>{target});
}
/**
 * @brief KMP自动机
 * @tparam T vector元素的类型
 * @param source 模式串
 * @param target 匹配串
 * @return 第一个的首字符下标
 */
template <typename T>
[[nodiscard]] std::optional<int> GetKMPFirst(const span<T>& source, const span<T>& target) {
  return GetKMPFirst(GetKmpDFA(source), source.size(), target);
}
template <typename T>
[[nodiscard]] std::optional<int> GetKMPFirst(const vector<T>& source, const span<T>& target) {
  return GetKMPFirst(GetKmpDFA(source), source.size(), target);
}
/**
 * @brief KMP自动机
 * @tparam T vector元素的类型
 * @param DFA 预处理得的模式串的自动机
 * @param source_size 模式串的长度
 * @param target 匹配串
 * @return 所有匹配的首字符下标
 * @paragraph 实际上就是以目标串为输入序列在自动机上移动
 */
template <typename T>
[[nodiscard]] vector<int> GetKMP(const vector<std::unordered_map<T, int>>& DFA, const size_t source_size,
                                 const span<T>& target) {
  int status = 0;
  vector<int> ans;
  for (size_t i = 0; i < target.size(); ++i) {
    if(auto now_map=DFA.At(status);now_map.contains(target[i])) {
      status =now_map.at(target[i]);
    } else {
      status =0;
    }
    if (status == source_size) {
      ans.push_back(i - source_size + 1l);
    }
  }
  return ans;
}
/**
 * @brief KMP自动机
 * @tparam T vector元素的类型
 * @param DFA 预处理得的模式串的自动机
 * @param source_size 模式串的长度
 * @param target 匹配串
 * @return 第一个的首字符下标
 * @paragraph 实际上就是以目标串为输入序列在自动机上移动
 */
template <typename T>
[[nodiscard]] std::optional<int> GetKMPFirst(const vector<std::unordered_map<T, int>>& DFA, const size_t source_size, const span<T>& target) {
  int status = 0;
  for (size_t i = 0; i < target.size(); ++i) {
    if(auto now_map=DFA.At(status);now_map.contains(target[i])) {
      status =now_map.at(target[i]);
    } else {
      status =0;
    }
    if (status == source_size) {
      return i-source_size+1;
    }
  }
  return {};
}
}  // namespace lhy

#endif  // STRING_VIEW_H
