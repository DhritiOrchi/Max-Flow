#include <bits/stdc++.h>
using namespace std;
const long long  mod=1e9+7;

struct MaxFlow {
    long long  n;
    vector<vector<long long>> cap, flow, adj;

    MaxFlow(long long  n) : n(n) {
        cap.assign(n, vector<long long >(n, 0));
        flow.assign(n, vector<long long >(n, 0));
        adj.resize(n);
    }

    void addEdge(long long  u, long long  v, long long  c) {
        if(cap[u][v]==0)
        {
            adj[u].push_back(v);
            adj[v].push_back(u);
        }
        cap[u][v] +=c;
    }

    long long  bfs(long long  s, long long  t, vector<long long >& parent) {
        fill(parent.begin(), parent.end(), -1);
        parent[s] = -2;
        queue<pair<long long ,long long >> q;
        q.push({s, LLONG_MAX});

        while (!q.empty()) {
            long long  u = q.front().first;
            long long  f = q.front().second;
            q.pop();

            for (long long  v : adj[u]) {
                if (parent[v] == -1 && cap[u][v] - flow[u][v] > 0) {
                    parent[v] = u;
                    long long  new_flow = min(f, cap[u][v] - flow[u][v]);
                    if (v == t) return new_flow;
                    q.push({v, new_flow});
                }
            }
        }
        return 0;
    }

    long long maxFlow(long long s, long long t) {
        long long total = 0;
        vector<long long> parent(n);

        while (true) {
            long long pushed = bfs(s, t, parent);
            if (pushed == 0) break;

            total += pushed;
            long long cur = t;
            while (cur != s) {
                long long prev = parent[cur];
                flow[prev][cur] += pushed;
                flow[cur][prev] -= pushed;
                cur = prev;
            }
        }
        return total;
    }


    
    long long  bipartiteMatching(long long  L, long long  R, vector<pair<long long ,long long >>& edges) {
        
        long long  S = 0;
        long long  T = L + R + 1;

        // Source → Left
        for (long long  i = 1; i <= L; i++)
            addEdge(S, i, 1);

        // Left → Right
        for (auto &e : edges) {
            long long  u = e.first;   // 1..L
            long long  v = e.second;  // 1..R
            addEdge(u, L + v, 1);
        }

        // Right → Sink
        for (long long  i = 1; i <= R; i++)
            addEdge(L + i, T, 1);

        return maxFlow(S, T);
    }
    vector<bool> minCut(long long  S)
    {// first call the max flow algorithm
        vector<bool>visited(n,false);
        queue<long long >q;
        q.push(S);
        visited[S]=true;
        while (!q.empty())
        {
            long long  node=q.front();
            q.pop();
            for(auto it:adj[node])
            {
                if(!visited[it] && cap[node][it]-flow[node][it]>0)
                {
                    visited[it]=true;
                    q.push(it);

                }
            }
            /* code */
        }
        return visited;// where 1-> s side , 0-> t side
        
    }
    //Edge-disjolong long  paths between s and t are equivalent
    // to the maximum flow in a graph where all edges have capacity 1.
    vector<vector<long long >> edgeDisjointPaths(long long  s,long long  t)
    {
        long long  max_paths=maxFlow(s,t);
        vector<vector<long long >>paths;
        vector<vector<long long >>tempFlow=flow;
        for(long long  p=0;p<max_paths;p++)
        {
            vector<long long >path;
            long long  curr=s;
            path.push_back(s);
            while (curr!=t)
            {
                for(long long  next:adj[curr])
                {
                    if(tempFlow[curr][next]>0)
                    {
                        path.push_back(next);
                        tempFlow[curr][next]--;
                        tempFlow[next][curr]++;
                        curr=next;
                        break;
                    }
                }

                /* code */
            }
            paths.push_back(path);
            
        }
        return paths;
    }
    vector<pair<long long ,long long>>  howManyMinimumEdgeCut(long long S,long long T,vector<pair<long long ,long long>>&edges)
    {
        vector<pair<long long ,long long>>ans;
        long long result=maxFlow(S,T);
        vector<bool>visited=minCut(S);
        long long count=0;
        for(auto it:edges)
        {
            if((visited[it.first]^visited[it.second]))
            ans.push_back(it);

        }
        return ans;
        
    }
    




};
long long  vertexDisjointPathCount(long long  V,long long  S,long long  T,vector<pair<long long ,long long >>&edges)
{
    long long  N=2*V;
    MaxFlow mf(N);
    auto vin=[&](long long  v){return v;};
    auto vout=[&](long long  v){ return v+V;};
    for(long long  v=0;v<V;v++)
    {
        if(v==S || v==T)
        {
            mf.addEdge(vin(v),vout(v),LLONG_MAX);

        }
        else
        {
            mf.addEdge(vout(v),vin(v),1);
        }
    }
    for(auto &e:edges)
    {
        long long  u=e.first;
        long long  v=e.second;
        mf.addEdge(vout(u),vin(v),LLONG_MAX);
        mf.addEdge(vout(v),vin(u),LLONG_MAX);//undirected
    }
    return mf.maxFlow(vout(S),vin(T));

}
int main()
{
    int n,m;
    cin>>n>>m;
    int src=0,des=n-1;
    MaxFlow mf(n);
    vector<pair<long long,long long>>edges(m);
    for(int i=0;i<m;i++)
    {
        int a,b;
        cin>>a>>b;
        mf.addEdge(a-1,b-1,1);
        mf.addEdge(b-1,a-1,1);
        edges[i]={a-1,b-1};
    }
    vector<pair<long long,long long>>ans=mf.howManyMinimumEdgeCut(src,des,edges);
    cout<<ans.size()<<endl;
    for(auto it:ans)
    cout<<it.first+1<<" "<<it.second+1<<endl;

}