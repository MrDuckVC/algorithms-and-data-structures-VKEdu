#pragma once
#include <utility>

#include "IGraph.h"

class ArcGraph : public IGraph {
 public:
  explicit ArcGraph(int vertices_count);
  explicit ArcGraph(const IGraph& other);

  void AddEdge(int from, int to) override;
  int VerticesCount() const override;
  std::vector<int> GetNextVertices(int vertex) const override;
  std::vector<int> GetPrevVertices(int vertex) const override;

 private:
  int vertices_count_;
  std::vector<std::pair<int, int>> edges_;
};
