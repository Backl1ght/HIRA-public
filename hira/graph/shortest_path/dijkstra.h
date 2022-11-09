#ifndef HIRA_GRAPH_SHORTEST_PATH_DIJKSTRA_H
#define HIRA_GRAPH_SHORTEST_PATH_DIJKSTRA_H

#include "hira/common_header.h"

namespace hira {

namespace graph {

template <typename DistanceType,
          typename Comp = std::greater<>,
          typename Edge = std::pair<DistanceType, i32>,
          typename Node = std::pair<DistanceType, i32>>
std::vector<DistanceType> Dijkstra(const std::vector<std::vector<Edge>>& g,
                                   const std::vector<i32>& s) {
  const DistanceType INF = std::numeric_limits<DistanceType>::max();
  const i32 n = g.size();
  const Comp comp;

  std::vector<DistanceType> dis(n, INF);
  std::vector<bool> vis(n, false);

  std::priority_queue<Node, std::vector<Node>, Comp> q;
  for (i32 u : s) {
    dis[u] = 0;
    q.push(Node(dis[u], u));
  }
  while (!q.empty()) {
    auto [c, u] = q.top();
    q.pop();

    if (vis[u])
      continue;
    vis[u] = true;

    for (auto [w, v] : g[u]) {
      if (comp(dis[v], c + w)) {
        dis[v] = c + w;
        q.push(Node(dis[v], v));
      }
    }
  }

  return dis;
}

}  // namespace graph

}  // namespace hira

#endif  // HIRA_GRAPH_SHORTEST_PATH_DIJKSTRA_H