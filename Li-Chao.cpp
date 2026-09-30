// Dynamic Li Chao Tree
// Default: minimum query.
// Huge implicit domain: [-(2^62), 2^62).
// Supports:
//   add_line(m,b)              -> whole domain
//   add_segment(l,r,m,b)       -> only x in [l,r)
//
// For maximum: use LiChao<false>.
//
// __int128 is used for m*x+b to avoid overflow during comparisons.

template<bool isMin = true>
struct LiChao {
    static constexpr long long XL = -(1LL << 62);
    static constexpr long long XR =  (1LL << 62);
    static constexpr long long INF = 4'000'000'000'000'000'000LL;

    struct Line {
        long long m, b;

        Line(long long _m = 0,
             long long _b = isMin ? INF : -INF)
            : m(_m), b(_b) {}

        long long value(long long x) const {
            __int128 y = (__int128)m * x + b;
            if (y > INF) return INF;
            if (y < -INF) return -INF;
            return (long long)y;
        }
    };

    struct Node {
        Line line;
        Node *l = nullptr, *r = nullptr;

        Node(Line x) : line(x) {}
    };

    Node* root = nullptr;

    static bool better(long long a, long long b) {
        if constexpr (isMin) return a < b;
        else return a > b;
    }

    static long long combine(long long a, long long b) {
        if constexpr (isMin) return min(a, b);
        else return max(a, b);
    }

    void add_line(long long m, long long b) {
        add_line(Line(m, b));
    }

    void add_line(Line nw) {
        add_line(root, XL, XR, nw);
    }

    void add_line(Node*& node, long long l, long long r, Line nw) {
        if (!node) {
            node = new Node(nw);
            return;
        }

        long long mid = l + (r - l) / 2;

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

    void add_segment(long long ql, long long qr,
                     long long m, long long b) {
        add_segment(ql, qr, Line(m, b));
    }

    void add_segment(long long ql, long long qr, Line nw) {
        ql = max(ql, XL);
        qr = min(qr, XR);

        if (ql >= qr)
            return;

        add_segment(root, XL, XR, ql, qr, nw);
    }

    void add_segment(Node*& node, long long l, long long r,
                     long long ql, long long qr, Line nw) {
        if (qr <= l || r <= ql)
            return;

        if (ql <= l && r <= qr) {
            add_line(node, l, r, nw);
            return;
        }

        if (!node)
            node = new Node(Line());

        long long mid = l + (r - l) / 2;

        add_segment(node->l, l, mid, ql, qr, nw);
        add_segment(node->r, mid, r, ql, qr, nw);
    }

    long long query(long long x) const {
        if (x < XL || x >= XR)
            return isMin ? INF : -INF;

        return query(root, XL, XR, x);
    }

    long long query(Node* node, long long l, long long r,
                    long long x) const {
        if (!node)
            return isMin ? INF : -INF;

        long long ans = node->line.value(x);

        if (r - l <= 1)
            return ans;

        long long mid = l + (r - l) / 2;

        if (x < mid)
            return combine(ans, query(node->l, l, mid, x));

        return combine(ans, query(node->r, mid, r, x));
    }
};