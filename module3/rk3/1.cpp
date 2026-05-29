/*
**Условие**:

Посчитать кол-во компонент связности в неориентированном графе. Граф должен быть реализован в виде класса.

* *Формат ввода*:

    Первая строка содержит число `N` – количество вершин.

    Вторая строка содержит число `M` - количество ребер.

    Каждая следующая строка содержит ребро (откуда, куда).

* *Формат вывода*: Целое число - кол-во компонент связности в графе.

**Пример 1**:

| **Ввод**                     | **Вывод** |
| :--------------------------- | :-------- |
| `3`<br>`2`<br>`0 1`<br>`0 2` | `1`       |

**Пример 2**:

| **Ввод**                     | **Вывод** |
| :--------------------------- | :-------- |
| `4`<br>`2`<br>`0 1`<br>`0 2` | `2`       |
*/

#include <iostream>
#include <queue>
#include <vector>

class ListGraph {
 public:
  explicit ListGraph(int vertices_count) : adjacency_list_(vertices_count) {}

  void AddEdge(int from, int to) {
    adjacency_list_[from].push_back(to);
    adjacency_list_[to].push_back(from);
  }

  int VerticesCount() const { return adjacency_list_.size(); }

  const std::vector<int>& GetNextVertices(int vertex) const { return adjacency_list_[vertex]; }

 private:
  std::vector<std::vector<int>> adjacency_list_;
};

int CountConnectedComponents(const ListGraph& graph) {
  int n = graph.VerticesCount();
  std::vector<bool> visited(n, false);
  int components_count = 0;

  for (int i = 0; i < n; ++i) {
    if (!visited[i]) {
      components_count++;

      std::queue<int> q;
      q.push(i);
      visited[i] = true;

      while (!q.empty()) {
        int curr = q.front();
        q.pop();

        for (int next_v : graph.GetNextVertices(curr)) {
          if (!visited[next_v]) {
            visited[next_v] = true;
            q.push(next_v);
          }
        }
      }
    }
  }

  return components_count;
}

int main() {
  int n = 0;
  int m = 0;

  if (!(std::cin >> n >> m)) return 0;

  ListGraph graph(n);

  for (int i = 0; i < m; ++i) {
    int u, v;
    std::cin >> u >> v;
    graph.AddEdge(u, v);
  }

  std::cout << CountConnectedComponents(graph) << std::endl;

  return 0;
}
