struct SegTree {
    int n;
    vector<long long> t;

    static long long merge(long long a, long long b) {
        return min(a, b);
    }

    SegTree(int n, long long default_val = LLONG_MAX) : n(n) {
        t.assign(2 * n, default_val);
    }

    SegTree(const vector<long long>& a) {
        n = a.size();
        t.resize(2 * n);
        for (int i = 0; i < n; i++) t[n + i] = a[i];
        for (int i = n - 1; i > 0; i--)
            t[i] = merge(t[i << 1], t[i << 1 | 1]);
    }

    void modify(int pos, long long val) {
        pos += n;
        t[pos] = val;
        for (pos >>= 1; pos > 0; pos >>= 1)
            t[pos] = merge(t[pos << 1], t[pos << 1 | 1]);
    }

    long long query(int l, int r) {
        long long resL = LLONG_MAX, resR = LLONG_MAX;
        l += n; r += n;
        while (l <= r) {
            if (l & 1) resL = merge(resL, t[l++]);
            if (!(r & 1)) resR = merge(t[r--], resR);
            l >>= 1; r >>= 1;
        }
        return merge(resL, resR);
    }
};