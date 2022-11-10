#ifndef HIRA_GRAPH_LCA_HLD_LCA_H
#define HIRA_GRAPH_LCA_HLD_LCA_H

#include "hira/common_header.h"

namespace hira {

namespace graph {

class HldLCA {
 private:
  void dfs1(int u, int fa) {
    parent_[u] = fa;
    size_[u] = 1;
    heavy_son_[u] = -1;

    for (int v : g_[u]) {
      if (v == fa)
        continue;

      depth_[v] = depth_[u] + 1;
      dfs1(v, u);

      size_[u] += size_[v];
      if (heavy_son_[u] == -1 || size_[v] > size_[heavy_son_[u]])
        heavy_son_[u] = v;
    }
  }

  void dfs2(int u, int fa, int top) {
    top_[u] = top;

    if (heavy_son_[u] != -1)
      dfs2(heavy_son_[u], u, top);

    for (int v : g_[u]) {
      if (v == fa || v == heavy_son_[u])
        continue;
      dfs2(v, u, v);
    }
  }

 public:
  HldLCA(const std::vector<std::vector<int>>& g, int root)
      : n_(g.size()),
        g_(g),
        parent_(n_),
        size_(n_),
        heavy_son_(n_),
        depth_(n_),
        top_(n_) {
    depth_[root] = 0;
    dfs1(root, root);
    dfs2(root, root, root);
  }

  int lca(int u, int v) {
    while (top_[u] != top_[v]) {
      if (depth_[top_[u]] < depth_[top_[v]])
        std::swap(u, v);
      u = parent_[top_[u]];
    }
    if (depth_[u] > depth_[v])
      std::swap(u, v);
    return u;
  }

 private:
  int n_;
  const std::vector<std::vector<int>>& g_;
  std::vector<int> parent_;
  std::vector<int> size_;
  std::vector<int> heavy_son_;
  std::vector<int> depth_;
  std::vector<int> top_;
};

}  // namespace graph

}  // namespace hira

#endif  // HIRA_GRAPH_LCA_HLD_LCA_H