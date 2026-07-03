/*
**Условие**:

Рик и Морти снова бороздят просторы вселенных, но решили ограничиться только теми, номера которых меньше $M$. Они могут телепортироваться из вселенной с номером
$z$ во вселенную $(z+1) \pmod M$ за $a$ бутылок лимонада или во вселенную $(z^2+1) \pmod M$ за $b$ бутылок лимонада. Рик и Морти хотят добраться из вселенной с
номером $x$ во вселенную с номером $y$. Сколько бутылок лимонада отдаст Рик за такое путешествие, если он хочет потратить их как можно меньше? Граф должен быть
реализован в виде класса.

* *Формат ввода*:

    В строке подряд даны: количество бутылок $a$ за первый тип телепортации, количество бутылок $b$ за второй тип телепортации, количество вселенных $M$, номер
стартовой вселенной $x$, номер конечной вселенной $y$ ($0 \le a, b \le 100$, $1 \le M \le 10^6$, $0 \le x, y < M$).

* *Формат вывода*:

    Выведите одно число — количество бутылок лимонада, которые отдаст Рик за такое путешествие.

**Пример 1**:

| **Ввод**      | **Вывод** |
| :------------ | :-------- |
| `3 14 15 9 9` | `0`       |

**Пример 2**:

| **Ввод**    | **Вывод** |
| :---------- | :-------- |
| `6 1 5 2 3` | `6`       |

**Пример 3**:

| **Ввод**    | **Вывод** |
| :---------- | :-------- |
| `6 1 5 2 1` | `2`       |
*/

#include <functional>
#include <iostream>
#include <queue>
#include <utility>
#include <vector>

class UniverseGraph {
 public:
  UniverseGraph(long long M, long long a, long long b) : M_(M), a_(a), b_(b) {}

  long long VerticesCount() const { return M_; }

  std::vector<std::pair<long long, long long>> GetNextVertices(long long z) const {
    std::vector<std::pair<long long, long long>> next_vertices;

    next_vertices.push_back({(z + 1) % M_, a_});

    next_vertices.push_back({((z * z) + 1) % M_, b_});

    return next_vertices;
  }

 private:
  long long M_, a_, b_;
};

long long FindMinLemonade(const UniverseGraph& graph, long long start, long long target) {
  if (start == target) {
    return 0;
  }

  std::vector<long long> dist(graph.VerticesCount(), -1);

  using Element = std::pair<long long, long long>;
  std::priority_queue<Element, std::vector<Element>, std::greater<Element>> pq;

  dist[start] = 0;
  pq.push({0, start});

  while (!pq.empty()) {
    long long current_dist = pq.top().first;
    long long current_v = pq.top().second;
    pq.pop();

    if (current_v == target) {
      return current_dist;
    }

    if (current_dist > dist[current_v]) {
      continue;
    }

    for (const auto& edge : graph.GetNextVertices(current_v)) {
      long long next_v = edge.first;
      long long weight = edge.second;

      if (dist[next_v] == -1 || dist[current_v] + weight < dist[next_v]) {
        dist[next_v] = dist[current_v] + weight;
        pq.push({dist[next_v], next_v});
      }
    }
  }

  return dist[target];
}

int main() {
  long long a, b, M, x, y;
  if (std::cin >> a >> b >> M >> x >> y) {
    UniverseGraph graph(M, a, b);
    std::cout << FindMinLemonade(graph, x, y) << std::endl;
  }

  return 0;
}
