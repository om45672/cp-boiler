inline long long hilbertOrder(int x, int y, int pw = 21, int rot = 0) {
    if (pw == 0) return 0;

    int h = 1 << (pw - 1);
    int seg;

    if (x < h) {
        seg = (y < h ? 0 : 3);
    } else {
        seg = (y < h ? 1 : 2);
    }

    seg = (seg + rot) & 3;

    static const int rotateDelta[4] = {3, 0, 0, 1};
    int nrot = (rot + rotateDelta[seg]) & 3;

    int nx = x & (x ^ h);
    int ny = y & (y ^ h);

    long long sub = hilbertOrder(nx, ny, pw - 1, nrot);
    long long add = (long long)seg << (2 * pw - 2);

    if (seg == 1 || seg == 2)
        return add + sub;
    else
        return add + ((1LL << (2 * pw - 2)) - sub - 1);
}