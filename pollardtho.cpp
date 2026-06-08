using u128 = __uint128_t;
using ll = long long;

// ---------- modular arithmetic ----------
ll mod_mul(ll a, ll b, ll mod) {
    return (u128)a * b % mod;
}

ll mod_pow(ll a, ll d, ll mod) {
    ll res = 1;
    while (d) {
        if (d & 1) res = mod_mul(res, a, mod);
        a = mod_mul(a, a, mod);
        d >>= 1;
    }
    return res;
}

// ---------- Miller Rabin ----------
bool isPrime(ll n) {
    if (n < 2) return false;
    for (ll p : {2, 3, 5, 7, 11, 13, 17, 19, 23, 29})
        if (n % p == 0) return n == p;

    ll d = n - 1, s = 0;
    while ((d & 1) == 0) d >>= 1, s++;

    // deterministic bases for 64-bit
    for (ll a : {2, 325, 9375, 28178, 450775, 9780504, 1795265022}) {
        if (a % n == 0) continue;
        ll x = mod_pow(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool composite = true;
        for (int r = 1; r < s; r++) {
            x = mod_mul(x, x, n);
            if (x == n - 1) {
                composite = false;
                break;
            }
        }
        if (composite) return false;
    }
    return true;
}

// ---------- Pollard Rho ----------
mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());

ll f(ll x, ll c, ll mod) {
    return (mod_mul(x, x, mod) + c) % mod;
}

ll pollard(ll n) {
    if (n % 2 == 0) return 2;
    if (n % 3 == 0) return 3;

    ll x = uniform_int_distribution<ll>(2, n - 2)(rng);
    ll y = x;
    ll c = uniform_int_distribution<ll>(1, n - 1)(rng);
    ll d = 1;

    while (d == 1) {
        x = f(x, c, n);
        y = f(y, c, n);
        y = f(y, c, n);
        d = gcd(llabs(x - y), n);
    }
    return (d == n ? pollard(n) : d);
}

// ---------- factorization ----------
void factor(ll n, map<ll,int> &mp) {
    if (n == 1) return;
    if (isPrime(n)) {
        mp[n]++;
    } else {
        ll d = pollard(n);
        factor(d, mp);
        factor(n / d, mp);
    }
}

// ---------- generate all divisors ----------
void gen_divisors(vector<pair<ll,int>> &fac, int idx, ll cur, vector<ll> &divs) {
    if (idx == (int)fac.size()) {
        divs.push_back(cur);
        return;
    }
    auto [p, cnt] = fac[idx];
    for (int i = 0; i <= cnt; i++) {
        gen_divisors(fac, idx + 1, cur, divs);
        cur *= p;
    }
}

vector<ll> get_divisors(ll n) {
    map<ll,int> mp;
    factor(n, mp);

    vector<pair<ll,int>> fac(mp.begin(), mp.end());
    vector<ll> divs;
    gen_divisors(fac, 0, 1, divs);

    sort(divs.begin(), divs.end());
    return divs;
}