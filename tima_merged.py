from collections import defaultdict
from heapq import heappush, heappop

def dijkstras(source, target, graph):
  # graph is a ragged array
  # graph[from] = [(cost, to, color), ...]

  visited = set()
  # todo is a heap of (cost, (to, k1, k2)) tuples.
  todo = []
  heappush(todo, (0, source))
  while todo:
    cost, loc = heappop(todo)
    to, k1, k2 = loc

    if loc in visited:
      continue
    if loc == target:
      return cost

    visited.add(loc)

    for edge in graph[to]:
      edge_cost, to, color = edge

      next_loc = (
        to, 
        k1 + (1 if color == 1 else 0),
        k2 + (1 if color == 2 else 0)
      )
      if next_loc[1] > target[1]:
        continue
      if next_loc[2] > target[2]:
        continue

      if not next_loc in visited:
        heappush(todo, (cost + edge_cost, next_loc))

  return -1


def main():
  n, m, k1, k2 = map(int, input().split())
  edges = list()
  for i in range(m):
    u, v, x, c = map(int, input().split())
    edges.append((u,v,x,c))
  s, t = map(int, input().split())

  # TODO: well, we know we need dijkstras, but what's the best way to translate the graph?

  # Let's just modify the dijkstra's to work on the graph implicitly, making 3 location items instead of 1
  graph = [list() for _ in range(n + 1)]
  for edge in edges:
    u,v,x,c = edge
    graph[u].append((x,v,c))
    graph[v].append((x,u,c))

  print(dijkstras((s,0,0), (t,k1,k2), graph))

if __name__ == "__main__":
    main()
