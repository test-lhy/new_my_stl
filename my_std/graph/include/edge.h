//
// Created by lhy31 on 2024/1/8
//

#ifndef MY_STL_EDGE_H
#define MY_STL_EDGE_H
#include <type_traits>
namespace lhy {
class Node;
class Edge {
 public:
  Edge() = default;
  Edge(Node& first, Node& second) : first_(&first), second_(&second) {}
  virtual ~Edge() = default;
  virtual bool operator==(const Edge& other)const { return first_ == other.first_ && second_ == other.second_; }
  bool operator!=(const Edge& other)const { return !(*this == other); }
  Node& GetAnotherNode(const Node& other) { return *((&other == first_) ? second_ : first_); }
  Node& GetFirst() { return *first_; }
  Node& GetSecond() { return *second_; }
  [[nodiscard]] Edge GetReversed() const {
    Edge zan = *this;
    zan.first_ = second_;
    zan.second_ = first_;
    return zan;
  }
  [[nodiscard]] Node& GetAnotherNode(const Node& other) const { return *((&other == first_) ? second_ : first_); }
  [[nodiscard]] Node& GetFirst() const { return *first_; }
  [[nodiscard]] Node& GetSecond() const { return *second_; }

 private:
  Node* first_;
  Node* second_;
};
template <typename T>
concept edge_c = std::is_base_of_v<Edge, T>;
}  // namespace lhy
#endif  // MY_STL_EDGE_H