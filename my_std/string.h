//
// Created by lhy on 23-10-8.
//
#include "vector.h"
#ifndef MY_STL_STRING_H
#define MY_STL_STRING_H
namespace lhy {
using string = vector<char>;
/**
 * @brief n时间复杂度的前缀函数
 * @tparam T vector元素的类型
 * @param vec 被求vector/string
 * @return vector<int> 前缀数组
 */
template <typename T>
vector<int> GetPrefix(const vector<T>& vec) {
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
/**
 * @brief n^2时间复杂度的前缀函数
 * @tparam T vector元素的类型
 * @param vec 被求vector/string
 * @return 前缀数组
 * @deprecated 用GetPrefix
 */
template <typename T>
vector<int> GetPrefixBad(const vector<T>& vec) {
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
 */
template<typename T>
vector<int> GetKMP(vector<T> target,const vector<T>& source,T split='|'){
  int target_size=target.size();
  target+={split};
  target+=source;
  vector<int> prefix= GetPrefix(target);
  vector<int> ans;
  for (int i = target.size()-source.size(); i < prefix.size(); ++i) {
    if (prefix[i]==target_size){
      ans.push_back(i-2*target_size);
    }
  }
  return ans;
}
}  // namespace lhy
#endif  // MY_STL_STRING_H