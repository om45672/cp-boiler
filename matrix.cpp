#define ll long long
#define rep(i,a,b) for(int i=a;i<=b;++i)
const int MOD = 1e9+7;

struct Matrix{
    int n,m;
    vector<vector<int>> mat;
    Matrix(int n,int m): n(n), m(m), mat(n,vector<int>(m,0)){}

    static Matrix identity(int n){
        Matrix I(n,n);
        rep(i,0,n-1) I.mat[i][i] = 1;
        return I;
    }

    Matrix operator*(const Matrix& other) const{
        assert(m==other.n);
        Matrix product(n,other.m);
        rep(i,0,n-1) rep(j,0,other.m-1) rep(k,0,m-1)
            product.mat[i][j] = (product.mat[i][j] + 1LL * mat[i][k] * other.mat[k][j]) % MOD;
        return product;
    }

    Matrix pow(ll expo) const {
        assert(n==m);
        Matrix res = identity(n);
        Matrix base = *this;
        while(expo){
            if(expo & 1) res = res * base;
            base = base * base;
            expo >>= 1;
        }
        return res;
    }
};

// Usage Example:
// int main() {
//     Matrix A(2, 2);
//     A.mat = {{1, 1}, {1, 0}};
//     ll n = 10;
//     Matrix B = A.pow(n);
//     rep(i,0,1){ rep(j,0,1) cout << B.mat[i][j] << ' '; cout << endl; }
// }