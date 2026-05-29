#include "ListGraph.h"

ListGraph::ListGraph(int vertices_count) : adjacency_list_(vertices_count), prev_adjacency_list_(vertices_count) {}

ListGraph::ListGraph(const IGraph& other) : adjacency_list_(other.VerticesCount()), prev_adjacency_list_(other.VerticesCount()) {
  for (int i = 0; i < other.VerticesCount(); ++i) {
    adjacency_list_[i] = other.GetNextVertices(i);
    prev_adjacency_list_[i] = other.GetPrevVertices(i);
  }
}

void ListGraph::AddEdge(int from, int to) {
  adjacency_list_[from].push_back(to);
  prev_adjacency_list_[to].push_back(from);
}

int ListGraph::VerticesCount() const { return adjacency_list_.size(); }

std::vector<int> ListGraph::GetNextVertices(int vertex) const { return adjacency_list_[vertex]; }

std::vector<int> ListGraph::GetPrevVertices(int vertex) const { return prev_adjacency_list_[vertex]; }
