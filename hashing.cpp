const int NN = 1000'001;
typedef __int128 HASH;
HASH p[NN],h[NN];
constexpr HASH M = 100000000000000003;
mt19937 gen(__builtin_ia32_rdtsc());
uniform_int_distribution<int> dist(256, M-1);
string s;
void pre(string &s){
    p[0]=1; p[1] = dist(gen);
    for(int i=0;i<(int)s.size();++i){
        p[i+1] = p[i]*p[1]%M;
        h[i+1] = (h[i]*p[1]+s[i])%M;
    }
}

HASH sub_hash(int l, int r){
    return (h[r]-p[r-l]*h[l]%M+M)%M;
}