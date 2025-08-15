# new_my_stl - C++标准库的简易实现与扩展

`new_my_stl` 是一个旨在实现和扩展C++标准库功能的项目，涵盖了数据结构、算法、网络编程、数据库交互以及高级语言特性等多个方面。

## 主要特性

### 核心库与数据结构

- **容器实现**: 实现了 `array`, `optional`, `span`, `tuple`, `string_view` 等多种标准容器和工具类。
- **动态内存管理**: 提供了 `shared_ptr`, `unique_ptr`, `weak_ptr`, `auto_ptr` 等智能指针的实现。
- **常用数据结构**: 实现了 `vector`, `list`, `stack`, `queue`, `priority_queue` 等基本数据结构。
- **图与树**: 包含 `graph` (有向图、无向图、带权重图), `trees` (森林), `binary_search_tree` (二叉搜索树), `FibTree` (
  斐波那契堆) 等图和树结构及其算法。
- **哈希表**: 实现了 `unordered_map` (双哈希)。
- **索引容器**: 增加了 `IndexContainer` 类，支持基于索引的容器管理。

### 算法

- **排序算法**: 实现了多种排序算法，包括快速排序 (三路排序), 插入排序, 堆排序, 归并排序, 桶排序, 计数排序, 内省排序,
  TimSort, TournamentSort 等。
- **字符串算法**: 实现了 KMP 算法。
- **通用算法**: 提供了 `algorithm.h` 中的基础算法。

### 并发与网络

- **协程**: 引入了协程 (`co_epoll`, `co`) 以支持异步操作。
- **线程安全**: 实现了线程安全的双向链表 `ConMutexList`。
- **网络模块**: 提供了基础网络功能，支持 TCP/UDP 通信，并实现了 HTTP 服务器。
- **数据库交互**: 集成了数据库模块，支持 MySQL 交互。

### 高级语言特性与工具

- **反射**: 实现了 C++ 对象的反射机制，支持成员的获取、访问控制检查，以及序列化和反序列化。
- **宏编程**: 实现了宏加法器和宏展开器
- **类型萃取**: 提供了 `type_traits` 工具，用于编译期类型判断。
- **函数式编程**: 增加了 `functional.h` 中的相关功能。
- **错误处理**: 提供了自定义异常处理。
- **CMake构建**: 优化了 CMake 构建配置，使其更加模块化，并支持跨平台编译 (Mac, Linux, Docker)。
- **代码规范**: 添加了命名规范限制。

### 编译环境

- 支持 Mac, Linux, Docker 等平台。
- 优化了 CMake 配置以适应不同编译器。

### 性能分析

可以使用以下命令生成性能分析图：

```
gprof -b ./cmake-build-debug/${name} ./cmake-build-debug/gmon.out|gprof2dot -s|dot -Tpng -o ./debug_png/${name}.png
```