#include <bits/stdc++.h>
using namespace std;

// Aliases
using ll = long long;
using ld = long double;
using pii = pair<int, int>;
using pll = pair<ll, ll>;
using vi = vector<int>;
using vll = vector<ll>;

// Constants
const int MOD = 1e9 + 7;
const int INF = 1e9;
const ll LINF = 1e18;

// Macros
#define fastio() ios::sync_with_stdio(0); cin.tie(0); cout.tie(0)
#define all(x) (x).begin(), (x).end()
#define rall(x) (x).rbegin(), (x).rend()
#define pb push_back
#define eb emplace_back
#define mp make_pair
#define ff first
#define ss second
#define sz(x) (int)(x).size()
#define yes cout << "YES\n"
#define no cout << "NO\n"

// Debugging
#ifdef LOCAL
    #define debug(x) cerr << #x << " = " << (x) << '\n';
#else
    #define debug(x)
#endif

// Fast I/O
void OmPatel() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
}

// Power Function (Modular Exponentiation)
ll power(ll a, ll b, ll m = MOD) {
    ll res = 1;
    while (b) {
        if (b & 1) res = res * a % m;
        a = a * a % m;
        b >>= 1;
    }
    return res;
}

// GCD
ll gcd(ll a, ll b) { return b ? gcd(b, a % b) : a; }

// LCM
ll lcm(ll a, ll b) { return (a / gcd(a, b)) * b; }

// Inverse Modulo
ll mod_inv(ll a, ll m = MOD) { return power(a, m - 2, m); }

// Prime Check
bool is_prime(ll n) {
    if (n < 2) return false;
    for (ll i = 2; i * i <= n; i++)
        if (n % i == 0) return false;
    return true;
}

// Sieve of Eratosthenes
vector<int> sieve(int n) {
    vector<int> primes(n + 1, 1), res;
    primes[0] = primes[1] = 0;
    for (int i = 2; i * i <= n; i++) {
        if (primes[i]) {
            for (int j = i * i; j <= n; j += i) primes[j] = 0;
        }
    }
    for (int i = 2; i <= n; i++)
        if (primes[i]) res.pb(i);
    return res;
}

// Sorting
template <typename T> void sort_asc(vector<T> &v) { sort(all(v)); }
template <typename T> void sort_desc(vector<T> &v) { sort(rall(v)); }

// Main Function
void solve() {
    
}

int main() {
    OmPatel();
    int t = 1;
    cin >> t;
    while (t--) solve();
    return 0;
}
