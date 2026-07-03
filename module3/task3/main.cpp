/*
# Модуль №3

**Общие требования для всех задач**:

* Ввод/вывод отделены от решения.
* Не должно быть утечек памяти.

## Задача № 3. «Города» (4 балла)

**Обязательная задача.**

Требуется отыскать самый выгодный маршрут между городами.

**Требования**: время работы $O((N + M) \log N)$, где $N$ - количество городов, $M$ - известных дорог между ними. Граф должен быть реализован в виде класса.

* *Формат входных данных.*

    Первая строка содержит число `N` – количество городов.

    Вторая строка содержит число `M` - количество дорог.

    Каждая следующая строка содержит описание дороги (откуда, куда, время в пути).

    Последняя строка содержит маршрут (откуда и куда нужно доехать).

* *Формат выходных данных.*

    Вывести длину самого выгодного маршрута.

| in                                                                                                                         | out |
| :------------------------------------------------------------------------------------------------------------------------- | :-- |
| `6`<br>`9`<br>`0 3 1`<br>`0 4 2`<br>`1 2 7`<br>`1 3 2`<br>`1 4 3`<br>`1 5 3`<br>`2 5 3`<br>`3 4 4`<br>`3 5 6`<br>`0 2`<br> | `9` |
*/

#include <functional>
#include <iostream>
#include <limits>
#include <queue>
#include <utility>
#include <vector>

struct Edge {
  int to;
  int weight;

  Edge(int to, int weight) : to(to), weight(weight) {}
};

class ListGraph {
 public:
  explicit ListGraph(int vertices_count) : adjacency_list_(vertices_count) {}

  void AddEdge(int from, int to, int weight) {
    adjacency_list_[from].push_back(Edge(to, weight));
    adjacency_list_[to].push_back(Edge(from, weight));
  }

  int VerticesCount() const { return adjacency_list_.size(); }

  const std::vector<Edge>& GetNextEdges(int vertex) const { return adjacency_list_[vertex]; }

 private:
  std::vector<std::vector<Edge>> adjacency_list_;
};

int Dijkstra(const ListGraph& graph, int start, int target) {
  int n = graph.VerticesCount();
  const int INF = std::numeric_limits<int>::max();
  std::vector<int> dist(n, INF);

  using Element = std::pair<int, int>;
  std::priority_queue<Element, std::vector<Element>, std::greater<Element>> pq;

  dist[start] = 0;
  pq.push({0, start});

  while (!pq.empty()) {
    int current_dist = pq.top().first;
    int current_v = pq.top().second;
    pq.pop();

    if (current_dist > dist[current_v]) {
      continue;
    }

    if (current_v == target) {
      return dist[target];
    }

    for (const Edge& edge : graph.GetNextEdges(current_v)) {
      int next_v = edge.to;
      int weight = edge.weight;

      if (dist[current_v] + weight < dist[next_v]) {
        dist[next_v] = dist[current_v] + weight;
        pq.push({dist[next_v], next_v});
      }
    }
  }

  return dist[target] == INF ? -1 : dist[target];
}

int main() {
  int n, m;
  if (!(std::cin >> n >> m)) return 0;

  ListGraph graph(n);

  for (int i = 0; i < m; ++i) {
    int from, to, weight;
    std::cin >> from >> to >> weight;
    graph.AddEdge(from, to, weight);
  }

  int start, target;
  std::cin >> start >> target;

  std::cout << Dijkstra(graph, start, target) << std::endl;

  return 0;
}
