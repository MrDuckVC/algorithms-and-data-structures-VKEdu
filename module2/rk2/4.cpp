/*
### Задачи 4. AVL-дерево

| Ограничения | |
| :--- | :--- |
| **Ограничение времени** | 2 секунды |
| **Ограничение памяти** | 244.14 Мб |
| **Ввод** | стандартный ввод или input.txt |
| **Вывод** | стандартный вывод или output.txt |

**Условие**:

**Реализуйте AVL-дерево. Решения с использованием других структур засчитываться не будут.**

Входной файл содержит описание операций с деревом. Операций не больше $10^5$.

* Шаблоны не обязательны.
* Рекурсия разрешена.

* *Формат ввода*:

  В каждой строке находится одна из следующих операций:
  * `insert x` — добавить в дерево ключ $x$. Если ключ $x$ есть в дереве, то ничего делать не надо.
  * `delete x` — удалить из дерева ключ $x$. Если ключа $x$ в дереве нет, то ничего делать не надо.
  * `exists x` — если ключ $x$ есть в дереве, вывести «`true`», иначе «`false`».
  * `next x` — минимальный элемент в дереве, больший $x$, или «`none`», если такого нет.
  * `prev x` — максимальный элемент в дереве, меньший $x$, или «`none`», если такого нет.

    Все числа во входном файле целые и по модулю не превышают $10^9$.

* *Формат вывода*:

    Выведите последовательно результат выполнения всех операций `exists`, `next`, `prev`. Следуйте формату выходного файла из примера.
*/
#include <algorithm>
#include <iostream>
#include <string>

class AVLTree {
 private:
  struct Node {
    int key;
    int height;
    Node* left;
    Node* right;

    explicit Node(int k) : key(k), height(1), left(nullptr), right(nullptr) {}
  };

  Node* root_;

  int height(Node* p) const { return p ? p->height : 0; }

  int bfactor(Node* p) const { return height(p->right) - height(p->left); }

  void fix_height(Node* p) {
    int hl = height(p->left);
    int hr = height(p->right);
    p->height = std::max(hl, hr) + 1;
  }

  Node* rotate_right(Node* p) {
    Node* q = p->left;
    p->left = q->right;
    q->right = p;
    fix_height(p);
    fix_height(q);
    return q;
  }

  Node* rotate_left(Node* q) {
    Node* p = q->right;
    q->right = p->left;
    p->left = q;
    fix_height(q);
    fix_height(p);
    return p;
  }

  Node* balance(Node* p) {
    fix_height(p);
    if (bfactor(p) == 2) {
      if (bfactor(p->right) < 0) {
        p->right = rotate_right(p->right);
      }
      return rotate_left(p);
    }
    if (bfactor(p) == -2) {
      if (bfactor(p->left) > 0) {
        p->left = rotate_left(p->left);
      }
      return rotate_right(p);
    }
    return p;
  }

  Node* insert(Node* p, int k) {
    if (!p) return new Node(k);

    if (k < p->key) {
      p->left = insert(p->left, k);
    } else if (k > p->key) {
      p->right = insert(p->right, k);
    } else {
      return p;
    }
    return balance(p);
  }

  Node* find_min(Node* p) const { return p->left ? find_min(p->left) : p; }

  Node* remove_min(Node* p) {
    if (!p->left) {
      return p->right;
    }
    p->left = remove_min(p->left);
    return balance(p);
  }

  Node* remove(Node* p, int k) {
    if (!p) return nullptr;

    if (k < p->key) {
      p->left = remove(p->left, k);
    } else if (k > p->key) {
      p->right = remove(p->right, k);
    } else {
      Node* q = p->left;
      Node* r = p->right;
      delete p;

      if (!r) return q;

      Node* min_node = find_min(r);
      min_node->right = remove_min(r);
      min_node->left = q;
      return balance(min_node);
    }
    return balance(p);
  }

  void destroy_tree(Node* p) {
    if (p) {
      destroy_tree(p->left);
      destroy_tree(p->right);
      delete p;
    }
  }

 public:
  AVLTree() : root_(nullptr) {}

  ~AVLTree() { destroy_tree(root_); }

  void Insert(int k) { root_ = insert(root_, k); }

  void Delete(int k) { root_ = remove(root_, k); }

  bool Exists(int k) const {
    Node* curr = root_;
    while (curr) {
      if (k == curr->key) return true;
      if (k < curr->key)
        curr = curr->left;
      else
        curr = curr->right;
    }
    return false;
  }

  void Next(int k) const {
    Node* curr = root_;
    Node* successor = nullptr;

    while (curr) {
      if (curr->key > k) {
        successor = curr;
        curr = curr->left;
      } else {
        curr = curr->right;
      }
    }

    if (successor)
      std::cout << successor->key << std::endl;
    else
      std::cout << "none" << std::endl;
  }

  void Prev(int k) const {
    Node* curr = root_;
    Node* predecessor = nullptr;

    while (curr) {
      if (curr->key < k) {
        predecessor = curr;
        curr = curr->right;
      } else {
        curr = curr->left;
      }
    }

    if (predecessor)
      std::cout << predecessor->key << std::endl;
    else
      std::cout << "none" << std::endl;
  }
};

int main() {
  AVLTree tree;
  std::string operation;
  int x;

  while (std::cin >> operation >> x) {
    if (operation == "insert") {
      tree.Insert(x);
    } else if (operation == "delete") {
      tree.Delete(x);
    } else if (operation == "exists") {
      if (tree.Exists(x))
        std::cout << "true" << std::endl;
      else
        std::cout << "false" << std::endl;
    } else if (operation == "next") {
      tree.Next(x);
    } else if (operation == "prev") {
      tree.Prev(x);
    }
  }

  return 0;
}
