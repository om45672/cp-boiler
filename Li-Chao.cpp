struct LiChao {
    struct Line {
        long long m, b; // y = m*x + b
        Line(long long _m = 0, long long _b = (long long)4e18) : m(_m), b(_b) {}
        long long value(long long x) const {
            return m * x + b;
        }
    };

    struct Node {
        Line line;
        Node *l, *r;
        Node(Line v) : line(v), l(nullptr), r(nullptr) {}
    };

    const long long INF = (long long)4e18;
    long long L, R; // x-domain
    Node* root;

    LiChao(long long _L, long long _R) : L(_L), R(_R) {
        root = nullptr;
    }

    void add_line(Line newLine) {
        add_line(root, L, R, newLine);
    }

    void add_line(Node* &node, long long l, long long r, Line newLine) {
        if (!node) {
            node = new Node(newLine);
            return;
        }

        long long mid = (l + r) >> 1;

        bool left = newLine.value(l) < node->line.value(l);
        bool m = newLine.value(mid) < node->line.value(mid);

        if (m) {
            swap(node->line, newLine);
        }

        if (r - l == 1) return;

        if (left != m) {
            add_line(node->l, l, mid, newLine);
        } else {
            add_line(node->r, mid, r, newLine);
        }
    }

    long long query(long long x) const {
        return query(root, L, R, x);
    }

    long long query(Node* node, long long l, long long r, long long x) const {
        if (!node) return INF;

        long long res = node->line.value(x);
        if (r - l == 1) return res;

        long long mid = (l + r) >> 1;
        if (x < mid && node->l) {
            res = min(res, query(node->l, l, mid, x));
        } else if (x >= mid && node->r) {
            res = min(res, query(node->r, mid, r, x));
        }
        return res;
    }
};