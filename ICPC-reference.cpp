#pragma GCC optimize("O3,unroll-loops,no-stack-protector,fast-math")
#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
using namespace std;
using namespace __gnu_pbds;

#ifdef ONLINE_JUDGE
#define debug(...) 42
#else
#include "debug.h"
#endif

#define int long long
using ll = long long;
using ld = long double;

using pii = pair<int,int>;
using pll = pair<ll,ll>;
using vi = vector<int>;
using vll = vector<ll>;

const int MOD = 1'000'000'007;
const int mod = 998'244'353;
const ll INF = 4'000'000'000'000'000'000LL;

#define all(x) (x).begin(), (x).end()
#define rall(x) (x).rbegin(), (x).rend()
#define pb push_back
#define eb emplace_back
#define ff first
#define ss second
#define sz(x) (int)(x).size()

#define yes cout << "YES\n"
#define no cout << "NO\n"
#define fr(i,n) for(int i = 0; i < (n); ++i)
#define rep(i,a,b) for(int i = (a); i <= (b); ++i)
#define per(i,a,b) for(int i = (a); i >= (b); --i)
#define OmPatel() ios::sync_with_stdio(0); cin.tie(0); cout.tie(0)

// ============================================================
// 0. PBDS ordered set
// ============================================================
template<class T>
using ordered_set = tree<T, null_type, less<T>, rb_tree_tag,
                         tree_order_statistics_node_update>;

// ordered_set<int> s;
// s.order_of_key(x) -> # elements < x
// *s.find_by_order(k) -> k-th element (0-indexed)
// Multiset: use ordered_set<pair<int,int>>.

// ============================================================
// 1. DSU
// ============================================================
struct DSU {
    vi parent, sz;

    DSU(int n = 0) {
        parent.resize(n + 1);
        sz.assign(n + 1, 1);
        iota(all(parent), 0);
    }

    int find(int x) {
        return parent[x] == x ? x : parent[x] = find(parent[x]);
    }

    bool unite(int a, int b) {
        a = find(a), b = find(b);
        if (a == b) return false;
        if (sz[a] < sz[b]) swap(a, b);
        parent[b] = a;
        sz[a] += sz[b];
        return true;
    }

    int size(int x) {
        return sz[find(x)];
    }
};

// ============================================================
// 2. Rollback DSU
// ============================================================
struct RollbackDSU {
    vi parent, sz;
    vector<pii> st;

    RollbackDSU(int n = 0) {
        parent.resize(n);
        sz.assign(n, 1);
        iota(all(parent), 0);
    }

    int find(int x) {
        return parent[x] == x ? x : find(parent[x]);
    }

    int time() const {
        return sz(st);
    }

    bool unite(int a, int b) {
        a = find(a), b = find(b);
        if (a == b) return false;
        if (sz[a] < sz[b]) swap(a, b);

        st.pb({b, sz[a]});
        parent[b] = a;
        sz[a] += sz[b];
        return true;
    }

    void rollback(int t) {
        while (sz(st) > t) {
            auto [b, oldSize] = st.back();
            st.pop_back();
            int a = parent[b];
            parent[b] = b;
            sz[a] = oldSize;
        }
    }
};

// ============================================================
// 3. Fenwick Tree (1-indexed)
// ============================================================
struct Fenwick {
    int n;
    vector<ll> bit;

    Fenwick(int n = 0) : n(n), bit(n + 1, 0) {}

    void add(int i, ll val) {
        for (; i <= n; i += i & -i)
            bit[i] += val;
    }

    ll get(int i) const {
        ll s = 0;
        for (; i > 0; i -= i & -i)
            s += bit[i];
        return s;
    }

    ll query(int l, int r) const {
        return l > r ? 0 : get(r) - get(l - 1);
    }

    // smallest 1-indexed position with prefix sum >= k
    int Kth(ll k) const {
        int pos = 0;
        ll sum = 0;

        for (int i = __lg(max(1LL, n)); i >= 0; --i) {
            int pw = 1LL << i;
            if (pos + pw <= n && sum + bit[pos + pw] < k) {
                pos += pw;
                sum += bit[pos];
            }
        }
        return pos + 1;
    }
};

// ============================================================
// 4. Iterative Segment Tree
// [l,r] query -- CURRENT OPERATION: minimum
// Change merge() and ID to get max/sum/gcd/etc.
// ============================================================
struct SegTree {
    int n;
    vector<ll> t;

    static constexpr ll ID = INF;

    static ll merge(ll a, ll b) {
        return min(a, b);
    }

    SegTree(int n = 0) : n(n), t(2 * n, ID) {}

    SegTree(const vector<ll>& a) {
        n = sz(a);
        t.assign(2 * n, ID);

        fr(i, n) t[n + i] = a[i];
        for (int i = n - 1; i; --i)
            t[i] = merge(t[i << 1], t[i << 1 | 1]);
    }

    void modify(int pos, ll val) {
        for (t[pos += n] = val; pos >>= 1;)
            t[pos] = merge(t[pos << 1], t[pos << 1 | 1]);
    }

    ll query(int l, int r) const { // inclusive
        ll left = ID, right = ID;

        for (l += n, r += n; l <= r; l >>= 1, r >>= 1) {
            if (l & 1) left = merge(left, t[l++]);
            if (!(r & 1)) right = merge(t[r--], right);
        }

        return merge(left, right);
    }
};

// ============================================================
// 5. Lazy Segment Tree
// Range add + range minimum query.
// ============================================================
struct LazySegTree {
    int n;
    vector<ll> tree, lazy;

    LazySegTree(int n = 0)
        : n(n), tree(4 * n + 5, 0), lazy(4 * n + 5, 0) {}

    void build(const vector<ll>& a, int node, int l, int r) {
        if (l == r) {
            tree[node] = a[l];
            return;
        }

        int mid = (l + r) / 2;
        build(a, node << 1, l, mid);
        build(a, node << 1 | 1, mid + 1, r);
        tree[node] = min(tree[node << 1], tree[node << 1 | 1]);
    }

    void build(const vector<ll>& a) {
        n = sz(a);
        tree.assign(4 * n + 5, 0);
        lazy.assign(4 * n + 5, 0);
        if (n) build(a, 1, 0, n - 1);
    }

    void apply(int node, ll x) {
        tree[node] += x;
        lazy[node] += x;
    }

    void push(int node) {
        if (!lazy[node]) return;
        apply(node << 1, lazy[node]);
        apply(node << 1 | 1, lazy[node]);
        lazy[node] = 0;
    }

    void add(int ql, int qr, ll x, int node, int l, int r) {
        if (qr < l || r < ql) return;

        if (ql <= l && r <= qr) {
            apply(node, x);
            return;
        }

        push(node);
        int mid = (l + r) / 2;
        add(ql, qr, x, node << 1, l, mid);
        add(ql, qr, x, node << 1 | 1, mid + 1, r);
        tree[node] = min(tree[node << 1], tree[node << 1 | 1]);
    }

    void add(int l, int r, ll x) {
        if (n && l <= r)
            add(l, r, x, 1, 0, n - 1);
    }

    ll query(int ql, int qr, int node, int l, int r) {
        if (qr < l || r < ql) return INF;
        if (ql <= l && r <= qr) return tree[node];

        push(node);
        int mid = (l + r) / 2;
        return min(query(ql, qr, node << 1, l, mid),
                   query(ql, qr, node << 1 | 1, mid + 1, r));
    }

    ll query(int l, int r) {
        return (n && l <= r) ? query(l, r, 1, 0, n - 1) : INF;
    }
};

// ============================================================
// 6. Dynamic Li Chao Tree
//
// Huge implicit x-domain: [-2^62, 2^62).
// Only touched nodes are created.
//
// Supports:
//   add_line(m,b)              -> line on the full x-domain
//   add_segment(l,r,m,b)       -> line only on [l,r)
//
// Default = minimum.
// For maximum: LiChao<false>.
//
// Uses __int128 for m*x+b internally.
// ============================================================
template<bool isMin = true>
struct LiChao {
    static constexpr ll XL = -(1LL << 62);
    static constexpr ll XR =  (1LL << 62);

    struct Line {
        ll m, b;

        Line(ll _m = 0, ll _b = isMin ? INF : -INF)
            : m(_m), b(_b) {}

        ll value(ll x) const {
            __int128 y = (__int128)m * x + b;
            if (y > INF) return INF;
            if (y < -INF) return -INF;
            return (ll)y;
        }
    };

    struct Node {
        Line line;
        Node *l = nullptr, *r = nullptr;

        Node(Line _line) : line(_line) {}
    };

    Node* root = nullptr;

    static bool better(ll a, ll b) {
        if constexpr (isMin) return a < b;
        else return a > b;
    }

    static ll combine(ll a, ll b) {
        if constexpr (isMin) return min(a, b);
        else return max(a, b);
    }

    void add_line(Line nw) {
        add_line(root, XL, XR, nw);
    }

    void add_line(Node*& node, ll l, ll r, Line nw) {
        if (!node) {
            node = new Node(nw);
            return;
        }

        ll mid = l + (r - l) / 2;

        bool lef = better(nw.value(l), node->line.value(l));
        bool midBetter = better(nw.value(mid), node->line.value(mid));

        if (midBetter)
            swap(node->line, nw);

        if (r - l <= 1)
            return;

        if (lef != midBetter)
            add_line(node->l, l, mid, nw);
        else
            add_line(node->r, mid, r, nw);
    }

    // Add y = m*x+b only on x in [ql,qr).
    void add_segment(ll ql, ll qr, Line nw) {
        ql = max(ql, XL);
        qr = min(qr, XR);

        if (ql >= qr) return;
        add_segment(root, XL, XR, ql, qr, nw);
    }

    void add_segment(Node*& node, ll l, ll r,
                     ll ql, ll qr, Line nw) {
        if (qr <= l || r <= ql)
            return;

        if (ql <= l && r <= qr) {
            add_line(node, l, r, nw);
            return;
        }

        if (!node)
            node = new Node(Line());

        ll mid = l + (r - l) / 2;

        add_segment(node->l, l, mid, ql, qr, nw);
        add_segment(node->r, mid, r, ql, qr, nw);
    }

    ll query(ll x) const {
        if (x < XL || x >= XR)
            return isMin ? INF : -INF;

        return query(root, XL, XR, x);
    }

    ll query(Node* node, ll l, ll r, ll x) const {
        if (!node)
            return isMin ? INF : -INF;

        ll ans = node->line.value(x);

        if (r - l <= 1)
            return ans;

        ll mid = l + (r - l) / 2;

        if (x < mid)
            return combine(ans, query(node->l, l, mid, x));
        return combine(ans, query(node->r, mid, r, x));
    }
};

// ============================================================
// 7. Convex Hull Trick
// Monotonic slopes + monotonic query x.
// Minimum hull, increasing slopes.
// ============================================================
struct CHT {
    struct Line {
        ll slope, yIntercept;

        Line(ll slope = 0, ll yIntercept = 0)
            : slope(slope), yIntercept(yIntercept) {}

        ll val(ll x) const {
            return slope * x + yIntercept;
        }

        // First integer x where 'y' becomes <= this line.
        ll intersect(Line y) const {
            return (y.yIntercept - yIntercept + slope - y.slope - 1)
                 / (slope - y.slope);
        }
    };

    deque<pair<Line, ll>> dq;

    void insert(ll slope, ll yIntercept) {
        Line nw(slope, yIntercept);

        while (!dq.empty() && dq.back().first.slope == nw.slope) {
            if (dq.back().first.yIntercept <= nw.yIntercept)
                return;
            dq.pop_back();
        }

        while (dq.size() > 1 &&
               dq.back().second >= dq.back().first.intersect(nw))
            dq.pop_back();

        if (dq.empty())
            dq.emplace_back(nw, -INF);
        else
            dq.emplace_back(nw, dq.back().first.intersect(nw));
    }

    ll query(ll x) {
        while (dq.size() > 1 && dq[1].second <= x)
            dq.pop_front();

        return dq.front().first.val(x);
    }
};

// ============================================================
// 8. LCA - Binary Lifting
// Root = 0.
// ============================================================
struct LCA {
    int n, LOG;
    vector<vi> adj, up;
    vi depth;

    LCA(int n = 0) {
        init(n);
    }

    void init(int _n) {
        n = _n;
        LOG = 1;
        while ((1LL << LOG) <= max(1LL, n))
            ++LOG;

        adj.assign(n, {});
        up.assign(LOG, vi(n));
        depth.assign(n, 0);
    }

    void addEdge(int u, int v) {
        adj[u].pb(v);
        adj[v].pb(u);
    }

    void build(int root = 0) {
        function<void(int,int)> dfs = [&](int u, int p) {
            up[0][u] = p;

            for (int v : adj[u]) if (v != p) {
                depth[v] = depth[u] + 1;
                dfs(v, u);
            }
        };

        dfs(root, root);

        rep(j,1,LOG - 1)
            fr(i,n)
                up[j][i] = up[j - 1][up[j - 1][i]];
    }

    int jump(int u, int k) const {
        for (int j = 0; j < LOG; ++j)
            if (k >> j & 1)
                u = up[j][u];
        return u;
    }

    int lca(int a, int b) const {
        if (depth[a] < depth[b])
            swap(a, b);

        a = jump(a, depth[a] - depth[b]);

        if (a == b)
            return a;

        for (int j = LOG - 1; j >= 0; --j)
            if (up[j][a] != up[j][b])
                a = up[j][a], b = up[j][b];

        return up[0][a];
    }

    int dist(int a, int b) const {
        int c = lca(a, b);
        return depth[a] + depth[b] - 2 * depth[c];
    }
};

// ============================================================
// 9. HLD
// Additive node values + path/subtree sum.
// Adapt merge/query if needed.
// ============================================================
struct HLD {
    int n, curPos = 0;

    vector<vi> adj;
    vi parent, depth, heavy, head, pos, sub;
    vector<ll> val, base, seg;

    HLD(int n = 0) {
        init(n);
    }

    void init(int _n) {
        n = _n;
        curPos = 0;

        adj.assign(n + 1, {});
        parent.assign(n + 1, 0);
        depth.assign(n + 1, 0);
        heavy.assign(n + 1, -1);
        head.assign(n + 1, 0);
        pos.assign(n + 1, 0);
        sub.assign(n + 1, 0);

        val.assign(n + 1, 0);
        base.assign(n, 0);
        seg.assign(4 * n + 5, 0);
    }

    void addEdge(int u, int v) {
        adj[u].pb(v);
        adj[v].pb(u);
    }

    int dfs(int u, int p) {
        parent[u] = p;
        sub[u] = 1;

        int best = 0;

        for (int v : adj[u]) if (v != p) {
            depth[v] = depth[u] + 1;

            int s = dfs(v, u);
            sub[u] += s;

            if (s > best)
                best = s, heavy[u] = v;
        }

        return sub[u];
    }

    void decompose(int u, int h) {
        head[u] = h;
        pos[u] = curPos;
        base[curPos++] = val[u];

        if (heavy[u] != -1)
            decompose(heavy[u], h);

        for (int v : adj[u])
            if (v != parent[u] && v != heavy[u])
                decompose(v, v);
    }

    void buildSeg(int node, int l, int r) {
        if (l == r) {
            seg[node] = base[l];
            return;
        }

        int mid = (l + r) / 2;
        buildSeg(node << 1, l, mid);
        buildSeg(node << 1 | 1, mid + 1, r);

        seg[node] = seg[node << 1] + seg[node << 1 | 1];
    }

    ll querySeg(int ql, int qr, int node,
                int l, int r) const {
        if (qr < l || r < ql)
            return 0;

        if (ql <= l && r <= qr)
            return seg[node];

        int mid = (l + r) / 2;

        return querySeg(ql, qr, node << 1, l, mid)
             + querySeg(ql, qr, node << 1 | 1, mid + 1, r);
    }

    void pointAdd(int idx, ll x, int node,
                  int l, int r) {
        if (l == r) {
            seg[node] += x;
            return;
        }

        int mid = (l + r) / 2;

        if (idx <= mid)
            pointAdd(idx, x, node << 1, l, mid);
        else
            pointAdd(idx, x, node << 1 | 1, mid + 1, r);

        seg[node] = seg[node << 1] + seg[node << 1 | 1];
    }

    void build(int root = 1) {
        dfs(root, 0);
        decompose(root, root);
        buildSeg(1, 0, n - 1);
    }

    void addNode(int u, ll x) {
        pointAdd(pos[u], x, 1, 0, n - 1);
    }

    ll queryPath(int u, int v) const {
        ll ans = 0;

        while (head[u] != head[v]) {
            if (depth[head[u]] < depth[head[v]])
                swap(u, v);

            ans += querySeg(pos[head[u]], pos[u],
                            1, 0, n - 1);

            u = parent[head[u]];
        }

        if (depth[u] > depth[v])
            swap(u, v);

        return ans + querySeg(pos[u], pos[v],
                              1, 0, n - 1);
    }

    ll querySubtree(int u) const {
        return querySeg(pos[u], pos[u] + sub[u] - 1,
                        1, 0, n - 1);
    }

    int lca(int u, int v) const {
        while (head[u] != head[v]) {
            if (depth[head[u]] > depth[head[v]])
                u = parent[head[u]];
            else
                v = parent[head[v]];
        }

        return depth[u] < depth[v] ? u : v;
    }
};

// ============================================================
// 10. SCC (Kosaraju) + 2-SAT
// ============================================================
struct SCC {
    int n;
    vector<vi> g, rg;
    vi vis, comp, order;

    SCC(int n = 0)
        : n(n), g(n), rg(n), vis(n), comp(n, -1) {}

    void addEdge(int u, int v) {
        g[u].pb(v);
        rg[v].pb(u);
    }

    void dfs1(int u) {
        vis[u] = 1;
        for (int v : g[u])
            if (!vis[v]) dfs1(v);
        order.pb(u);
    }

    void dfs2(int u, int c) {
        comp[u] = c;
        for (int v : rg[u])
            if (comp[v] == -1) dfs2(v, c);
    }

    int build() {
        fill(all(vis), 0);
        order.clear();

        fr(i,n)
            if (!vis[i]) dfs1(i);

        fill(all(comp), -1);
        reverse(all(order));

        int cc = 0;

        for (int u : order)
            if (comp[u] == -1)
                dfs2(u, cc++);

        return cc;
    }
};

struct TwoSAT {
    int N;
    vector<vi> g, rg;
    vi comp, order, vis;

    TwoSAT(int n = 0)
        : N(n), g(2 * n), rg(2 * n) {}

    int id(int x, int val) const {
        return 2 * x + val;
    }

    void addEdge(int u, int v) {
        g[u].pb(v);
        rg[v].pb(u);
    }

    void imply(int x, int xv, int y, int yv) {
        addEdge(id(x, xv), id(y, yv));
    }

    // (x=xv) OR (y=yv)
    void addOr(int x, int xv, int y, int yv) {
        imply(x, xv ^ 1, y, yv);
        imply(y, yv ^ 1, x, xv);
    }

    void dfs1(int u) {
        vis[u] = 1;
        for (int v : g[u])
            if (!vis[v]) dfs1(v);
        order.pb(u);
    }

    void dfs2(int u, int c) {
        comp[u] = c;
        for (int v : rg[u])
            if (comp[v] == -1) dfs2(v, c);
    }

    bool solve() {
        vis.assign(2 * N, 0);
        comp.assign(2 * N, -1);
        order.clear();

        fr(i, 2 * N)
            if (!vis[i]) dfs1(i);

        reverse(all(order));

        int cc = 0;

        for (int u : order)
            if (comp[u] == -1)
                dfs2(u, cc++);

        fr(i, N)
            if (comp[2 * i] == comp[2 * i + 1])
                return false;

        return true;
    }
};

// ============================================================
// 11. Topological Sort + Floyd-Warshall
// ============================================================
vi topoSort(const vector<vi>& g) {
    int n = sz(g);
    vi indeg(n), ret;
    queue<int> q;

    fr(u,n)
        for (int v : g[u])
            ++indeg[v];

    fr(i,n)
        if (!indeg[i])
            q.push(i);

    while (!q.empty()) {
        int u = q.front();
        q.pop();

        ret.pb(u);

        for (int v : g[u])
            if (--indeg[v] == 0)
                q.push(v);
    }

    return ret;
}

void floydWarshall(vector<vector<ll>>& d) {
    int n = sz(d);

    fr(i,n)
        d[i][i] = min(d[i][i], 0LL);

    fr(k,n)
        fr(i,n) if (d[i][k] < INF)
            fr(j,n) if (d[k][j] < INF)
                d[i][j] = min(d[i][j], d[i][k] + d[k][j]);

    fr(k,n) if (d[k][k] < 0)
        fr(i,n)
            fr(j,n)
                if (d[i][k] < INF && d[k][j] < INF)
                    d[i][j] = -INF;
}

// ============================================================
// 12. Dinic
// ============================================================
struct Dinic {
    struct Edge {
        int to, rev;
        ll cap, orig;

        ll flow() const {
            return max(0LL, orig - cap);
        }
    };

    int n;
    vector<vector<Edge>> adj;
    vi lvl, ptr;

    Dinic(int n = 0)
        : n(n), adj(n), lvl(n), ptr(n) {}

    void addEdge(int u, int v, ll cap, ll rcap = 0) {
        Edge a{v, sz(adj[v]), cap, cap};
        Edge b{u, sz(adj[u]), rcap, rcap};

        adj[u].pb(a);
        adj[v].pb(b);
    }

    bool bfs(int s, int t) {
        fill(all(lvl), -1);

        queue<int> q;
        lvl[s] = 0;
        q.push(s);

        while (!q.empty()) {
            int u = q.front();
            q.pop();

            for (auto &e : adj[u]) {
                if (e.cap > 0 && lvl[e.to] == -1) {
                    lvl[e.to] = lvl[u] + 1;
                    q.push(e.to);
                }
            }
        }

        return lvl[t] != -1;
    }

    ll dfs(int u, int t, ll pushed) {
        if (u == t || !pushed)
            return pushed;

        for (int &i = ptr[u]; i < sz(adj[u]); ++i) {
            Edge &e = adj[u][i];

            if (e.cap > 0 && lvl[e.to] == lvl[u] + 1) {
                ll f = dfs(e.to, t, min(pushed, e.cap));

                if (!f)
                    continue;

                e.cap -= f;
                adj[e.to][e.rev].cap += f;

                return f;
            }
        }

        return 0;
    }

    ll maxflow(int s, int t) {
        ll flow = 0;

        while (bfs(s, t)) {
            fill(all(ptr), 0);

            while (ll f = dfs(s, t, INF))
                flow += f;
        }

        return flow;
    }
};

// ============================================================
// 13. Z + KMP prefix + Manacher
// ============================================================
vi z_function(const string& s) {
    int n = sz(s);
    vi z(n);

    for (int i = 1, l = 0, r = 0; i < n; ++i) {
        if (i < r)
            z[i] = min(r - i, z[i - l]);

        while (i + z[i] < n &&
               s[z[i]] == s[i + z[i]])
            ++z[i];

        if (i + z[i] > r)
            l = i, r = i + z[i];
    }

    return z;
}

vi prefix_function(const string& s) {
    int n = sz(s);
    vi pi(n);

    for (int i = 1; i < n; ++i) {
        int j = pi[i - 1];

        while (j && s[i] != s[j])
            j = pi[j - 1];

        if (s[i] == s[j])
            ++j;

        pi[i] = j;
    }

    return pi;
}

// d1[i] = radius of odd palindrome at i, including center.
vi manacher_odd(const string& s) {
    int n = sz(s);
    vi d1(n);

    for (int l = 0, r = -1, i = 0; i < n; ++i) {
        int k = (i > r ? 1 : min(d1[l + r - i], r - i + 1));

        while (0 <= i - k && i + k < n &&
               s[i - k] == s[i + k])
            ++k;

        d1[i] = k--;

        if (i + k > r)
            l = i - k, r = i + k;
    }

    return d1;
}

// d2[i] = radius of even palindrome centered between i-1 and i.
vi manacher_even(const string& s) {
    int n = sz(s);
    vi d2(n);

    for (int l = 0, r = -1, i = 0; i < n; ++i) {
        int k = (i > r ? 0 : min(d2[l + r - i + 1], r - i + 1));

        while (0 <= i - k - 1 && i + k < n &&
               s[i - k - 1] == s[i + k])
            ++k;

        d2[i] = k--;

        if (i + k > r)
            l = i - k - 1, r = i + k;
    }

    return d2;
}

// ============================================================
// 14. String Hashing (64-bit)
// ============================================================
struct Hash {
    using ull = unsigned long long;

    vector<ull> p, h;
    ull base;

    Hash(string& s) {
        base = chrono::steady_clock::now()
                   .time_since_epoch().count() | 1ULL;

        p.resize(sz(s) + 1);
        h.resize(sz(s) + 1);

        p[0] = 1;

        fr(i,sz(s)) {
            p[i + 1] = p[i] * base;
            h[i + 1] = h[i] * base + (unsigned char)s[i];
        }
    }

    ull get(int l, int r) const { // [l,r)
        return h[r] - h[l] * p[r - l];
    }
};

// ============================================================
// 15. Matrix
// ============================================================
struct Matrix {
    int n, m;
    vector<vector<int>> mat;

    Matrix(int n = 0, int m = 0)
        : n(n), m(m), mat(n, vector<int>(m, 0)) {}

    static Matrix identity(int n) {
        Matrix I(n, n);

        fr(i,n)
            I.mat[i][i] = 1;

        return I;
    }

    Matrix operator*(const Matrix& o) const {
        assert(m == o.n);

        Matrix res(n, o.m);

        fr(i,n) fr(k,m) if (mat[i][k])
            fr(j,o.m)
                res.mat[i][j] =
                    (res.mat[i][j] +
                     1LL * mat[i][k] * o.mat[k][j]) % MOD;

        return res;
    }

    Matrix power(ll e) const {
        assert(n == m);

        Matrix res = identity(n);
        Matrix base = *this;

        while (e) {
            if (e & 1)
                res = res * base;

            base = base * base;
            e >>= 1;
        }

        return res;
    }
};

// ============================================================
// 16. Modular arithmetic + factorials
// ============================================================
ll modpow(ll a, ll e, ll m = mod) {
    ll r = 1 % m;
    a %= m;

    while (e) {
        if (e & 1)
            r = (ll)((__int128)r * a % m);

        a = (ll)((__int128)a * a % m);
        e >>= 1;
    }

    return r;
}

struct Mint {
    ll x;

    Mint(ll x = 0) {
        this->x = x % mod;
        if (this->x < 0) this->x += mod;
    }

    Mint power(ll e) const {
        return Mint(modpow(x, e));
    }

    Mint inv() const {
        return power(mod - 2);
    }

    Mint& operator+=(const Mint& o) {
        x += o.x;
        if (x >= mod) x -= mod;
        return *this;
    }

    Mint& operator-=(const Mint& o) {
        x -= o.x;
        if (x < 0) x += mod;
        return *this;
    }

    Mint& operator*=(const Mint& o) {
        x = (ll)((__int128)x * o.x % mod);
        return *this;
    }

    Mint& operator/=(const Mint& o) {
        return *this *= o.inv();
    }

    friend Mint operator+(Mint a, const Mint& b) { return a += b; }
    friend Mint operator-(Mint a, const Mint& b) { return a -= b; }
    friend Mint operator*(Mint a, const Mint& b) { return a *= b; }
    friend Mint operator/(Mint a, const Mint& b) { return a /= b; }

    friend ostream& operator<<(ostream& os, const Mint& a) {
        return os << a.x;
    }
};

struct Factorials {
    int n;
    vector<Mint> fact, ifact;

    Factorials(int n = 0) {
        init(n);
    }

    void init(int _n) {
        n = _n;

        fact.assign(n + 1, 1);
        ifact.assign(n + 1, 1);

        rep(i,1,n)
            fact[i] = fact[i - 1] * i;

        if (n) {
            ifact[n] = fact[n].inv();

            for (int i = n; i >= 1; --i)
                ifact[i - 1] = ifact[i] * i;
        }
    }

    Mint C(int n, int r) const {
        if (n < 0 || r < 0 || r > n || n > this->n)
            return 0;

        return fact[n] * ifact[r] * ifact[n - r];
    }

    Mint P(int n, int r) const {
        if (n < 0 || r < 0 || r > n || n > this->n)
            return 0;

        return fact[n] * ifact[n - r];
    }

    Mint starsBars(int n, int r) const {
        if (n < 0 || r <= 0)
            return 0;

        return C(n + r - 1, r - 1);
    }
};

// ============================================================
// 17. 64-bit Miller-Rabin
// ============================================================
ll mul_mod(ll a, ll b, ll m) {
    return (ll)((__int128)a * b % m);
}

ll pow_mod(ll a, ll d, ll m) {
    ll r = 1 % m;

    while (d) {
        if (d & 1)
            r = mul_mod(r, a, m);

        a = mul_mod(a, a, m);
        d >>= 1;
    }

    return r;
}

bool isPrime64(ll n) {
    if (n < 2)
        return false;

    for (ll p : {2LL,3LL,5LL,7LL,11LL,13LL,
                 17LL,19LL,23LL,29LL,31LL,37LL}) {
        if (n == p)
            return true;
        if (n % p == 0)
            return false;
    }

    ll d = n - 1, s = 0;

    while (!(d & 1))
        d >>= 1, ++s;

    for (ll a : {2LL,325LL,9375LL,28178LL,
                 450775LL,9780504LL,1795265022LL}) {
        if (a % n == 0)
            continue;

        ll x = pow_mod(a % n, d, n);

        if (x == 1 || x == n - 1)
            continue;

        bool composite = true;

        for (int r = 1; r < s; ++r) {
            x = mul_mod(x, x, n);

            if (x == n - 1) {
                composite = false;
                break;
            }
        }

        if (composite)
            return false;
    }

    return true;
}

// ============================================================
// 18. Trie
// ============================================================
struct Trie {
    static const int ALPHA = 26;

    struct Node {
        array<int, ALPHA> nxt;
        int cnt = 0, end = 0;

        Node() {
            nxt.fill(-1);
        }
    };

    vector<Node> tr;

    Trie() {
        tr.emplace_back();
    }

    void insert(const string& s) {
        int u = 0;

        for (char c : s) {
            int x = c - 'a';

            if (tr[u].nxt[x] == -1) {
                tr[u].nxt[x] = sz(tr);
                tr.emplace_back();
            }

            u = tr[u].nxt[x];
            ++tr[u].cnt;
        }

        ++tr[u].end;
    }

    int countPrefix(const string& s) const {
        int u = 0;

        for (char c : s) {
            int x = c - 'a';

            if (tr[u].nxt[x] == -1)
                return 0;

            u = tr[u].nxt[x];
        }

        return tr[u].cnt;
    }

    int countWord(const string& s) const {
        int u = 0;

        for (char c : s) {
            int x = c - 'a';

            if (tr[u].nxt[x] == -1)
                return 0;

            u = tr[u].nxt[x];
        }

        return tr[u].end;
    }
};

// ============================================================
// 19. Contest skeleton
// ============================================================
void solve() {

}

signed main() {
    OmPatel();

    int T = 1;
    cin >> T;

    while (T--)
        solve();

    return 0;
}
