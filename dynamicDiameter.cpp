#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
typedef pair<int, int> pii;
typedef pair<ll, ll> pll;
typedef vector<int> vi;
typedef vector<pii> vpii;
typedef vector<pll> vpll;
typedef long double ld;

/*author :: humayan kabir,sust*/

int const MAX = 100000 + 7;
int const MAXM = 10000 + 7;
int const LOGN = 17;

int n, ara[MAX];
vpii e[MAX];
vi divisors[MAXM];
ll ans;

void init()
{
  for(int i = 1; i<MAXM; i++) {
    for(int j = i; j<MAXM; j+=i) {
      divisors[j].push_back(i);
    }
  }
}

namespace Lca {
  int lca[MAX][LOGN];
  int inTime[MAX];
  int outTime[MAX];
  ll h[MAX];
  int Timer;

  void dfs(int u, int p, int dist) {
    h[u] = dist;
    inTime[u] = ++Timer;
    for(int i = 0; i<LOGN; i++) {
      lca[u][i] = -1;
    }
    lca[u][0] = p;
    for(int i = 1; i<LOGN; i++) {
      int pp = lca[u][i - 1];
      if(pp == -1) continue;
      lca[u][i] = lca[pp][i - 1];
    }
    for(auto v : e[u]) {
      if(v.first == p) continue;
      dfs(v.first, u, dist + v.second);
    }
    outTime[u] = Timer;
  }

  void build() {
    Timer = 0;
    dfs(1, -1, 0);
  }

  int isancestor(int a, int b) {
    return (inTime[a] <= inTime[b] && outTime[a] >= outTime[b]);
  }

  int getlca(int a,int b)
  {
    if(isancestor(a,b)) return a;
    if(isancestor(b,a)) return b;
    for(int i = LOGN - 1; i>=0; i--)
    {
      if(lca[a][i] == -1 or isancestor(lca[a][i], b)) continue;
      a = lca[a][i];
    }
    return lca[a][0];
  }

  ll getdist(int a,int b) {
    return h[a] + h[b] - 2 * h[getlca(a,b)];
  }
};

vpii edge[MAX];

void pushedge(int x, int y)
{
  if(ara[x] > ara[y]) swap(x,y);
  int g = __gcd(ara[x], ara[y]);
  for(int d : divisors[g]) {
    edge[d].push_back(make_pair(x, y));
  }
}

namespace Dsu {
  int par[MAX];
  ll diameter[MAX];
  int furthestA[MAX];
  int furthestB[MAX]; //here furthestA & B represent
  int sizeT[MAX];    //diameter two point of tree root at varible x

  void init(int u) {
    par[u] = u;
    diameter[u] = 0;
    furthestA[u] = u;
    furthestB[u] = u;
    sizeT[u] = 1;
  }

  int find(int u)
  {
    if(par[u] == u) return u;
    return par[u] = find(par[u]);
  }

  ll unionTree(int a,int b)
  {
    a = find(a);
    b = find(b);
    if(sizeT[a] < sizeT[b]) swap(a,b);
    sizeT[a] += sizeT[b];
    int a1 = Dsu::furthestA[a];
    int a2 = Dsu::furthestB[a];
    int b1 = Dsu::furthestA[b];
    int b2 = Dsu::furthestB[b];
    if(diameter[a] < Lca::getdist(a1,b1)) {
      Dsu::diameter[a] = Lca::getdist(a1,b1);
      furthestA[a] = a1;
      furthestB[a] = b1;
    }
    if(diameter[a] < Lca::getdist(a1,b2)) {
      Dsu::diameter[a] = Lca::getdist(a1,b2);
      furthestA[a] = a1;
      furthestB[a] = b2;
    }
    if(diameter[a] < Lca::getdist(a2,b1)) {
      Dsu::diameter[a] = Lca::getdist(a2,b1);
      furthestA[a] = a2;
      furthestB[a] = b1;
    }
    if(diameter[a] < Lca::getdist(a2,b2)) {
      Dsu::diameter[a] = Lca::getdist(a2,b2);
      furthestA[a] = a2;
      furthestB[a] = b2;
    }
    if(diameter[a] < Lca::getdist(b1,b2)) {
      Dsu::diameter[a] = Lca::getdist(b1,b2);
      furthestA[a] = b1;
      furthestB[a] = b2;
    }
    par[b] = a;
    return diameter[a];
  }

}

bool cmp(pii a, pii b) {
  return ara[a.first] < ara[b.first];
}

void clear() {
  for(int i = 1; i<=n; i++) {
    e[i].clear();
  }
  for(int i = 1; i<MAXM; i++) {
    edge[i].clear();
  }
}
void solve() {
  Lca::build();
  for(int i = 1; i<=n; i++) {
    for(pii v : e[i]) {
      if(i < v.first) pushedge(i, v.first);
    }
  }
  ans = 0;
  for(int i = 1; i<MAXM; i++) {
    sort(edge[i].begin(), edge[i].end(), cmp);
    for(pii v : edge[i]) {
      Dsu::init(v.first);
      Dsu::init(v.second);
    }
    ll cur = 0;
    for(int j = edge[i].size() - 1; j>=0; j--) {
      ll d = Dsu::unionTree(edge[i][j].first, edge[i][j].second);
      cur = max(cur, d * ara[edge[i][j].first]);
    }
    ans = max(ans, cur * i);
  }
  printf("%lld\n", ans);
  clear();
}
int main(int argc, char const *argv[]) {
  init();

  int t;
  scanf("%d", &t);
  while(t--)
  {
    scanf("%d", &n);
    for(int i = 1; i<=n; i++) scanf("%d", &ara[i]);
    for(int i = 1; i<n; i++) {
      int u, v, w;
      scanf("%d %d %d", &u, &v, &w);
      e[u].push_back(make_pair(v, w));
      e[v].push_back(make_pair(u, w));
    }
    solve();
  }
  return 0;
}