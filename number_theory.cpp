
namespace nt {

using ll = long long;

// --------------------------------------------------
// Miller–Rabin for 64-bit integers
// --------------------------------------------------
ll modmul(ll a, ll b, ll m=MOD){
    __int128 r = (__int128)a * b;
    return (ll)(r % m);
}

ll power(ll a, ll d, ll m=MOD){
    ll r = 1;
    while(d){
        if(d & 1) r = modmul(r, a, m);
        a = modmul(a, a, m);
        d >>= 1;
    }
    return r;
}

bool isprime(ll n){
    if(n < 2) return false;
    for(ll p : {2,3,5,7,11,13,17,19,23,29,31,37})
        if(n == p) return true;
        else if(n % p == 0) return false;

    ll d = n - 1, s = 0;
    while((d & 1) == 0) d >>= 1, s++;

    for(ll a : {2,325,9375,28178,450775,9780504,1795265022}){
        if(a % n == 0) continue;
        ll x = power(a, d, n);
        if(x == 1 || x == n-1) continue;
        bool composite = true;
        for(int r = 1; r < s; r++){
            x = modmul(x, x, n);
            if(x == n-1){
                composite = false;
                break;
            }
        }
        if(composite) return false;
    }
    return true;
}
// --------------------------------------------------
// Fast power + modular inverse (prime mod)
// --------------------------------------------------
ll modinv(ll a, ll m = MOD){
    return power(a, m - 2, m);
}

// --------------------------------------------------
// Sieve + SPF
// --------------------------------------------------
const int MAXN = 2000000;
int spf[MAXN + 1];
vector<int> primes;

void sieve(){
    for(int i = 2; i <= MAXN; i++){
        if(!spf[i]) spf[i] = i, primes.push_back(i);
        for(int p : primes){
            if(p > spf[i] || 1LL * p * i > MAXN) break;
            spf[p * i] = p;
        }
    }
}

// --------------------------------------------------
// Factorization using SPF
// --------------------------------------------------
vector<pair<ll,int>> factorize(ll x){
    vector<pair<ll,int>> f;
    while(x > 1){
        ll p = spf[x], c = 0;
        while(x % p == 0) x /= p, c++;
        f.push_back({p, c});
    }
    return f;
}

// --------------------------------------------------
// Euler Totient
// --------------------------------------------------
ll phi(ll n){
    ll r = n;
    for(auto &pr : factorize(n)){
        r -= r / pr.first;
    }
    return r;
}

// --------------------------------------------------
// nCr using factorials
// --------------------------------------------------
vector<ll> fact, invfact;

void init_factorials(int n){
    fact.assign(n+1, 1);
    invfact.assign(n+1, 1);

    for(int i = 1; i <= n; i++)
        fact[i] = fact[i-1] * i % MOD;

    invfact[n] = modinv(fact[n]);
    for(int i = n-1; i >= 0; i--)
        invfact[i] = invfact[i+1] * (i+1) % MOD;
}

ll nCr(int n, int r){
    if(r < 0 || r > n) return 0;
    return fact[n] * invfact[r] % MOD * invfact[n-r] % MOD;
}

// --------------------------------------------------
// Extended GCD + inverse for non-prime mod
// --------------------------------------------------
ll ext_gcd(ll a, ll b, ll &x, ll &y){
    if(b == 0){ x = 1; y = 0; return a; }
    ll x1, y1;
    ll g = ext_gcd(b, a % b, x1, y1);
    x = y1;
    y = x1 - (a / b) * y1;
    return g;
}

ll modinv_general(ll a, ll m){
    ll x, y;
    ll g = ext_gcd(a, m, x, y);
    if(g != 1) return -1;
    x %= m;
    if(x < 0) x += m;
    return x;
}


} // namespace nt
using namespace nt;