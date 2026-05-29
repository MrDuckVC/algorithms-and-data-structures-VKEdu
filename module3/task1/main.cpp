/*
# Модуль №3

**Общие требования для всех задач**:

* Ввод/вывод отделены от решения.
* Не должно быть утечек памяти.

## Задача № 1. «Представление графа» (5 баллов)

**Обязательная задача.**

Дан базовый интерфейс для представления ориентированного графа:

```cpp
struct IGraph {
    virtual ~IGraph() {}

    // Добавление ребра от from к to.
    virtual void AddEdge(int from, int to) = 0;

    virtual int VerticesCount() const  = 0;

    virtual std::vector<int> GetNextVertices(int vertex) const = 0;
    virtual std::vector<int> GetPrevVertices(int vertex) const = 0;
};
```

Необходимо написать несколько реализаций интерфейса:

* `ListGraph`, хранящий граф в виде массива списков смежности,
* `MatrixGraph`, хранящий граф в виде матрицы смежности,
* `SetGraph`, хранящий граф в виде массива хэш-таблиц/сбалансированных деревьев поиска,
* `ArcGraph`, хранящий граф в виде одного массива пар `{from, to}`.

Также необходимо реализовать конструктор, принимающий `const IGraph&`. Такой конструктор должен скопировать переданный граф в создаваемый объект.

Для каждого класса создавайте отдельные `h` и `cpp` файлы.

Число вершин графа задается в конструкторе каждой реализации.
*/

#include <cassert>
#include <functional>
#include <iostream>
#include <queue>
#include <vector>

#include "ArcGraph.h"
#include "ListGraph.h"
#include "MatrixGraph.h"
#include "SetGraph.h"

void BFS(const IGraph &graph, int vertex, std::vector<bool> &visited, const std::function<void(int)> &func) {
  std::queue<int> qu;
  qu.push(vertex);
  visited[vertex] = true;

  while (!qu.empty()) {
    int currentVertex = qu.front();
    qu.pop();

    func(currentVertex);

    for (int nextVertex : graph.GetNextVertices(currentVertex)) {
      if (!visited[nextVertex]) {
        visited[nextVertex] = true;
        qu.push(nextVertex);
      }
    }
  }
}

void mainBFS(const IGraph &graph, const std::function<void(int)> &func) {
  std::vector<bool> visited(graph.VerticesCount(), false);
  for (int i = 0; i < graph.VerticesCount(); ++i) {
    if (!visited[i]) {
      BFS(graph, i, visited, func);
    }
  }
}

void DFS(const IGraph &graph, int vertex, std::vector<bool> &visited, const std::function<void(int)> &func) {
  visited[vertex] = true;
  func(vertex);

  for (int nextVertex : graph.GetNextVertices(vertex)) {
    if (!visited[nextVertex]) {
      DFS(graph, nextVertex, visited, func);
    }
  }
}

void mainDFS(const IGraph &graph, const std::function<void(int)> &func) {
  std::vector<bool> visited(graph.VerticesCount(), false);
  for (int i = 0; i < graph.VerticesCount(); ++i) {
    if (!visited[i]) {
      DFS(graph, i, visited, func);
    }
  }
}

int main() {
  auto print_vertex = [](int vertex) { std::cout << vertex << " "; };

  std::cout << "Создаем базовый ListGraph" << std::endl;
  ListGraph listGraph(7);
  listGraph.AddEdge(0, 1);
  listGraph.AddEdge(0, 5);
  listGraph.AddEdge(1, 2);
  listGraph.AddEdge(1, 3);
  listGraph.AddEdge(1, 5);
  listGraph.AddEdge(1, 6);
  listGraph.AddEdge(3, 2);
  listGraph.AddEdge(3, 4);
  listGraph.AddEdge(3, 6);
  listGraph.AddEdge(5, 4);
  listGraph.AddEdge(5, 6);
  listGraph.AddEdge(6, 4);

  std::cout << "ListGraph BFS: ";
  mainBFS(listGraph, print_vertex);
  std::cout << std::endl;
  std::cout << "ListGraph DFS: ";
  mainDFS(listGraph, print_vertex);
  std::cout << std::endl << std::endl;

  std::cout << "Копируем ListGraph -> ArcGraph" << std::endl;
  ArcGraph arcGraph(listGraph);
  assert(arcGraph.VerticesCount() == listGraph.VerticesCount());
  std::cout << "ArcGraph BFS: ";
  mainBFS(arcGraph, print_vertex);
  std::cout << std::endl;
  std::cout << "ArcGraph DFS: ";
  mainDFS(arcGraph, print_vertex);
  std::cout << std::endl << std::endl;

  std::cout << "Копируем ArcGraph -> MatrixGraph" << std::endl;
  MatrixGraph matrixGraph(arcGraph);
  assert(matrixGraph.VerticesCount() == arcGraph.VerticesCount());
  std::cout << "MatrixGraph BFS: ";
  mainBFS(matrixGraph, print_vertex);
  std::cout << std::endl;
  std::cout << "MatrixGraph DFS: ";
  mainDFS(matrixGraph, print_vertex);
  std::cout << std::endl << std::endl;

  std::cout << "Копируем MatrixGraph -> SetGraph" << std::endl;
  SetGraph setGraph(matrixGraph);
  assert(setGraph.VerticesCount() == matrixGraph.VerticesCount());
  std::cout << "SetGraph BFS: ";
  mainBFS(setGraph, print_vertex);
  std::cout << std::endl;
  std::cout << "SetGraph DFS: ";
  mainDFS(setGraph, print_vertex);
  std::cout << std::endl << std::endl;

  std::cout << "Все графы инициализированы друг от друга, вершины скопированы корректно." << std::endl;

  return 0;
}
