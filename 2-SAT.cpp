struct TwoSAT {
    int N;
    vector<vector<int>> g, rg;
    vector<int> comp, order, vis;

    TwoSAT(int n) {
        N = n;
        g.resize(2 * n);
        rg.resize(2 * n);
    }

    int id(int x, int val) {
        return 2 * x + val;
    }

    void addEdge(int u, int v) {
        g[u].push_back(v);
        rg[v].push_back(u);
    }

    void imply(int x, int xv, int y, int yv) {
        addEdge(id(x, xv), id(y, yv));
    }

    void addOr(int x, int xv, int y, int yv) {
        imply(x, xv ^ 1, y, yv);
        imply(y, yv ^ 1, x, xv);
    }

    void dfs1(int node) {
        vis[node] = 1;

        for (auto &it : g[node]) {
            if (!vis[it]) {
                dfs1(it);
            }
        }

        order.push_back(node);
    }

    void dfs2(int node, int c) {
        comp[node] = c;

        for (auto &it : rg[node]) {
            if (comp[it] == -1) {
                dfs2(it, c);
            }
        }
    }

    bool solve() {
        vis.assign(2 * N, 0);
        comp.assign(2 * N, -1);
        order.clear();

        for (int i = 0; i < 2 * N; i++) {
            if (!vis[i]) {
                dfs1(i);
            }
        }

        reverse(order.begin(), order.end());

        int j = 0;

        for (int i = 0; i < 2 * N; i++) {
            if (comp[order[i]] == -1) {
                dfs2(order[i], j++);
            }
        }

        for (int i = 0; i < N; i++) {
            if (comp[2 * i] == comp[2 * i + 1]) {
                return false;
            }
        }

        return true;
    }
};