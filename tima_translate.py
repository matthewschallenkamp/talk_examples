from collections import defaultdict
from heapq import heappush, heappop

def dijkstras(source, target, graph):
  # graph is a ragged array
  # graph[from] = [(cost, to), ...]

  visited = set()
  # todo is a heap of (cost, location) tuples.
  todo = []
  heappush(todo, (0, source))
  while todo:
    cost, loc = heappop(todo)

    if loc in visited:
      continue
    if loc == target:
      return cost

    visited.add(loc)

    for edge in graph[loc]:
      edge_cost, to = edge

      if not to in visited:
        heappush(todo, (cost + edge_cost, to))

  return -1

def main():
  n, m, k1, k2 = map(int, input().split())
  edges = list()
  for i in range(m):
    u, v, x, c = map(int, input().split())
    edges.append((u,v,x,c))
  s, t = map(int, input().split())

  # TODO: well, we know we need dijkstras, but what's the best way to translate the graph?

  # let's translate our graph into the dijkstra's format
  # per edge, if it's red or blue it becomes different directed edges in our graph, at each layer.
  # and white remains non-directional.

  # let's index by idx * k1l * k2l + k1_spot * k2l + k2_spot
  # We need one extra space because we can go from 0 to the limit.
  k1l, k2l = 1+k1, 1+k2

  def make_index(idx, k1s, k2s):
    return idx * (k1l * k2l) + k1s * k2l + k2s

  new_graph = [list() for _ in range(make_index(n, k1, k2) + 1)]

  for edge in edges:
    u,v,x,c = edge
    for (fro, to) in ((u,v), (v,u)):
      for k1_spot in range(k1l):
        for k2_spot in range(k2l):
          source_index = make_index(fro, k1_spot, k2_spot)
          if c == 0:
            target_index = make_index(to, k1_spot, k2_spot)
          elif c == 1:
            if k1_spot == k1:
              continue
            target_index = make_index(to, k1_spot + 1, k2_spot)
          elif c == 2:
            if k2_spot == k2:
              continue
            target_index = make_index(to, k1_spot, k2_spot + 1)
          else:
            print("ups")

          new_graph[source_index].append((x, target_index))

  s_index = make_index(s, 0, 0)
  t_index = make_index(t, k1, k2)

  print(dijkstras(s_index, t_index, new_graph))


if __name__ == "__main__":
    main()
