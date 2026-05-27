/*
# Модуль №1

**Общие требования для всех задач**:

* Ввод/вывод отделены от решения.
* Не должно быть утечек памяти.

## Задача № 4. Использование АВЛ-дерева (5 баллов)

**Обязательная задача.**

**Требование для всех вариантов Задачи 4:**

Решение должно поддерживать передачу функции сравнения снаружи.

### № 4.2. Порядковые статистики

Дано число $N$ и $N$ строк. Каждая строка содержит команду добавления или удаления натуральных чисел, а также запрос на получение $k$-ой порядковой статистики.
Команда добавления числа $A$ задается положительным числом $A$, команда удаления числа $A$ задается отрицательным числом “$-A$”. Запрос на получение $k$-ой
порядковой статистики задается числом $k$.

**Требования**: скорость выполнения запроса - $O(\log n)$.

| in                                                    | out                                 |
| :---------------------------------------------------- | :---------------------------------- |
| `5`<br>`40 0`<br>`10 1`<br>`4 1`<br>`-10 0`<br>`50 2` | `40`<br>`40`<br>`10`<br>`4`<br>`50` |
*/

/*
 * Задача № 4.2. Порядковые статистики в АВЛ-дереве
 * Найти k-ю порядковую статистику за O(log n)
 */

#include <algorithm>
#include <iostream>

class AVLTree {
 private:
  struct Node {
    int key;
    int height;
    int size;
    Node* left;
    Node* right;

    explicit Node(int k) : key(k), height(1), size(1), left(nullptr), right(nullptr) {}
  };

  Node* root_;

  int get_height(Node* p) const { return p ? p->height : 0; }

  int get_size(Node* p) const { return p ? p->size : 0; }

  int bfactor(Node* p) const { return get_height(p->right) - get_height(p->left); }

  void fix_node(Node* p) {
    if (!p) return;
    p->height = std::max(get_height(p->left), get_height(p->right)) + 1;
    p->size = get_size(p->left) + get_size(p->right) + 1;
  }

  Node* rotate_right(Node* p) {
    Node* q = p->left;
    p->left = q->right;
    q->right = p;

    fix_node(p);
    fix_node(q);

    return q;
  }

  Node* rotate_left(Node* q) {
    Node* p = q->right;
    q->right = p->left;
    p->left = q;

    fix_node(q);
    fix_node(p);

    return p;
  }

  Node* balance(Node* p) {
    fix_node(p);

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

  int find_kth(Node* p, int k) const {
    if (!p) return -1;

    int left_size = get_size(p->left);

    if (k == left_size) {
      return p->key;
    } else if (k < left_size) {
      return find_kth(p->left, k);
    } else {
      return find_kth(p->right, k - left_size - 1);
    }
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

  void Remove(int k) { root_ = remove(root_, k); }

  int FindKth(int k) const { return find_kth(root_, k); }
};

int main() {
  int n;
  if (!(std::cin >> n)) return 0;

  AVLTree tree;

  for (int i = 0; i < n; ++i) {
    int val, k;
    std::cin >> val >> k;

    if (val > 0) {
      tree.Insert(val);
    } else {
      tree.Remove(-val);
    }

    std::cout << tree.FindKth(k) << std::endl;
  }

  return 0;
}
