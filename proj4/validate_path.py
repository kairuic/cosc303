#!/usr/bin/env python3
"""Validate CS302 Project 4 output against a map and an optimal Dijkstra cost.

Usage: ./dijkstras < INPUT_MAP | python3 validate_path.py INPUT_MAP
Exit 0 on a valid minimum-cost path; exit 1 otherwise.
"""
import heapq
import sys
from pathlib import Path


def fail(message):
    print(f"FAIL: {message}", file=sys.stderr)
    return 1


def parse_map(filename):
    tokens = Path(filename).read_text().split()
    pos = 0

    def take():
        nonlocal pos
        if pos >= len(tokens):
            raise ValueError("map input ends unexpectedly")
        result = tokens[pos]
        pos += 1
        return result

    ntypes = int(take())
    if ntypes <= 0:
        raise ValueError("number of terrain types must be positive")
    costs = {}
    for _ in range(ntypes):
        symbol, cost = take(), int(take())
        if len(symbol) != 1 or symbol in costs or cost <= 0:
            raise ValueError("invalid terrain definition")
        costs[symbol] = cost

    rows, cols = int(take()), int(take())
    if rows <= 0 or cols <= 0:
        raise ValueError("map dimensions must be positive")
    terrain = [[take() for _ in range(cols)] for _ in range(rows)]
    if any(tile not in costs for row in terrain for tile in row):
        raise ValueError("undefined terrain symbol in map")
    start = (int(take()), int(take()))
    goal = (int(take()), int(take()))
    if pos != len(tokens):
        raise ValueError("extra tokens after ending coordinates")
    for name, (r, c) in (("start", start), ("goal", goal)):
        if not (0 <= r < rows and 0 <= c < cols):
            raise ValueError(f"{name} is outside the map")
    return costs, terrain, start, goal


def optimal_cost(costs, terrain, start, goal):
    rows, cols = len(terrain), len(terrain[0])
    dist = [[float('inf')] * cols for _ in range(rows)]
    sr, sc = start
    dist[sr][sc] = 0
    heap = [(0, sr, sc)]
    while heap:
        d, r, c = heapq.heappop(heap)
        if d != dist[r][c]:
            continue
        if (r, c) == goal:
            return d
        move_cost = costs[terrain[r][c]]  # Cost of LEAVING tile (r, c)
        for nr, nc in ((r-1, c), (r+1, c), (r, c-1), (r, c+1)):
            if 0 <= nr < rows and 0 <= nc < cols:
                new_dist = d + move_cost
                if new_dist < dist[nr][nc]:
                    dist[nr][nc] = new_dist
                    heapq.heappush(heap, (new_dist, nr, nc))
    raise ValueError("goal unreachable")


def validate(map_file):
    try:
        costs, terrain, start, goal = parse_map(map_file)
    except (OSError, ValueError) as exc:
        return fail(f"invalid map: {exc}")
    try:
        tokens = sys.stdin.read().split()
        numbers = [int(token) for token in tokens]
    except (OSError, ValueError) as exc:
        return fail(f"student output must contain only integers: {exc}")
    if len(numbers) < 3 or len(numbers) % 2 != 1:
        return fail("expected a cost followed by one or more row/column pairs")
    reported_cost = numbers[0]
    path = list(zip(numbers[1::2], numbers[2::2]))
    if path[0] != start:
        return fail(f"path begins at {path[0]}, not {start}")
    if path[-1] != goal:
        return fail(f"path ends at {path[-1]}, not {goal}")
    rows, cols = len(terrain), len(terrain[0])
    actual_cost = 0
    for i, (r, c) in enumerate(path):
        if not (0 <= r < rows and 0 <= c < cols):
            return fail(f"step {i} at {(r, c)} is outside the map")
        if i + 1 < len(path):
            nr, nc = path[i + 1]
            if abs(nr - r) + abs(nc - c) != 1:
                return fail(f"illegal move at step {i}: {(r, c)} -> {(nr, nc)}")
            actual_cost += costs[terrain[r][c]]
    if reported_cost != actual_cost:
        return fail(f"reported cost {reported_cost} != path cost {actual_cost}")
    optimum = optimal_cost(costs, terrain, start, goal)
    if actual_cost != optimum:
        return fail(f"path cost {actual_cost} != optimal cost {optimum}")
    print(f"PASS: optimal cost {optimum}; {len(path)} cells")
    return 0


if __name__ == '__main__':
    if len(sys.argv) != 2:
        print(f"Usage: {sys.argv[0]} INPUT_MAP", file=sys.stderr)
        sys.exit(2)
    sys.exit(validate(sys.argv[1]))
