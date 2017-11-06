#include <iostream>
#include <iomanip>
#include <queue>

#define MAX_N 10
#define MAX_M 10

const int INF = 10000000;

char maze[MAX_N][MAX_M+1] = {
    "#S######.#",
    "......#..#",
    ".#.##.##.#",
    ".#........",
    "##.##.####",
    "....#....#",
    ".#######.#",
    "....#.....",
    ".####.###.",
    "....#...G#"
};

typedef std::pair<int, int> P;
int d[MAX_N][MAX_M];

int bfs ();

int main ()
{
    std::cout << bfs() << '\n';

    for (int i = 0;i < MAX_N; i++) {
        for (int j = 0; j < MAX_M; j++) {
            if (d[i][j] == INF) {
                std::cout << "###";
            } else {
                std::cout << std::setw(3) << d[i][j];
            }
        }
        std::cout << '\n';
    }
}

int dx[4] = {1, 0, -1, 0},
    dy[4] = {0, 1, 0, -1};

int bfs ()
{

    std::queue<P> que;

    for (int i = 0; i < MAX_N; i++) {
        for (int j = 0; j < MAX_M; j++) {
            d[i][j] = INF;
        }
    }

    int sy = 0,
        sx = 1;
    int gx = 8,
        gy = 9;

    que.push (P(sx, sy));
    d[sy][sx] = 0;

    while (!que.empty()) {
        P p = que.front ();
        que.pop ();

        if (p.first == gx && p.second == gy) { break; }

        for (int i = 0; i < 4; i++) {
            int nx = p.first  + dx[i];
            int ny = p.second + dy[i];

            if (0 <= nx && nx < MAX_N && 0 <= ny && ny < MAX_M &&
                maze[ny][nx] != '#' && d[ny][nx] == INF) {
                que.push (P(nx, ny));
                d[ny][nx] = d[p.second][p.first] + 1;
            }
        }
    }

    return d[gx][gy];
}
