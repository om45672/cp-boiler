// ---------------- Segment Tree ----------------
struct SegTree {
    int n;
    vector<ll> tree;

    void init(int _n) {
        n = _n;
        tree.assign(4 * n, 0);
    }

    void point_add(int idx, ll val, int node, int l, int r) {
        if (l == r) {
            tree[node] += val;
            return;
        }
        int mid = (l + r) / 2;
        if (idx <= mid)
            point_add(idx, val, 2 * node, l, mid);
        else
            point_add(idx, val, 2 * node + 1, mid + 1, r);
        tree[node] = tree[2 * node] + tree[2 * node + 1];
    }

    void point_add(int idx, ll val) {
        point_add(idx, val, 1, 0, n - 1);
    }

    ll query_range(int ql, int qr, int node, int l, int r) {
        if (qr < l || ql > r) return 0;
        if (ql <= l && r <= qr) return tree[node];
        int mid = (l + r) / 2;
        return query_range(ql, qr, 2 * node, l, mid)
             + query_range(ql, qr, 2 * node + 1, mid + 1, r);
    }

    ll query_range(int l, int r) {
        return query_range(l, r, 1, 0, n - 1);
    }
};

// ---------------- Heavy-Light Decomposition ----------------
struct HLD {
    int n;
    vector<vector<int>> adj;
    vector<int> parent, depth, heavy, head, pos, sz;
    vector<ll> val;
    vector<ll> baseArray;
    int curPos;
    SegTree seg;

    HLD(int _n = 0) { init(_n); }

    void init(int _n) {
        n = _n;
        adj.assign(n + 1, {});
        parent.assign(n + 1, 0);
        depth.assign(n + 1, 0);
        heavy.assign(n + 1, -1);
        head.assign(n + 1, 0);
        pos.assign(n + 1, 0);
        sz.assign(n + 1, 0);
        val.assign(n + 1, 0);
        baseArray.assign(n, 0);
        curPos = 0;
    }

    void addEdge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    int dfs(int u, int p) {
        parent[u] = p;
        depth[u] = (p == 0 ? 0 : depth[p] + 1);
        sz[u] = 1;
        int maxSub = 0;
        for (int v : adj[u]) if (v != p) {
            int sub = dfs(v, u);
            sz[u] += sub;
            if (sub > maxSub) {
                maxSub = sub;
                heavy[u] = v;
            }
        }
        return sz[u];
    }

    void decompose(int u, int h) {
        head[u] = h;
        pos[u] = curPos++;
        baseArray[pos[u]] = val[u];
        if (heavy[u] != -1)
            decompose(heavy[u], h);
        for (int v : adj[u]) if (v != parent[u] && v != heavy[u])
            decompose(v, v);
    }

    void build(int root = 1) {
        dfs(root, 0);
        decompose(root, root);
        seg.init(n);
        for (int i = 1; i <= n; ++i)
            seg.point_add(pos[i], baseArray[pos[i]]);
    }

    void update_node(int u, ll delta) {
        seg.point_add(pos[u], delta);
    }

    void set_node(int u, ll newVal) {
        ll cur = seg.query_range(pos[u], pos[u]);
        seg.point_add(pos[u], newVal - cur);
    }

    ll query_path(int u, int v) {
        ll res = 0;
        while (head[u] != head[v]) {
            if (depth[head[u]] < depth[head[v]]) swap(u, v);
            int h = head[u];
            res += seg.query_range(pos[h], pos[u]);
            u = parent[h];
        }
        if (depth[u] > depth[v]) swap(u, v);
        res += seg.query_range(pos[u], pos[v]);
        return res;
    }

    ll query_subtree(int u) {
        return seg.query_range(pos[u], pos[u] + sz[u] - 1);
    }

    int lca(int u, int v) {
        while (head[u] != head[v]) {
            if (depth[head[u]] > depth[head[v]]) u = parent[head[u]];
            else v = parent[head[v]];
        }
        return depth[u] < depth[v] ? u : v;
    }
};