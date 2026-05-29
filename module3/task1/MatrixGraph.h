#pragma once
#include "IGraph.h"

class MatrixGraph : public IGraph {
 public:
  explicit MatrixGraph(int vertices_count);
  explicit MatrixGraph(const IGraph& other);

  void AddEdge(int from, int to) override;
  int VerticesCount() const override;
  std::vector<int> GetNextVertices(int vertex) const override;
  std::vector<int> GetPrevVertices(int vertex) const override;

 private:
  std::vector<std::vector<bool>> matrix_;
};
