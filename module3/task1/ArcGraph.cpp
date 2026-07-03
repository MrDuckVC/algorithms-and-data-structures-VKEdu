#include "ArcGraph.h"

#include <vector>

ArcGraph::ArcGraph(int vertices_count) : vertices_count_(vertices_count) {}

ArcGraph::ArcGraph(const IGraph& other) : vertices_count_(other.VerticesCount()) {
  for (int i = 0; i < vertices_count_; ++i) {
    std::vector<int> next_vertices = other.GetNextVertices(i);
    for (int to : next_vertices) {
      edges_.push_back({i, to});
    }
  }
}

void ArcGraph::AddEdge(int from, int to) { edges_.push_back({from, to}); }

int ArcGraph::VerticesCount() const { return vertices_count_; }

std::vector<int> ArcGraph::GetNextVertices(int vertex) const {
  std::vector<int> next_vertices;
  for (const auto& edge : edges_) {
    if (edge.first == vertex) {
      next_vertices.push_back(edge.second);
    }
  }
  return next_vertices;
}

std::vector<int> ArcGraph::GetPrevVertices(int vertex) const {
  std::vector<int> prev_vertices;
  for (const auto& edge : edges_) {
    if (edge.second == vertex) {
      prev_vertices.push_back(edge.first);
    }
  }
  return prev_vertices;
}
