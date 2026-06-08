struct Fenwick{
    int n;
    vector<int> bit;

    Fenwick(int n){
        this->n = n;
        bit.assign(n + 1, 0);
    }

    void add(int i, int val){
        for(; i <= n; i += i & -i)
            bit[i] += val;
    }

    int get(int i){
        int s = 0;
        for(; i > 0; i -= i & -i)
            s += bit[i];
        return s;
    }

    int Kth(int k){
        int pos = 0, sum = 0;
        for(int i = __lg(n); i >= 0; --i){
            int pw = (1 << i);
            if(pos + pw <= n && sum + bit[pos + pw] < k){
                pos += pw;
                sum += bit[pos];
            }
        }
        return 1 + pos;
    }
};