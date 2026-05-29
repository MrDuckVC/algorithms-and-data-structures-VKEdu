/*
# Модуль №3

**Общие требования для всех задач**:

* Ввод/вывод отделены от решения.
* Не должно быть утечек памяти.

## Задача № 2. Количество различных путей (3 балла)

**Обязательная задача.**

Дан невзвешенный неориентированный граф. В графе может быть несколько кратчайших путей между какими-то вершинами. Найдите количество различных кратчайших путей
между заданными вершинами.

**Требования**: сложность $O(V + E)$, граф реализован в виде класса.

* *Формат входных данных.*

  * `v`: кол-во вершин (макс. $50000$),
  * `n`: кол-во ребер (макс. $200000$),
  * `n` пар реберных вершин,
  * пара вершин `u`, `w` для запроса.

* *Формат выходных данных.* Количество кратчайших путей от u к w.

| in                                                                       | out |
| :----------------------------------------------------------------------- | :-- |
| `4`<br>`5`<br>`0 1`<br>`0 2`<br>`1 2`<br>`1 3`<br>`2 3`<br><br>`0 3`<br> | `2` |
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

int CountShortestPaths(const ListGraph& graph, int start, int target) {
  int v_count = graph.VerticesCount();

  std::vector<int> dist(v_count, -1);

  std::vector<int> ways(v_count, 0);

  std::queue<int> q;

  dist[start] = 0;
  ways[start] = 1;
  q.push(start);

  while (!q.empty()) {
    int current = q.front();
    q.pop();

    for (int next_v : graph.GetNextVertices(current)) {
      if (dist[next_v] == -1) {
        dist[next_v] = dist[current] + 1;
        ways[next_v] = ways[current];
        q.push(next_v);
      } else if (dist[next_v] == dist[current] + 1) {
        ways[next_v] += ways[current];
      }
    }
  }

  return ways[target];
}

int main() {
  int v_count = 0;
  int e_count = 0;

  if (!(std::cin >> v_count >> e_count)) return 0;

  ListGraph graph(v_count);

  for (int i = 0; i < e_count; ++i) {
    int from, to;
    std::cin >> from >> to;
    graph.AddEdge(from, to);
  }

  int start, target;
  std::cin >> start >> target;

  std::cout << CountShortestPaths(graph, start, target) << std::endl;

  return 0;
}
