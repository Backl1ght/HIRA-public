#ifndef HIRA_GRAPH_LCA_BINARY_LIFTING_LCA_H
#define HIRA_GRAPH_LCA_BINARY_LIFTING_LCA_H

#include "hira/common_header.h"

namespace hira {

namespace graph {

class BinaryLiftingLCA {
 private:
  void dfs(int u, int fa) {
    if (u == fa) {
      for (int i = 0; i < lg_; ++i)
        parent_[u][i] = u;
    } else {
      parent_[u][0] = fa;
      for (int i = 1; i < lg_; ++i)
        parent_[u][i] = parent_[parent_[u][i - 1]][i - 1];
    }

    for (int v : g_[u]) {
      if (v == fa)
        continue;

      depth_[v] = depth_[u] + 1;
      dfs(v, u);
    }
  }

 public:
  BinaryLiftingLCA(const std::vector<std::vector<int>>& g, int root)
      : n_(g.size()),
        lg_(std::__lg(n_) + 1),
        g_(g),
        parent_(n_, std::vector<int>(lg_)),
        depth_(n_) {
    depth_[root] = 0;
    dfs(root, root);
  }

  int jump(int u, int distance) {
    for (int i = lg_ - 1; i >= 0; --i)
      if (distance >> i & 1)
        u = parent_[u][i];
    return u;
  }

  int lca(int u, int v) {
    if (depth_[u] < depth_[v])
      std::swap(u, v);
    if (depth_[u] != depth_[v])
      u = jump(u, depth_[u] - depth_[v]);

    if (u == v)
      return u;

    for (int i = lg_ - 1; i >= 0; --i) {
      if (parent_[u][i] != parent_[v][i]) {
        u = parent_[u][i];
        v = parent_[v][i];
      }
    }

    return parent_[u][0];
  }

 private:
  int n_;
  int lg_;
  const std::vector<std::vector<int>>& g_;
  std::vector<std::vector<int>> parent_;
  std::vector<int> depth_;
};

}  // namespace graph

}  // namespace hira

#endif  // HIRA_GRAPH_LCA_BINARY_LIFTING_LCA_H
