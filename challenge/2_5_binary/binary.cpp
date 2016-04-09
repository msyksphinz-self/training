#include <iostream>
#include <vector>

#define MAX_V (100)

std::vector<int> G[MAX_V];

int V;
int color[MAX_V];

bool dfs(int v, int c)
{
    color[v] = c;
    for (int i = 0; i < G[v].size(); i++) {
        if (color[G[v][i]] == c) return false;
        if (color[G[v][i]] == 0 && !dfs(G[v][i], -c)) return false;
    }
    return true;
}


void solve ()
{
    for (int i = 0; i < V; i++) {
        if (color[i] == 0) {
            if (!dfs(i,  1)) {
                printf ("No\n");
                return;
            }
        }
    }
    printf ("Yes.\n");
}


int main ()
{
    V = 3;

    G[0].push_back(1);
    G[0].push_back(2);

    G[1].push_back(0);
    G[1].push_back(2);

    G[2].push_back(0);
    G[2].push_back(1);

    solve();
}
