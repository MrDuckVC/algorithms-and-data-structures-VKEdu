/*
**Условие**:

Нужно проверить, является ли путь в неориентированном графе гамильтоновым. Граф должен быть реализован в виде класса.

* *Формат ввода*:

    Первая строка содержит число `N` – количество вершин.

    Вторая строка содержит число `M` - количество ребер.

    Третья строка содержит число `K` - количество вершин в пути.

    Далее идут `M` строк с ребрами (откуда, куда).

    Далее идут `K` вершин, принадлежащих пути.

* *Формат вывода*:

    `1` - если путь гамильтонов

    `0` - если путь не гамильтонов

**Примечания**:
Путь может не существовать на графе. В этом случае нужно вывести `0`.

**Пример 1**:

| **Ввод**                                                           | **Вывод** |
| :----------------------------------------------------------------- | :-------- |
| `4`<br>`4`<br>`4`<br>`0 1`<br>`1 2`<br>`2 3`<br>`3 0`<br>`0 1 2 3` | `1`       |

**Пример 2**:

| **Ввод**                                                    | **Вывод** |
| :---------------------------------------------------------- | :-------- |
| `4`<br>`3`<br>`5`<br>`0 1`<br>`1 2`<br>`1 3`<br>`0 1 2 1 3` | `0`       |
*/

#include <iostream>
#include <unordered_set>
#include <vector>

class SetGraph {
 public:
  explicit SetGraph(int vertices_count) : adjacency_sets_(vertices_count) {}

  void AddEdge(int from, int to) {
    adjacency_sets_[from].insert(to);
    adjacency_sets_[to].insert(from);
  }

  int VerticesCount() const { return adjacency_sets_.size(); }

  bool HasEdge(int u, int v) const {
    if (u < 0 || u >= VerticesCount() || v < 0 || v >= VerticesCount()) {
      return false;
    }
    return adjacency_sets_[u].find(v) != adjacency_sets_[u].end();
  }

 private:
  std::vector<std::unordered_set<int>> adjacency_sets_;
};

bool IsHamiltonianPath(const SetGraph& graph, const std::vector<int>& path) {
  int n = graph.VerticesCount();

  if (path.size() != static_cast<size_t>(n)) {
    return false;
  }

  std::vector<bool> visited(n, false);

  if (path[0] < 0 || path[0] >= n) return false;
  visited[path[0]] = true;

  for (size_t i = 1; i < path.size(); ++i) {
    int curr = path[i];
    int prev = path[i - 1];

    if (curr < 0 || curr >= n) {
      return false;
    }

    if (visited[curr]) {
      return false;
    }

    if (!graph.HasEdge(prev, curr)) {
      return false;
    }

    visited[curr] = true;
  }

  return true;
}

int main() {
  int n, m, k;
  if (!(std::cin >> n >> m >> k)) return 0;

  SetGraph graph(n);

  for (int i = 0; i < m; ++i) {
    int u, v;
    std::cin >> u >> v;
    graph.AddEdge(u, v);
  }

  std::vector<int> path(k);
  for (int i = 0; i < k; ++i) {
    std::cin >> path[i];
  }

  if (IsHamiltonianPath(graph, path)) {
    std::cout << 1 << std::endl;
  } else {
    std::cout << 0 << std::endl;
  }

  return 0;
}
