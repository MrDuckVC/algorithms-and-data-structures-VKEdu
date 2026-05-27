/*
# Модуль №1

**Общие требования для всех задач**:

* Ввод/вывод отделены от решения.
* Не должно быть утечек памяти.

## Задача № 2. Порядок обхода (4 балла)

**Обязательная задача.**

Дано число $N < 10^{6}$ и последовательность целых чисел из $[-2^{31}..2^{31}]$ длиной $N$.

Требуется построить бинарное дерево, заданное наивным порядком вставки.

Т.е., при добавлении очередного числа $K$ в дерево с корнем root, если $root→Key ≤ K$, то узел $K$ добавляется в правое поддерево root; иначе в левое поддерево
$root$.

**Требования**:

* Рекурсия запрещена.
* Решение должно поддерживать передачу функции сравнения снаружи.

### № 2.1

Выведите элементы в порядке in-order (слева направо).

| in             | out     |
| :------------- | :------ |
| `3`<br>`2 1 3` | `1 2 3` |
| `3`<br>`1 2 3` | `1 2 3` |
| `3`<br>`3 1 2` | `1 2 3` |
*/
#include <functional>
#include <iostream>
#include <stack>

template <typename T, typename Compare = std::less<T>>
class BinarySearchTree {
 private:
  struct Node {
    T value;
    Node* left;
    Node* right;
    explicit Node(const T& val) : value(val), left(nullptr), right(nullptr) {}
  };

  Node* root_;
  Compare cmp_;

 public:
  explicit BinarySearchTree(const Compare& cmp = Compare()) : root_(nullptr), cmp_(cmp) {}

  ~BinarySearchTree() {
    if (!root_) return;

    std::stack<Node*> st;
    st.push(root_);

    while (!st.empty()) {
      Node* current = st.top();
      st.pop();

      if (current->left) st.push(current->left);
      if (current->right) st.push(current->right);

      delete current;
    }
  }

  BinarySearchTree(const BinarySearchTree&) = delete;
  BinarySearchTree& operator=(const BinarySearchTree&) = delete;

  void Add(const T& val) {
    if (!root_) {
      root_ = new Node(val);
      return;
    }

    Node* current = root_;
    while (true) {
      if (cmp_(val, current->value)) {
        if (!current->left) {
          current->left = new Node(val);
          break;
        }
        current = current->left;
      } else {
        if (!current->right) {
          current->right = new Node(val);
          break;
        }
        current = current->right;
      }
    }
  }

  void InOrderTraversal(const std::function<void(const T&)>& visit) const {
    std::stack<Node*> st;
    Node* current = root_;

    while (current != nullptr || !st.empty()) {
      while (current != nullptr) {
        st.push(current);
        current = current->left;
      }

      current = st.top();
      st.pop();

      visit(current->value);

      current = current->right;
    }
  }
};

int main() {
  int n;
  if (!(std::cin >> n)) return 0;

  BinarySearchTree<long long> bst;

  for (int i = 0; i < n; ++i) {
    long long val;
    std::cin >> val;
    bst.Add(val);
  }

  bool first = true;
  bst.InOrderTraversal([&first](const long long& val) {
    if (!first) {
      std::cout << " ";
    }
    std::cout << val;
    first = false;
  });
  std::cout << std::endl;

  return 0;
}
