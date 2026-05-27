/*
# Модуль №1

**Общие требования для всех задач**:

* Ввод/вывод отделены от решения.
* Не должно быть утечек памяти.

## Задача № 3. B-дерево (4 балла)

Постройте B-дерево минимального порядка $t$ и выведите его по слоям.

В качестве ключа используются числа, лежащие в диапазоне $0..2^{32} -1$

**Требования**:

* B-дерево должно быть реализовано в виде шаблонного класса.
* Решение должно поддерживать передачу функции сравнения снаружи.

* *Формат входных данных.*
    Сначала вводится минимальный порядок дерева t.

    Затем вводятся элементы дерева.
* *Формат выходных данных.*
    Программа должна вывести B-дерево по слоям. Каждый слой на новой строке, элементы должны выводится в том порядке, в котором они лежат в узлах.

| stdin                         | stdout                          |
| :---------------------------- | :------------------------------ |
| `2`<br>`0 1 2 3 4 5 6 7 8 9`  | `3`<br>`1 5 7`<br>`0 2 4 6 8 9` |
| `4`<br>`0 1 2 3 4 5 6 7 8 9`  | `3`<br>`0 1 2 4 5 6 7 8 9`      |
*/

#include <iostream>
#include <vector>
#include <queue>
#include <functional>
#include <cstdint>

template <typename T, typename Compare = std::less<T>>
class BTree {
private:
    struct Node {
        bool leaf;
        std::vector<T> keys;
        std::vector<Node*> children;

        explicit Node(bool is_leaf, size_t t) : leaf(is_leaf) {
            keys.reserve(2 * t - 1);
            children.reserve(2 * t);
        }
    };

    Node* root_;
    size_t t_;
    Compare cmp_;

    void SplitChild(Node* parent, int i) {
        Node* full_child = parent->children[i];
        Node* new_child = new Node(full_child->leaf, t_);

        for (size_t j = 0; j < t_ - 1; ++j) {
            new_child->keys.push_back(full_child->keys[j + t_]);
        }

        if (!full_child->leaf) {
            for (size_t j = 0; j < t_; ++j) {
                new_child->children.push_back(full_child->children[j + t_]);
            }
        }

        parent->children.insert(parent->children.begin() + i + 1, new_child);

        parent->keys.insert(parent->keys.begin() + i, full_child->keys[t_ - 1]);

        full_child->keys.resize(t_ - 1);
        if (!full_child->leaf) {
            full_child->children.resize(t_);
        }
    }

public:
    explicit BTree(size_t t, const Compare& cmp = Compare())
        : root_(nullptr), t_(t), cmp_(cmp) {}

    ~BTree() {
        if (!root_) return;
        std::queue<Node*> q;
        q.push(root_);
        while (!q.empty()) {
            Node* curr = q.front();
            q.pop();
            for (Node* child : curr->children) {
                q.push(child);
            }
            delete curr;
        }
    }

    BTree(const BTree&) = delete;
    BTree& operator=(const BTree&) = delete;

    void Add(const T& k) {
        if (!root_) {
            root_ = new Node(true, t_);
            root_->keys.push_back(k);
            return;
        }

        if (root_->keys.size() == 2 * t_ - 1) {
            Node* new_root = new Node(false, t_);
            new_root->children.push_back(root_);
            root_ = new_root;
            SplitChild(new_root, 0);
        }

        Node* curr = root_;
        while (true) {
            int i = curr->keys.size() - 1;

            while (i >= 0 && cmp_(k, curr->keys[i])) {
                i--;
            }
            i++;

            if (curr->leaf) {
                curr->keys.insert(curr->keys.begin() + i, k);
                break;
            } else {
                if (curr->children[i]->keys.size() == 2 * t_ - 1) {
                    SplitChild(curr, i);

                    if (!cmp_(k, curr->keys[i])) {
                        i++;
                    }
                }
                curr = curr->children[i];
            }
        }
    }

    void PrintLevelOrder() const {
        if (!root_) return;
        std::queue<Node*> q;
        q.push(root_);

        while (!q.empty()) {
            int level_size = q.size();
            bool first_in_level = true;

            for (int i = 0; i < level_size; ++i) {
                Node* curr = q.front();
                q.pop();

                for (const T& key : curr->keys) {
                    if (!first_in_level) {
                        std::cout << " ";
                    }
                    std::cout << key;
                    first_in_level = false;
                }

                for (Node* child : curr->children) {
                    q.push(child);
                }
            }
            std::cout << std::endl;
        }
    }
};

int main() {
    size_t t;
    if (!(std::cin >> t)) return 0;

    BTree<uint32_t> btree(t);

    uint32_t k;
    while (std::cin >> k) {
        btree.Add(k);
    }

    btree.PrintLevelOrder();

    return 0;
}
