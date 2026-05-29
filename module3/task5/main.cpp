/*
# Модуль №3

**Общие требования для всех задач**:

* Ввод/вывод отделены от решения.
* Не должно быть утечек памяти.

## Задача № 5. Приближенное решение метрической неориентированной задачи коммивояжера (6 баллов)

Найдите приближенное решение метрической неориентированной задачи коммивояжера в полном графе (на плоскости) с помощью минимального остовного дерева.

Оцените качество приближения на случайном наборе точек, нормально распределенном на плоскости с дисперсией $1$. Нормально распределенный набор точек получайте с
помощью преобразования Бокса-Мюллера.

При фиксированном `N`, количестве вершин графа, несколько раз запустите оценку качества приближения. Вычислите среднее значение и среднеквадратичное отклонение
качества приближения для данного `N`. Запустите данный эксперимент для всех `N` в некотором диапазоне, например, `[2, 10]`.

Автоматизируйте запуск экспериментов.

В решении требуется разумно разделить код на файлы. Каждому классу - свой заголовочный файл и файл с реализацией.

В контесте протестируйте работу алгоритма построения минимального остовного дерева. (Варианты в контесте - не те, который описаны здесь. Правильные варианты -
здесь.)

### Задача № 5. Вариант № 1

Для построения минимального остовного дерева используйте алгоритм Крускала.
*/

#include <algorithm>
#include <iostream>
#include <vector>

struct Edge {
  int u, v;
  int weight;

  bool operator<(const Edge& other) const { return weight < other.weight; }
};

class DSU {
 public:
  explicit DSU(int n) : parent_(n + 1), rank_(n + 1, 0) {
    for (int i = 0; i <= n; ++i) {
      parent_[i] = i;
    }
  }

  int Find(int i) {
    if (parent_[i] == i) {
      return i;
    }
    return parent_[i] = Find(parent_[i]);
  }

  bool Unite(int i, int j) {
    int root_i = Find(i);
    int root_j = Find(j);

    if (root_i != root_j) {
      if (rank_[root_i] < rank_[root_j]) {
        parent_[root_i] = root_j;
      } else if (rank_[root_i] > rank_[root_j]) {
        parent_[root_j] = root_i;
      } else {
        parent_[root_j] = root_i;
        rank_[root_i]++;
      }
      return true;
    }
    return false;
  }

 private:
  std::vector<int> parent_;
  std::vector<int> rank_;
};

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(NULL);

  int n, m;
  if (!(std::cin >> n >> m)) return 0;

  std::vector<Edge> edges(m);
  for (int i = 0; i < m; ++i) {
    std::cin >> edges[i].u >> edges[i].v >> edges[i].weight;
  }

  std::sort(edges.begin(), edges.end());

  DSU dsu(n);
  long long mst_weight = 0;
  int edges_added = 0;

  for (const Edge& edge : edges) {
    if (dsu.Unite(edge.u, edge.v)) {
      mst_weight += edge.weight;
      edges_added++;

      if (edges_added == n - 1) {
        break;
      }
    }
  }

  std::cout << mst_weight << std::endl;

  return 0;
}
