#ifndef HIRA_GRAPH_FLOW_PRIMAL_DUAL_MIN_COST_MAX_FLOW_H
#define HIRA_GRAPH_FLOW_PRIMAL_DUAL_MIN_COST_MAX_FLOW_H

#include "hira/common_header.h"

namespace hira {

namespace graph {

template <typename CapacityType, typename CostType>
class PrimalDualMinCostMaxFlow {
 private:
  struct Edge {
    int from, to;
    CapacityType capacity;
    CostType cost;
    Edge() {}
    Edge(int _from, int _to, CapacityType _capacity, CostType _cost)
        : from(_from), to(_to), capacity(_capacity), cost(_cost) {}
  };
  int n_;
  int m_;
  std::vector<Edge> edges_;
  std::vector<std::vector<int>> adjacent_;

 public:
  PrimalDualMinCostMaxFlow(int n) : n_(n), m_(0), edges_(0), adjacent_(n) {}

  void AddEdge(int u, int v, CapacityType capacity, CostType cost) {
    assert(0 <= u and u < n_);
    assert(0 <= v and v < n_);

    edges_.push_back(Edge(u, v, capacity, cost));
    adjacent_[u].push_back(m_);
    ++m_;

    edges_.push_back(Edge(v, u, 0, -cost));
    adjacent_[v].push_back(m_);
    ++m_;
  }

  std::pair<CostType, CapacityType> PrimalDual(int src, int dst) {
    const static CapacityType INF = std::numeric_limits<CapacityType>::max();

    std::vector<CostType> h(n_);
    std::vector<bool> inq(n_);
    std::function<void()> spfa = [&]() -> void {
      std::fill(h.begin(), h.end(), INF);

      std::queue<int> q;
      q.push(src);
      inq[src] = true;
      h[src] = 0;
      while (!q.empty()) {
        int u = q.front();
        q.pop();
        inq[u] = false;

        for (int edge_id : adjacent_[u]) {
          auto [from, to, capacity, cost] = edges_[edge_id];
          if (capacity <= 0 || h[to] <= h[from] + cost)
            continue;
          h[to] = h[from] + cost;
          if (!inq[to]) {
            q.push(to);
            inq[to] = true;
          }
        }
      }
    };

    std::vector<CostType> d(n_);
    std::vector<bool> vis(n_);
    std::vector<int> prev(n_);
    std::function<bool()> dijkstra = [&]() -> bool {
      std::fill(d.begin(), d.end(), INF);
      std::fill(vis.begin(), vis.end(), false);

      std::priority_queue<std::pair<CostType, int>> q;
      d[src] = 0;
      q.push(std::make_pair(-d[src], src));

      while (!q.empty()) {
        auto [_, u] = q.top();
        q.pop();

        if (vis[u])
          continue;
        vis[u] = true;

        for (int edge_id : adjacent_[u]) {
          auto [from, to, capacity, cost] = edges_[edge_id];
          CostType new_cost = cost + h[from] - h[to];

          if (vis[to] || capacity <= 0 || d[to] <= d[from] + new_cost)
            continue;

          d[to] = d[from] + new_cost;
          prev[to] = edge_id;
          q.push(std::make_pair(-d[to], to));
        }
      }

      return d[dst] != INF;
    };

    spfa();

    CapacityType max_flow = 0;
    CostType min_cost = 0;
    while (dijkstra()) {
      CapacityType augment = INF;
      for (int p = dst; p != src; p = edges_[prev[p] ^ 1].to) {
        augment = std::min(augment, edges_[prev[p]].capacity);
      }

      max_flow += augment;
      for (int p = dst; p != src; p = edges_[prev[p] ^ 1].to) {
        min_cost += edges_[prev[p]].cost * augment;
        edges_[prev[p]].capacity -= augment;
        edges_[prev[p] ^ 1].capacity += augment;
      }

      for (int i = 0; i < n_; ++i)
        h[i] += d[i];
    }

    return {min_cost, max_flow};
  }
};

}  // namespace graph

}  // namespace hira

#endif  // HIRA_GRAPH_FLOW_PRIMAL_DUAL_MIN_COST_MAX_FLOW_H