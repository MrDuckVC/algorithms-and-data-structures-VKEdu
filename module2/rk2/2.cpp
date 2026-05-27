/*
**Условие**:

Напишите функцию, вычисляющую количество вершин на кратчайшем пути от корня дерева до листа.
Значения - `int`.

Шаблоны не обязательны.
Рекурсия разрешена.

* *Формат ввода*: По очереди вводятся узлы бинарного дерева в порядке вставки.

* *Формат вывода*: Cамое короткое расстояние от корня до листа.
*/

#include <algorithm>
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

int GetMinDepth(const Node* node) {
  if (!node) {
    return 0;
  }

  if (!node->left && !node->right) {
    return 1;
  }

  if (!node->left) {
    return GetMinDepth(node->right) + 1;
  }

  if (!node->right) {
    return GetMinDepth(node->left) + 1;
  }

  return std::min(GetMinDepth(node->left), GetMinDepth(node->right)) + 1;
}

int main() {
  Node* root = nullptr;
  int val;

  while (std::cin >> val) {
    Add(root, val);
  }

  std::cout << GetMinDepth(root) << std::endl;

  DestroyTree(root);

  return 0;
}
