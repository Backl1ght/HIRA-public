# HIRA - wish you HIgh RAting

HIRA is an algorithm library designed for online competitive programming contest.

Most competitive programming contest only support submitting single solution file, this means you should submit one file that contains all code. Writing all code in one file may be annoying sometimes.

But we can take adcantage of **preprocessing** phase of compiler. That is, write solution taking advantage of libraries, preprocess the solution and then get a single source file for submission.

## Example

To use `scripts/make_submission.sh` and `scripts/make_debug.sh`, you need to change the value of `HIRA_ROOT` in the first line of these scripts frist.

Below is how to use HIRA to solve [P3376 【模板】网络最大流](https://www.luogu.com.cn/problem/P3376). This problem will give you a directed graph with $n$ vertices and $m$ edges, and ask you to output the maximum flow from vertex $s$ to vertex $t$.

Write a solution file `a.cpp`:
```cpp
// #define MULTIPLE_TASK
#include "hira/main.cpp"

#include "hira/graph/flow/dinic_max_flow.h"

void Initialize() {}

void SolveCase(int Case) {
  i32 n, m, s, t;
  std::cin >> n >> m >> s >> t;
  --s, --t;

  graph::DinicMaxFlow<i64> max_flow(n);
  for (i32 i = 0; i < m; ++i) {
    i32 u, v;
    i64 c;
    std::cin >> u >> v >> c;
    --u, --v;
    max_flow.AddEdge(u, v, c);
  }

  std::cout << max_flow.Dinic(s, t) << "\n";
}
```

Use `scripts/make_submission.sh` to generate an executable file `a`, and a solution file `a_submission.cpp`.

```bash
/path/to/hira/scripts/make_submission.sh a.cpp
```

Validate the correctness of your solution using executable `a`.

If the solution is validated to be correct, submit `a_submission.cpp` to the online judge.

`a_submission.cpp` may look like this:
```cpp
#include <bits/stdc++.h>
# 0 "/home/backlight/Workspace/Untitled/P_3376_模板_网络最大流.cpp"
# 0 "<built-in>"
# 0 "<command-line>"
# 1 "/usr/include/stdc-predef.h" 1 3 4
# 0 "<command-line>" 2
# 1 "/home/backlight/Workspace/Untitled/P_3376_模板_网络最大流.cpp"

# 1 "/home/backlight/Workspace/HIRA/hira/main.cpp" 1
# 1 "/home/backlight/Workspace/HIRA/hira/common_header.h" 1
# 21 "/home/backlight/Workspace/HIRA/hira/common_header.h"
using i32 = int32_t;
using u32 = uint32_t;
using i64 = int64_t;
using u64 = uint64_t;
# 2 "/home/backlight/Workspace/HIRA/hira/main.cpp" 2

# 1 "/home/backlight/Workspace/HIRA/hira/local/debug.h" 1
# 4 "/home/backlight/Workspace/HIRA/hira/main.cpp" 2
# 1 "/home/backlight/Workspace/HIRA/hira/local/platform.h" 1
# 5 "/home/backlight/Workspace/HIRA/hira/main.cpp" 2

# 1 "/home/backlight/Workspace/HIRA/hira/misc/io.h" 1





namespace hira {
# 23 "/home/backlight/Workspace/HIRA/hira/misc/io.h"
}
# 7 "/home/backlight/Workspace/HIRA/hira/main.cpp" 2


using namespace hira;

void Initialize();
void SolveCase(int Case);

int main(int argc, char* argv[]) {
  std::ios::sync_with_stdio(false); std::cin.tie(0); std::cout.tie(0);;

  Initialize();

  int T = 1;



  for (int t = 1; t <= T; ++t)
    SolveCase(t);
  return 0;
}
# 3 "/home/backlight/Workspace/Untitled/P_3376_模板_网络最大流.cpp" 2

# 1 "/home/backlight/Workspace/HIRA/hira/graph/flow/dinic_max_flow.h" 1





namespace hira {

namespace graph {

template <typename CapacityType>
class DinicMaxFlow {
  struct Edge {
    int from, to;
    CapacityType capacity, flow;
    Edge() {}
    Edge(int _from, int _to, CapacityType _capacity, CapacityType _flow)
        : from(_from), to(_to), capacity(_capacity), flow(_flow) {}
  };

  int n_;
  int m_;
  std::vector<Edge> edges_;
  std::vector<std::vector<int>> adjacent_;

 public:
  explicit DinicMaxFlow(int n) : n_(n), m_(0), edges_(0), adjacent_(n) {}

  void AddEdge(int from, int to, CapacityType capacity) {
    assert(0 <= from and from < n_);
    assert(0 <= to and to < n_);

    edges_.emplace_back(from, to, capacity, 0);
    adjacent_[from].push_back(m_);
    ++m_;

    edges_.emplace_back(to, from, 0, 0);
    adjacent_[to].push_back(m_);
    ++m_;
  }

  CapacityType Dinic(int src, int dst) {
    const static CapacityType INF = std::numeric_limits<CapacityType>::max();
    std::vector<int> level(n_);
    std::vector<int> start_index(n_);

    std::function<bool()> bfs = [&]() -> bool {
      std::fill(level.begin(), level.end(), -1);

      std::queue<int> q;
      q.push(src);
      level[src] = 0;

      while (!q.empty()) {
        int u = q.front();
        q.pop();

        for (int edge_id : adjacent_[u]) {
          auto [from, to, capacity, flow] = edges_[edge_id];
          CapacityType residual_capacity = capacity - flow;
          if (residual_capacity > 0 && level[to] == -1) {
            level[to] = level[u] + 1;
            if (to == dst)
              break;
            q.push(to);
          }
        }
      }

      return level[dst] != -1;
    };

    std::function<CapacityType(int, CapacityType)> dfs =
        [&](int u, CapacityType max_augment) -> CapacityType {
      if (u == dst)
        return max_augment;

      if (max_augment == 0)
        return 0;

      CapacityType total_augment = 0;
      int i = start_index[u];
      for (; i < (int)adjacent_[u].size(); ++i) {
        int edge_id = adjacent_[u][i];
        auto [from, to, capacity, flow] = edges_[edge_id];
        if (level[to] == level[u] + 1) {
          CapacityType residual_capacity = capacity - flow;
          CapacityType new_augment =
              dfs(to, std::min(max_augment, residual_capacity));
          if (new_augment <= 0)
            continue;

          max_augment -= new_augment;
          total_augment += new_augment;
          edges_[edge_id].flow += new_augment;
          edges_[edge_id ^ 1].flow -= new_augment;

          if (max_augment == 0)
            break;
        }
      }
      start_index[u] = i;

      if (total_augment == 0)
        level[u] = -1;

      return total_augment;
    };

    CapacityType max_flow = 0;
    while (bfs()) {
      std::fill(start_index.begin(), start_index.end(), 0);
      CapacityType new_flow = dfs(src, INF);
      ;
      max_flow += new_flow;
    }

    return max_flow;
  }

  std::vector<bool> Cut(int s) {
    std::vector<bool> visited(n_, false);

    std::function<void(int)> dfs = [&](int u) {
      visited[u] = true;
      for (int edge_id : adjacent_[u]) {
        auto [from, to, capacity, flow] = edges_[edge_id];
        if (visited[to] || capacity == flow)
          continue;

        dfs(to);
      }
    };
    dfs(s);

    return visited;
  }
};

}

}
# 5 "/home/backlight/Workspace/Untitled/P_3376_模板_网络最大流.cpp" 2

void Initialize() {}

void SolveCase(int Case) {
  i32 n, m, s, t;
  std::cin >> n >> m >> s >> t;
  --s, --t;

  graph::DinicMaxFlow<i64> max_flow(n);
  for (i32 i = 0; i < m; ++i) {
    i32 u, v;
    i64 c;
    std::cin >> u >> v >> c;
    --u, --v;
    max_flow.AddEdge(u, v, c);
  }

  std::cout << max_flow.Dinic(s, t) << "\n";
}
```

## Another example

[ac-library](https://github.com/atcoder/ac-library) is convenient to use on AtCoder, but not that convenient on other online judges like Codeforces. You can apply the same idea on ac-library and make it easier to use on online judges other than AtCoder. 

## At last

Since the basic idea and implementation of HIRA are kind fo simple, and people's preferences are various, I encourage you to implement your own library and scripts.
