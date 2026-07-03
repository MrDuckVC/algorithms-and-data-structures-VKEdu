#pragma once
#include <unordered_set>
#include <vector>

#include "IGraph.h"

class SetGraph : public IGraph {
 public:
  explicit SetGraph(int vertices_count);
  explicit SetGraph(const IGraph& other);

  void AddEdge(int from, int to) override;
  int VerticesCount() const override;
  std::vector<int> GetNextVertices(int vertex) const override;
  std::vector<int> GetPrevVertices(int vertex) const override;

 private:
  std::vector<std::unordered_set<int>> adjacency_sets_;
  std::vector<std::unordered_set<int>> prev_adjacency_sets_;
};
