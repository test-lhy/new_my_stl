//
// Created by lhy on 24-4-1.
//

#ifndef WEIGHTED_H
#define WEIGHTED_H
#include <utility>
namespace lhy {
template <typename WeightType, typename ObjectType>
class Weighted : public ObjectType {
 public:
  Weighted() = default;
  template <typename... Args>
  // note:只能放在最前面
  explicit Weighted(WeightType&& weight, Args&&... args) : ObjectType(std::forward<Args>(args)...), weight_(weight){};
  explicit Weighted(ObjectType&& other, WeightType&& weight)
      : ObjectType(std::forward<ObjectType>(other)), weight_(weight){};
  friend bool operator==(const Weighted& lhs, const Weighted& rhs) {
    return static_cast<const ObjectType&>(lhs) == static_cast<const ObjectType&>(rhs) && lhs.weight_ == rhs.weight_;
  }
  friend bool operator!=(const Weighted& lhs, const Weighted& rhs) { return !(lhs == rhs); }
  WeightType& GetWeight() { return weight_; }

 private:
  WeightType weight_{};
};

}  // namespace lhy

#endif  // WEIGHTED_H
