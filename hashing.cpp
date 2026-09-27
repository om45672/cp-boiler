struct Hash {
    using ull = unsigned long long;
    static const int N = 1e6 + 5;
    ull p[N], h[N];
    ull base = chrono::steady_clock::now().time_since_epoch().count() | 1;

    Hash(string &s) {
        p[0] = 1;
        for(int i = 0; i < s.size(); i++) {
            p[i+1] = p[i] * base;
            h[i+1] = h[i] * base + s[i];
        }
    }

    ull get(int l, int r) { // [l,r)
        return h[r] - h[l] * p[r-l];
    }
};
