#ifndef HIRA_GRAPH_SHORTEST_PATH_SPFA_H
#define HIRA_GRAPH_SHORTEST_PATH_SPFA_H

#include "hira/common_header.h"

namespace hira {

namespace graph {

template <typename DistanceType,
          typename Comp = std::greater<>,
          typename Edge = std::pair<DistanceType, i32>>
std::vector<DistanceType> SPFA(const std::vector<std::vector<Edge>>& g,
                               const std::vector<i32>& s) {
  const DistanceType INF = std::numeric_limits<DistanceType>::max();
  const i32 n = g.size();
  const Comp comp;

  std::vector<DistanceType> dis(n, INF);
  std::vector<bool> inq(n, false);

  std::queue<i32> q;
  for (i32 u : s) {
    dis[u] = 0;
    inq[u] = true;
    q.push(u);
  }
  while (!q.empty()) {
    i32 u = q.front();
    q.pop();
    inq[u] = false;

    for (auto [w, v] : g[u]) {
      if (comp(dis[v], dis[u] + w)) {
        dis[v] = dis[u] + w;
        if (inq[v] == false) {
          inq[v] = true;
          q.push(v);
        }
      }
    }
  }

  return dis;
}

}  // namespace graph

}  // namespace hira

#endif  // HIRA_GRAPH_SHORTEST_PATH_SPFA_H