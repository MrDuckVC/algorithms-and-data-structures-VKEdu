#pragma once
#include <vector>

#include "IGraph.h"

class ListGraph : public IGraph {
 public:
  explicit ListGraph(int vertices_count);
  explicit ListGraph(const IGraph& other);

  void AddEdge(int from, int to) override;
  int VerticesCount() const override;
  std::vector<int> GetNextVertices(int vertex) const override;
  std::vector<int> GetPrevVertices(int vertex) const override;

 private:
  std::vector<std::vector<int>> adjacency_list_;
  std::vector<std::vector<int>> prev_adjacency_list_;
};
