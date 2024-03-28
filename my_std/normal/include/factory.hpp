
#ifndef MY_STL_FACTORY_HPP_
#define MY_STL_FACTORY_HPP_

#include "strings.h"
#include "unordered_map.h"
////////////////////////////////created by xy
/**
 * @brief 创建函数为命名空间中的基类启用自动注册工厂模式
 * @param _namespace 命名空间
 * @param _type 基类名称
 * @param _func 创建的函数名
 */
#define enable_factory(_namespace, _type, _func)                                                        \
  namespace _namespace {                                                                                \
  class _type;                                                                                          \
  inline _type *_func(const string &type_name) { return Factory<_type>::Instance().Create(type_name); } \
  }

/**
 * @brief 创建函数为命名空间中的基类启用自动注册工厂模式
 * @param _namespace 命名空间
 * @tparam _ttype 模板参数
 * @tparam _ctype 对模板参数类型的限制
 * @param _type 基类名称
 * @param _func 创建的函数名
 */
#define enable_single_template_factory(_namespace, _ctype, _ttype, _type, _func) \
  namespace _namespace {                                                         \
  template <_ctype _ttype>                                                       \
  class _type;                                                                   \
  template <_ctype _ttype>                                                       \
  inline _type<_ttype> *_func(const string &type_name) {                         \
    return Factory<_type<_ttype>>::Instance().Create(type_name);                 \
  }                                                                              \
  }
namespace lhy {
/**
 * @brief 实际基类所属注册信息基类
 * @tparam B 实际基类名称
 */
template <class B>
class RegistryBase {
 public:
  virtual ~RegistryBase() = default;

  /**
   * @brief 预留子类构造接口
   * @return 指向对应子类对象的基类指针
   */
  virtual B *Create() = 0;
};

/**
 * @brief 实际基类所属工厂类
 * @tparam B 实际基类名称
 */
template <class B>
class Factory final {
 public:
  /**
   * @brief 获取工厂类唯一实例
   * @return 唯一实例的引用
   */
  static Factory &Instance() {
    static Factory factory;
    return factory;
  }

  /**
   * @brief 向工厂类注册信息
   * @param type_name 实际子类所属标识符
   * @param registry 实际子类所属注册信息子类指针
   */
  void Register(const string &type_name, RegistryBase<B> *registry) { registry_[type_name] = registry; }

  /**
   * @brief 实际构造子类对象
   * @param type_name 实际子类所属标识符
   * @return 指向实际子类对象的基类指针
   */
  B *Create(const string &type_name) { return registry_.exist(type_name) ? registry_[type_name]->Create() : nullptr; }

 private:
  /// @note 该类用于识别类继承关系，故采用单例模式
  Factory() = default;
  ~Factory() = default;

  /// 子类信息注册表
  unordered_map<string, RegistryBase<B> *> registry_;
};

/**
 * @brief 实际子类所属注册信息子类
 * @tparam B 基类名称
 * @tparam S 子类名称
 */
template <class B, class S>
class RegistrySub final : public RegistryBase<B> {
 public:
  /**
   * @brief 构造注册信息并向工厂注册自身
   * @param type_name 实际子类标识符
   */
  explicit RegistrySub(const string &type_name) {
    static_assert(std::is_base_of<B, S>());
    Factory<B>::Instance().Register(type_name, this);
  }

  /**
   * @brief 构造实际子类对象
   * @return 基类指针形式的子类对象
   */
  B *Create() final { return new S(); }
};
}  // namespace lhy

#endif  // MY_STL_FACTORY_HPP_
