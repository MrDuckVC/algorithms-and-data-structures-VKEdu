#include "Graph.h"

#include <algorithm>

#include "DSU.h"

bool Edge::operator<(const Edge& other) const { return weight < other.weight; }

Graph::Graph(const std::vector<Point>& points) : n_(points.size()), points_(points) {
  for (int i = 0; i < n_; ++i) {
    for (int j = i + 1; j < n_; ++j) {
      edges_.push_back({i, j, GetDistance(points[i], points[j])});
    }
  }
}

std::vector<std::vector<int>> Graph::GetMST_Kruskal() const {
  std::vector<std::vector<int>> mst_adj(n_);
  std::vector<Edge> sorted_edges = edges_;

  std::sort(sorted_edges.begin(), sorted_edges.end());

  DSU dsu(n_);
  int edges_added = 0;

  for (const Edge& edge : sorted_edges) {
    if (dsu.Find(edge.u) != dsu.Find(edge.v)) {
      dsu.Unite(edge.u, edge.v);
      mst_adj[edge.u].push_back(edge.v);
      mst_adj[edge.v].push_back(edge.u);
      edges_added++;
      if (edges_added == n_ - 1) break;
    }
  }
  return mst_adj;
}

void Graph::DFS(int u, const std::vector<std::vector<int>>& adj, std::vector<bool>& visited, std::vector<int>& path) const {
  visited[u] = true;
  path.push_back(u);
  for (int v : adj[u]) {
    if (!visited[v]) {
      DFS(v, adj, visited, path);
    }
  }
}

double Graph::GetApproxTSP() const {
  std::vector<std::vector<int>> mst_adj = GetMST_Kruskal();
  std::vector<bool> visited(n_, false);
  std::vector<int> path;

  DFS(0, mst_adj, visited, path);

  double total_length = 0;
  for (size_t i = 0; i < path.size(); ++i) {
    int u = path[i];
    int v = path[(i + 1) % n_];
    total_length += GetDistance(points_[u], points_[v]);
  }
  return total_length;
}
