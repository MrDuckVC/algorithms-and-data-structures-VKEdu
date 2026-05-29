#include "MatrixGraph.h"

MatrixGraph::MatrixGraph(int vertices_count) : matrix_(vertices_count, std::vector<bool>(vertices_count, false)) {}

MatrixGraph::MatrixGraph(const IGraph& other) : matrix_(other.VerticesCount(), std::vector<bool>(other.VerticesCount(), false)) {
  for (int i = 0; i < other.VerticesCount(); ++i) {
    std::vector<int> next_vertices = other.GetNextVertices(i);
    for (int to : next_vertices) {
      matrix_[i][to] = true;
    }
  }
}

void MatrixGraph::AddEdge(int from, int to) { matrix_[from][to] = true; }

int MatrixGraph::VerticesCount() const { return matrix_.size(); }

std::vector<int> MatrixGraph::GetNextVertices(int vertex) const {
  std::vector<int> next_vertices;
  for (size_t i = 0; i < matrix_.size(); ++i) {
    if (matrix_[vertex][i]) {
      next_vertices.push_back(i);
    }
  }
  return next_vertices;
}

std::vector<int> MatrixGraph::GetPrevVertices(int vertex) const {
  std::vector<int> prev_vertices;
  for (size_t i = 0; i < matrix_.size(); ++i) {
    if (matrix_[i][vertex]) {
      prev_vertices.push_back(i);
    }
  }
  return prev_vertices;
}
