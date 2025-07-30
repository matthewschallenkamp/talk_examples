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
  pass


if __name__ == "__main__":
    main()
