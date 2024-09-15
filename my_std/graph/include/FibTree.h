//
// Created by lhy on 24-9-15.
//

#ifndef FIB_TREE_H
#define FIB_TREE_H
#include <cassert>

#include "basic.h"
#include "optional.h"
#include "trees.h"
#include "unordered_map.h"
namespace lhy {
/// 建议还是查看test中的Fib
template <typename T>
class FibData {
 public:
  FibData() = default;
  FibData(const T &data) : data_(data) {}
  friend bool operator==(const FibData &lhs, const FibData &rhs) {
    return lhs.data_ == rhs.data_ && lhs.mark_ == rhs.mark_;
  }
  T data_{};
  bool mark_{};
};
template <typename T>
using FibNode = WeightedNode<FibData<T>>;
template <typename T>
class FibTree : public Trees<FibNode<T>> {
 public:
  friend void show(FibTree<int> &tree, int count);
  using Base = Trees<FibNode<T>>;
  using iterator = typename vector<T>::iterator;
  explicit FibTree(const CmpType<T> &cmp = std::less<T>());
  FibTree(TForwardIterator<T>, TForwardIterator<T>, const CmpType<T> &cmp = std::less<T>());
  ~FibTree() override = default;
  [[nodiscard]] bool empty() const;
  void push(const T &);
  void pop();
  T top();
  T TopAndPop();

 private:
  void consolidate();
  CmpType<T> compare_function_;
  optional<Index> min_;
  int index_max_{};
};
template <typename T>
FibTree<T>::FibTree(const CmpType<T> &cmp) : compare_function_(cmp) {}
template <typename T>
FibTree<T>::FibTree(TForwardIterator<T> first, TForwardIterator<T> last, const CmpType<T> &cmp)
    : compare_function_(cmp) {
  for (; first != last; ++first) {
    push(*first);
  }
}
template <typename T>
bool FibTree<T>::empty() const {
  return Base::roots_.empty();
}
template <typename T>
void FibTree<T>::push(const T &data) {
  auto node = Base::AddRoot({data, ++index_max_});
  if (!min_.has_value()) {
    min_ = *node;
  } else {
    if (compare_function_(data, top())) {
      min_ = *node;
    }
  }
}
template <typename T>
void FibTree<T>::pop() {
  /// 这里没有清parent
  vector<Index> to_delete{};
  for (auto &each_node : Base::GetNode(*min_).GetEdges()) {
    to_delete.push_back(each_node.GetAnotherNode(*min_));
  }
  for (auto &each_node : to_delete) {
    Base::SetRoot(each_node);
  }
  Base::DeleteNode(*min_);
  Base::roots_.erase(*min_);
  if (empty()) {
    min_.reset();
  } else {
    min_ = *Base::roots_.begin();
    consolidate();
  }
}
template <typename T>
T FibTree<T>::top() {
  if (empty()) {
    throw logic_error("empty tree");
  }
  return Base::GetNode(*min_).GetNode().GetWeight().data_;
}
template <typename T>
T FibTree<T>::TopAndPop() {
  T ret = top();
  pop();
  return ret;
}
template <typename T>
void FibTree<T>::consolidate() {
  unordered_map<int, std::pair<long, list<Index>::iterator>> degree_map{};
  for (auto cur_iter = Base::roots_.begin(); cur_iter != Base::roots_.end(); ++cur_iter) {
    auto degree = Base::GetNode(*cur_iter).GetEdges().size();
    auto &cur_data = Base::GetNode(*cur_iter).GetNode().GetWeight().data_;
    while (degree_map.contains(degree)) {
      auto tmp = degree_map[degree];
      auto &tmp_data = Base::GetNode(tmp.first).GetNode().GetWeight().data_;
      if (compare_function_(tmp_data, cur_data)) {
        tmp.second.Last().Next() = cur_iter;
        cur_iter.Next().Last() = tmp.second;
        if (tmp.second.Next() == cur_iter) {
          cur_iter.Last() = tmp.second.Last();
          tmp.second.Next() = cur_iter.Next();
          cur_iter.Next() = tmp.second;
          tmp.second.Last() = cur_iter;
        } else {
          auto tmp_next = tmp.second.Next();
          auto cur_last = cur_iter.Last();
          cur_iter.Last() = tmp.second.Last();
          tmp.second.Next() = cur_iter.Next();
          tmp_next.Last() = cur_iter;
          cur_iter.Next() = tmp_next;
          cur_last.Next() = tmp.second;
          tmp.second.Last() = cur_last;
        }
        std::swap(cur_iter, tmp.second);
        // std::swap(cur, *tmp.second);
        // Swap tmp and cur in the list
      }
      Base::ChangeFather(tmp.second, *cur_iter);
      degree_map.erase(degree);
      ++degree;
    }
    degree_map[Base::GetNode(*cur_iter).GetEdges().size()] = {*cur_iter, cur_iter};
  }
  min_ = *Base::roots_.begin();
  for (auto &each_pair : Base::roots_) {
    if (compare_function_(Base::GetNode(each_pair).GetNode().GetWeight().data_, top())) {
      min_ = each_pair;
    }
  }
}

}  // namespace lhy

#endif  // FIB_TREE_H
