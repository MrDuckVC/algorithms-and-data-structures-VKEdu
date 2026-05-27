/*
**Условие**:

Напишите функцию, проверяющую, что во всех узлах бинарного дерева поиска хранится одинаковое значение.
Значения - `int`.

* Шаблоны не обязательны.
* Рекурсия разрешена.

* *Формат ввода*: По очереди вводятся узлы бинарного дерева в порядке вставки.
* *Формат вывода*:
  * `0` - если все значения в узлах бинарного дерева разные.
  * `1` - если все значения в узлах бинарного дерева одинаковые.
*/

#include <iostream>
#include <vector>

struct Node {
  int value;
  Node* left;
  Node* right;

  explicit Node(int val) : value(val), left(nullptr), right(nullptr) {}
};

void Add(Node*& root, int val) {
  if (!root) {
    root = new Node(val);
    return;
  }

  Node* current = root;
  while (true) {
    if (val < current->value) {
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

void DestroyTree(Node* root) {
  if (!root) return;

  std::vector<Node*> stack;
  stack.push_back(root);

  while (!stack.empty()) {
    Node* current = stack.back();
    stack.pop_back();

    if (current->left) stack.push_back(current->left);
    if (current->right) stack.push_back(current->right);

    delete current;
  }
}

bool AreAllValuesSame(const Node* node, int target_value) {
  if (!node) {
    return true;
  }

  if (node->value != target_value) {
    return false;
  }

  return AreAllValuesSame(node->left, target_value) && AreAllValuesSame(node->right, target_value);
}

bool CheckTree(const Node* root) {
  if (!root) return true;
  return AreAllValuesSame(root, root->value);
}

int main() {
  Node* root = nullptr;
  int val;

  while (std::cin >> val) {
    Add(root, val);
  }

  if (CheckTree(root)) {
    std::cout << 1 << std::endl;
  } else {
    std::cout << 0 << std::endl;
  }

  DestroyTree(root);

  return 0;
}
