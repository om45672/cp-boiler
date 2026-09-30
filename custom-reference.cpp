#pragma GCC optimize("O3,unroll-loops,no-stack-protector,fast-math")
#include <bits/stdc++.h>
using namespace std;

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

const int MOD = 1e9 + 7;
const int mod = 998244353;
const int INF = 4e18;

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
        a = find(a);
        b = find(b);

        if (a == b) return false;
        if (sz[a] < sz[b]) swap(a, b);

        parent[b] = a;
        sz[a] += sz[b];

        return true;
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

    int time() {
        return sz(st);
    }

    bool unite(int a, int b) {
        a = find(a);
        b = find(b);

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
// 3. Fenwick Tree
// 1-indexed
// ============================================================

struct Fenwick {
    int n;
    vector<ll> bit;

    Fenwick(int n = 0) : n(n), bit(n + 1, 0) {}

    void add(int i, ll val) {
        for (; i <= n; i += i & -i)
            bit[i] += val;
    }

    ll get(int i) {
        ll s = 0;

        for (; i > 0; i -= i & -i)
            s += bit[i];

        return s;
    }

    ll query(int l, int r) {
        return get(r) - get(l - 1);
    }

    // smallest index whose prefix sum >= k
    int Kth(ll k) {
        int pos = 0;
        ll sum = 0;

        for (int i = __lg(n); i >= 0; --i) {
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
// 4. Segment Tree
// [l,r] query
// Current operation = minimum
// ============================================================

struct SegTree {
    int n;
    vector<ll> t;

    static ll merge(ll a, ll b) {
        return min(a, b);
    }

    SegTree(int n = 0, ll default_val = INF) : n(n) {
        t.assign(2 * n, default_val);
    }

    SegTree(const vector<ll>& a) {
        n = sz(a);
        t.assign(2 * n, INF);

        for (int i = 0; i < n; ++i)
            t[n + i] = a[i];

        for (int i = n - 1; i; --i)
            t[i] = merge(t[i << 1], t[i << 1 | 1]);
    }

    void modify(int pos, ll val) {
        pos += n;
        t[pos] = val;

        for (pos >>= 1; pos; pos >>= 1)
            t[pos] = merge(t[pos << 1], t[pos << 1 | 1]);
    }

    ll query(int l, int r) {
        ll resL = INF, resR = INF;

        for (l += n, r += n; l <= r; l >>= 1, r >>= 1) {
            if (l & 1) resL = merge(resL, t[l++]);
            if (!(r & 1)) resR = merge(t[r--], resR);
        }

        return merge(resL, resR);
    }
};


// ============================================================
// 5. Dynamic Li Chao Tree
// Minimum of lines y = mx + b
//
// Huge implicit x-domain.
// Only creates nodes that are actually needed.
//
// To make it maximum instead:
//   - change comparisons < to >
//   - return -INF for empty nodes
// ============================================================

struct LiChao {
    struct Line {
        ll m, b;

        Line(ll _m = 0, ll _b = INF) : m(_m), b(_b) {}

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

    // Covers practically every signed 64-bit query coordinate.
    static constexpr ll X_LOW = -(1LL << 62);
    static constexpr ll X_HIGH = (1LL << 62);

    Node* root = nullptr;

    void add_line(Line nw) {
        add_line(root, X_LOW, X_HIGH, nw);
    }

    void add_line(Node*& node, ll l, ll r, Line nw) {
        if (!node) {
            node = new Node(nw);
            return;
        }

        ll mid = l + (r - l) / 2;

        bool lef = nw.value(l) < node->line.value(l);
        bool mid_better = nw.value(mid) < node->line.value(mid);

        if (mid_better)
            swap(node->line, nw);

        if (r - l <= 1)
            return;

        if (lef != mid_better)
            add_line(node->l, l, mid, nw);
        else
            add_line(node->r, mid, r, nw);
    }

    ll query(ll x) const {
        return query(root, X_LOW, X_HIGH, x);
    }

    ll query(Node* node, ll l, ll r, ll x) const {
        if (!node)
            return INF;

        ll ans = node->line.value(x);

        if (r - l <= 1)
            return ans;

        ll mid = l + (r - l) / 2;

        if (x < mid)
            ans = min(ans, query(node->l, l, mid, x));
        else
            ans = min(ans, query(node->r, mid, r, x));

        return ans;
    }
};


// ============================================================
// 6. Convex Hull Trick
// Monotonic slopes + monotonic query x
// Minimum hull
// ============================================================

struct CHT {
    struct Line {
        ll m, b;

        Line(ll m = 0, ll b = 0) : m(m), b(b) {}

        ll value(ll x) const {
            return m * x + b;
        }
    };

    deque<pair<Line, ll>> dq;

    // First x where y starts beating x.
    ll intersect(Line x, Line y) {
        return (y.b - x.b + x.m - y.m - 1) / (x.m - y.m);
    }

    void add(ll m, ll b) {
        Line nw(m, b);

        while (sz(dq) > 1 &&
               dq.back().ss >= intersect(dq.back().ff, nw))
            dq.pop_back();

        if (dq.empty()) {
            dq.pb({nw, -INF});
        } else {
            dq.pb({nw, intersect(dq.back().ff, nw)});
        }
    }

    ll query(ll x) {
        while (sz(dq) > 1 && dq[1].ss <= x)
            dq.pop_front();

        return dq.front().ff.value(x);
    }
};


// ============================================================
// 7. String Hashing
// ============================================================

struct Hash {
    using ull = unsigned long long;

    vector<ull> p, h;
    ull base;

    Hash(string& s) {
        int n = sz(s);

        base = chrono::steady_clock::now()
                   .time_since_epoch()
                   .count() | 1ULL;

        p.resize(n + 1);
        h.resize(n + 1);

        p[0] = 1;

        for (int i = 0; i < n; ++i) {
            p[i + 1] = p[i] * base;
            h[i + 1] = h[i] * base + s[i];
        }
    }

    // hash of s[l..r)
    ull get(int l, int r) {
        return h[r] - h[l] * p[r - l];
    }
};


// ============================================================
// 8. Matrix
// ============================================================

struct Matrix {
    int n, m;
    vector<vector<int>> mat;

    Matrix(int n, int m) : n(n), m(m), mat(n, vector<int>(m, 0)) {}

    static Matrix identity(int n) {
        Matrix I(n, n);

        rep(i, 0, n - 1)
            I.mat[i][i] = 1;

        return I;
    }

    Matrix operator*(const Matrix& other) const {
        assert(m == other.n);

        Matrix res(n, other.m);

        fr(i, n) {
            fr(k, m) {
                if (mat[i][k] == 0)
                    continue;

                fr(j, other.m) {
                    res.mat[i][j] =
                        (res.mat[i][j] +
                         1LL * mat[i][k] * other.mat[k][j]) % MOD;
                }
            }
        }

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
// 9. Min Stack
// ============================================================

struct MinStack {
    stack<pair<ll,ll>> st;

    void push(ll x) {
        ll mn = st.empty() ? x : min(x, st.top().ss);
        st.push({x, mn});
    }

    void pop() {
        st.pop();
    }

    ll top() {
        return st.top().ff;
    }

    ll getMin() {
        return st.top().ss;
    }

    bool empty() {
        return st.empty();
    }
};


// ============================================================
// 10. Template
// ============================================================

void solve() {

}

signed main() {
    OmPatel();

    int _ = 1;
    cin >> _;

    while (_-- > 0)
        solve();

    return 0;
}
