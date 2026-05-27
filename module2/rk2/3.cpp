/*
**Условие**:

Нужно кастомизировать `std::unordered_set` для операций со структурой данных:

```cpp
struct Node {
    std::string product_name;
    int color;
    int size;
};
```

Для этого нужно сделать:

* специализацию `std::hash` для `Node`
* определить оператор `==`

Как сделать специализацию `std::hash`:

```cpp
namespace std {
    template<>
    struct hash<Node> {
        std::size_t operator()(const Node& node) const noexcept {
            // ...
        }
    };
}
```

* *Формат ввода*:
  Каждая строка входных данных задает одну операцию над множеством.
  Запись операции состоит из типа операции и следующей за ним через пробел тройки значений структуры, над которой проводится операция.

  Тип операции – один из трех символов:

  * `+` означает добавление данной строки в множество;
  * `-` означает удаление строки из множества;
  * `?` означает проверку принадлежности данной строки множеству.

  При добавлении элемента в множество НЕ ГАРАНТИРУЕТСЯ, что он отсутствует в этом множестве.
  При удалении элемента из множества НЕ ГАРАНТИРУЕТСЯ, что он присутствует в этом множестве.

* *Формат вывода*:
    Программа должна вывести для каждой операции одну из двух строк `OK` или `FAIL`, в зависимости от того, встречается ли данные в нашем множестве.
*/

#include <iostream>
#include <string>
#include <unordered_set>

struct Node {
  std::string product_name;
  int color;
  int size;

  bool operator==(const Node& other) const { return product_name == other.product_name && color == other.color && size == other.size; }
};

namespace std {
template <>
struct hash<Node> {
  std::size_t operator()(const Node& node) const noexcept {
    std::size_t h1 = std::hash<std::string>{}(node.product_name);
    std::size_t h2 = std::hash<int>{}(node.color);
    std::size_t h3 = std::hash<int>{}(node.size);

    // Смешиваем хеши (используем алгоритм, аналогичный boost::hash_combine)
    // 0x9e3779b9 — константа, основанная на золотом сечении,
    // отлично распределяет биты и минимизирует коллизии
    std::size_t seed = h1;
    seed ^= h2 + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    seed ^= h3 + 0x9e3779b9 + (seed << 6) + (seed >> 2);

    return seed;
  }
};
}  // namespace std

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(NULL);

  std::unordered_set<Node> node_set;
  char operation;
  std::string name;
  int color, size;

  while (std::cin >> operation >> name >> color >> size) {
    Node current_node{name, color, size};

    if (operation == '+') {
      auto [it, inserted] = node_set.insert(current_node);
      if (inserted) {
        std::cout << "OK" << std::endl;
      } else {
        std::cout << "FAIL" << std::endl;
      }
    } else if (operation == '-') {
      if (node_set.erase(current_node) > 0) {
        std::cout << "OK" << std::endl;
      } else {
        std::cout << "FAIL" << std::endl;
      }
    } else if (operation == '?') {
      if (node_set.find(current_node) != node_set.end()) {
        std::cout << "OK" << std::endl;
      } else {
        std::cout << "FAIL" << std::endl;
      }
    }
  }

  return 0;
}
