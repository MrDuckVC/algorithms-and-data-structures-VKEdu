#pragma once
#include <vector>

class DSU {
 public:
  explicit DSU(int n);
  int Find(int i);
  void Unite(int i, int j);

 private:
  std::vector<int> parent_;
  std::vector<int> rank_;
};
