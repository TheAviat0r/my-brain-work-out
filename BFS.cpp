#include <iostream>
#include <queue>
#include <vector>
using namespace std;
int main()
{
    int n,s;
    cin>>n>>s;
    vector < vector <int> > g(n);
    queue<int> q;
    q.push(s);
    vector<bool> used (n);
    vector<int> d(n), p(n);
    used[s]=true;
    p[s]=-1;
    while (!q.empty())
    {
          int v =q.front();
          q.pop();
          for (size_t j; j<g[v].size(); ++j)
          {
              int t0=g[v][j];
              if (!used[t0])
              {
                            used[t0]=true;
                            q.push(t0);
                            d[t0]=d[v]+1;
                            p[t0]=v;
              }
          }
    }
}
    
