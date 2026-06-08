struct Tree {
    int n, LOG;
    vector<vector<pair<int,int>>> g;

    vector<int> parent, depth, sz, heavy;
    vector<vector<int>> up;
    vector<int> head, pos;
    vector<int> tin, tout;
    vector<long long> distRoot;
    int timer = 0;

    Tree(const vector<vector<int>>& adj, int root = 0) {
        n = adj.size();
        LOG = __lg(n) + 1;

        g.assign(n, {});
        for(int u = 0; u < n; u++)
            for(int v : adj[u])
                g[u].push_back({v, 1});

        init(root);
    }

    Tree(const vector<vector<pair<int,int>>>& adj, int root, int weighted) {
        n = adj.size();
        LOG = __lg(n) + 1;
        g = adj;
        init(root);
    }

    void init(int root) {
        parent.assign(n, -1);
        depth.assign(n, 0);
        sz.assign(n, 0);
        heavy.assign(n, -1);
        up.assign(LOG, vector<int>(n));
        head.assign(n, 0);
        pos.assign(n, 0);
        tin.assign(n, 0);
        tout.assign(n, 0);
        distRoot.assign(n, 0);

        dfs1(root, -1);
        dfs2(root, root);
    }

    void dfs1(int u, int p) {
        parent[u] = p;
        up[0][u] = (p == -1 ? u : p);

        for(int i = 1; i < LOG; i++)
            up[i][u] = up[i-1][ up[i-1][u] ];

        sz[u] = 1;
        heavy[u] = -1;

        int mx = 0;

        for(auto [v, w] : g[u]) {
            if(v == p) continue;
            depth[v] = depth[u] + 1;
            distRoot[v] = distRoot[u] + w;
            dfs1(v, u);
            sz[u] += sz[v];
            if(sz[v] > mx) {
                mx = sz[v];
                heavy[u] = v;
            }
        }
    }

    void dfs2(int u, int h) {
        head[u] = h;
        pos[u] = timer;
        tin[u] = timer++;

        if(heavy[u] != -1)
            dfs2(heavy[u], h);

        for(auto [v, w] : g[u]) {
            if(v == parent[u] || v == heavy[u]) continue;
            dfs2(v, v);
        }

        tout[u] = timer;
    }

    int lca(int a, int b) {
        if(depth[a] < depth[b])
            swap(a, b);

        int diff = depth[a] - depth[b];
        for(int i = 0; i < LOG; i++)
            if(diff & (1 << i))
                a = up[i][a];

        if(a == b) return a;

        for(int i = LOG - 1; i >= 0; i--) {
            if(up[i][a] != up[i][b]) {
                a = up[i][a];
                b = up[i][b];
            }
        }

        return parent[a];
    }

    long long dist(int a, int b) {
        int c = lca(a, b);
        return distRoot[a] + distRoot[b] - 2LL * distRoot[c];
    }

    bool is_ancestor(int u, int v) {
        return tin[u] <= tin[v] && tout[v] <= tout[u];
    }

    int kth_ancestor(int u, int k) {
        for(int i = 0; i < LOG; i++)
            if(k & (1 << i))
                u = up[i][u];
        return u;
    }

    template <typename OP>
    void path_query(int u, int v, OP op) {
        while(head[u] != head[v]) {
            if(depth[head[u]] < depth[head[v]])
                swap(u, v);
            op(pos[head[u]], pos[u]);
            u = parent[head[u]];
        }
        if(depth[u] > depth[v]) swap(u, v);
        op(pos[u], pos[v]);
    }

    pair<int,int> subtree(int u) {
        return {tin[u], tout[u] - 1};
    }
};
