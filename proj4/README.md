Project 04: Path Finding
========================

This project implements a [Dijkstra's Algorithm] to perform rudimentary path
finding on a 2D tile-based map.

[Dijkstra's Algorithm]: https://en.wikipedia.org/wiki/Dijkstra%27s_algorithm


Input
-----

    TILES_N
    TILE_NAME_0	TILE_COST_0
    ...
    TILE_NAME_N-1	TILE_COST_N-1

    MAP_ROWS MAP_COLUMNS
    TILE_0_0    ...
    ...

    TRAVELER_START_ROW TRAVELER_START_COL
    TRAVELER_END_ROW   TRAVELER_END_COL

Output
------

    Cost
    [(ROW_0, COL_0), ...]

Build and Run
------

    make - builds ./dijkstras and ./generate map
    make test - runs the tests
    ./dijkstras < map.txt - prints the minimum cost and path
    ./generate_map > map.txt - makes a random N x N map

Benchmark Methodology
------

    ./generate_map N > map_N.txt
    /usr/bin/time -v ./dijkstras < map_N.txt > /dev/null

    Each map was run 5 times.

Benchmark Table
------

    | N    | Median Elapsed Time (seconds) | Median Memory (KB) |
    |------|-------------------------------|--------------------|
    | 10   | 0                             | 3360               |
    | 20   | 0                             | 3360               |
    | 50   | 0                             | 3360               |
    | 100  | 0.01                          | 3360               |
    | 200  | 0.02                          | 3584               |
    | 500  | 0.14                          | 6048               |
    | 1000 | 0.57                          | 15020              |

Limitations
------

    If multiple paths have the same lowest cost, the program may print a different path than the reference output, but the result will still be optimal.