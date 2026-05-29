#pragma once
#include <vector>

#include "Point.h"

struct Edge {
  int u, v;
  double weight;
  bool operator<(const Edge& other) const;
};

class Graph {
 public:
  explicit Graph(const std::vector<Point>& points);

  std::vector<std::vector<int>> GetMST_Kruskal() const;

  double GetApproxTSP() const;

 private:
  int n_;
  std::vector<Point> points_;
  std::vector<Edge> edges_;

  void DFS(int u, const std::vector<std::vector<int>>& adj, std::vector<bool>& visited, std::vector<int>& path) const;
};
