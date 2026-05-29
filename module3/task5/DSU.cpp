#include "DSU.h"

DSU::DSU(int n) : parent_(n), rank_(n, 0) {
  for (int i = 0; i < n; ++i) {
    parent_[i] = i;
  }
}

int DSU::Find(int i) {
  if (parent_[i] == i) {
    return i;
  }
  return parent_[i] = Find(parent_[i]);
}

void DSU::Unite(int i, int j) {
  int root_i = Find(i);
  int root_j = Find(j);
  if (root_i != root_j) {
    if (rank_[root_i] < rank_[root_j]) {
      parent_[root_i] = root_j;
    } else if (rank_[root_i] > rank_[root_j]) {
      parent_[root_j] = root_i;
    } else {
      parent_[root_j] = root_i;
      rank_[root_i]++;
    }
  }
}
