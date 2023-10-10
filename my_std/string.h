//
// Created by lhy on 23-10-8.
//
#include <unordered_map>
#include <unordered_set>

#include "algorithm.h"
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
[[deprecated("use GetPrefix instead of GetPrefixBad")]] vector<int> GetPrefixBad(const vector<T>& vec) {
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
[[deprecated("please use GetKMP which use KmpDFA")]]
vector<int> GetBadKMP(vector<T> target, const vector<T>& source, T split = '|') {
  int target_size = target.size();
  target += {split};
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
vector<int> GetBorder(const vector<T>& vec) {
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
long long GetDiff(const vector<T>& vec) {
  vector<T> vec_temp;
  long long ans = 0;
  vector<T> prefix;
  for (auto* it = vec.rbegin(); it != vec.rend(); it--) {
    vec_temp.push_back(*it);
    prefix = GetPrefix(vec_temp);
    ans += vec_temp.size() + 1 - max(prefix);
  }
  return ans;
}
/**
 * @warning:总感觉用unordered_map就很慌
 * @brief n*元素种类的KMP自动机预处理自动机
 * @tparam T vector元素的类型
 * @param vec 模式串
 * @param split 用于使得其能在匹配成功后继续匹配而在结尾加入的必然不会在模式串和匹配串中出现的分隔符
 * @return KMP的自动机
 * @paragraph 实际上就是进行了路径压缩，保证对于每一个状态，其经过不同的新的输入都只需要走一步走到另一个状态，而原本可能要多步，这里使用了一点dp
 */
template<typename T>
vector<std::unordered_map<T,int>> GetKmpDFA(vector<T> vec, T split = '|'){
  vec+={split};///<为了让他必须在结尾不等,来使得在匹配成功后也能继续跳回匹配下一个可能匹配的
  vector<int> prefix(GetPrefix(vec));
  std::unordered_set<T> all_element;
  vector<std::unordered_map<T,int>> ans(vec.size());
  for (auto &each:vec) {
    all_element.insert(each);
  }
  for (int i = 0; i < vec.size(); ++i) {
    for(auto &each:all_element){
      if (i>0&&each!=vec[i]){
        ans[i][each]=ans[prefix[i-1]][each];///<让当前跳转到他同前后缀的状态通过each会跳转到的地方
      }else{
        ans[i][each]=i+(vec[i]==each);///<当是0或者相等的时候，要么状态向前一步(相等)，要么原地停滞(0)
      }
    }
  }
  return ans;
}
/**
 * @brief KMP自动机
 * @tparam T vector元素的类型
 * @param source 模式串
 * @param target 匹配串
 * @return 所有匹配的首字符下标
 */
template <typename T>
vector<int> GetKMP(const vector<T>& source,const vector<T>& target){
  return GetKMP(GetKmpDFA(source),source.size(),target);
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
vector<int> GetKMP(vector<std::unordered_map<T,int>> DFA,size_t source_size,const vector<T>& target){
  int status=0;
  vector<int> ans;
  for (size_t i = 0; i < target.size(); ++i) {
    status=DFA[status][target[i]];
    if (status==source_size){
      ans.push_back(i-source_size+1l);
    }
  }
  return ans;
}
}  // namespace lhy
#endif  // MY_STL_STRING_H