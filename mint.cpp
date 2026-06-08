struct mint{
    int x;
 
    mint (){ x = 0;}
    mint (int32_t xx){ x = xx % mod; if (x < 0) x += mod;}
    mint (long long xx){ x = xx % mod; if (x < 0) x += mod;}
 
    int val(){
        return x;
    }
    mint &operator++(){
        x++;
        if (x == mod) x = 0;
        return *this;
    }
    mint &operator--(){
        if (x == 0) x = mod;
        x--;
        return *this;
    }
    mint operator++(int32_t){
        mint result = *this;
        ++*this;
        return result;
    }
    
    mint operator--(int32_t){
        mint result = *this;
        --*this;
        return result;
    }
    mint& operator+=(const mint &b){
        x += b.x;
        if (x >= mod) x -= mod;
        return *this;
    }
    mint& operator-=(const mint &b){
        x -= b.x;
        if (x < 0) x += mod;
        return *this;
    }
    mint& operator*=(const mint &b){
        long long z = x;
        z *= b.x;
        z %= mod;
        x = (int)z;
        return *this;
    }
    mint operator+() const {
        return *this;
    }
    mint operator-() const {
        return mint() - *this;
    }
    mint operator/=(const mint &b){
        return *this = *this * b.inv();
    }
    mint power(long long n) const {
        mint ok = *this, r = 1;
        while (n){
            if (n & 1){
                r *= ok;
            }
            ok *= ok;
            n >>= 1;
        }
        return r;
    }
    mint inv() const {
        return power(mod - 2);
    }
    friend mint operator+(const mint& a, const mint& b){ return mint(a) += b;}
    friend mint operator-(const mint& a, const mint& b){ return mint(a) -= b;}
    friend mint operator*(const mint& a, const mint& b){ return mint(a) *= b;}
    friend mint operator/(const mint& a, const mint& b){ return mint(a) /= b;}
    friend bool operator==(const mint& a, const mint& b){ return a.x == b.x;}
    friend bool operator!=(const mint& a, const mint& b){ return a.x != b.x;}
    mint power(mint a, long long n){
        return a.power(n);
    }
    friend ostream &operator<<(ostream &os, const mint &m) {
        os << m.x;
        return os;
    }
    explicit operator bool() const {
        return x != 0;
    }
};
 
struct factorials{
    int n;
    vector <mint> ff, iff;
    
    factorials(int nn){
        n = nn;
        ff.resize(n + 1);
        iff.resize(n + 1);
        
        ff[0] = 1;
        for (int i = 1; i <= n; i++){
            ff[i] = ff[i - 1] * i;
        }
        
        iff[n] = ff[n].inv();
        for (int i = n - 1; i >= 0; i--){
            iff[i] = iff[i + 1] * (i + 1);
        }
    }
    
    mint C(int n, int r){
        if (n == r) return mint(1);
        if (n < 0 || r < 0 || r > n) return mint(0);
        return ff[n] * iff[r] * iff[n - r];
    }
    
    mint P(int n, int r){
        if (n < 0 || r < 0 || r > n) return mint(0);
        return ff[n] * iff[n - r];
    }
    
    mint solutions(int n, int r){
        return C(n + r - 1, n - 1);
    }
    
    mint catalan(int n){
        return ff[2 * n] * iff[n] * iff[n + 1];
    }
};
 
const int PRECOMP = 3e6 + 69;
factorials F(PRECOMP);