#include <bits/stdc++.h>
using namespace std;
vector<pair<int,int>> originalEdges;


struct MaxFlow {
    int n;
    vector<vector<int>> cap, flow, adj;

    MaxFlow(int n) : n(n) {
        cap.assign(n, vector<int>(n, 0));
        flow.assign(n, vector<int>(n, 0));
        adj.resize(n);
    }

    void addEdge(int u, int v, int c) {
        cap[u][v] += c;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    int bfs(int s, int t, vector<int>& parent) {
        fill(parent.begin(), parent.end(), -1);
        parent[s] = -2;
        queue<pair<int,int>> q;
        q.push({s, INT_MAX});

        while (!q.empty()) {
            int u = q.front().first;
            int f = q.front().second;
            q.pop();

            for (int v : adj[u]) {
                if (parent[v] == -1 && cap[u][v] - flow[u][v] > 0) {
                    parent[v] = u;
                    int new_flow = min(f, cap[u][v] - flow[u][v]);
                    if (v == t) return new_flow;
                    q.push({v, new_flow});
                }
            }
        }
        return 0;
    }

    int maxFlow(int s, int t) {
        int total = 0;
        vector<int> parent(n);

        while (int pushed = bfs(s, t, parent)) {
            total += pushed;
            int cur = t;
            while (cur != s) {
                int prev = parent[cur];
                flow[prev][cur] += pushed;
                flow[cur][prev] -= pushed;
                cur = prev;
            }
        }
        return total;
    }

    
    int bipartiteMatching(int L, int R, vector<pair<int,int>>& edges) {
        
        int S = 0;
        int T = L + R + 1;

        // Source → Left
        for (int i = 1; i <= L; i++)
            addEdge(S, i, 1);

        // Left → Right
        for (auto &e : edges) {
            int u = e.first;   // 1..L
            int v = e.second;  // 1..R
            addEdge(u, L + v, 1);
        }

        // Right → Sink
        for (int i = 1; i <= R; i++)
            addEdge(L + i, T, 1);

        return maxFlow(S, T);
    }
    vector<bool> minCut(int S)
    {// first call the max flow algorithm
        vector<bool>visited(n,false);
        queue<int>q;
        q.push(S);
        visited[S]=true;
        while (!q.empty())
        {
            int node=q.front();
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
    //Edge-disjoint paths between s and t are equivalent
    // to the maximum flow in a graph where all edges have capacity 1.
    vector<vector<int>> edgeDisjointPaths(int s,int t)
    {
        int max_paths=maxFlow(s,t);
        vector<vector<int>>paths;
        vector<vector<int>>tempFlow=flow;
        for(int p=0;p<max_paths;p++)
        {
            vector<int>path;
            int curr=s;
            path.push_back(s);
            while (curr!=t)
            {
                for(int next:adj[curr])
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
    




};
int vertexDisjointPathCount(int V,int S,int T,vector<pair<int,int>>&edges)
{
    int N=2*V;
    MaxFlow mf(N);
    auto vin=[&](int v){return v;};
    auto vout=[&](int v){ return v+V;};
    for(int v=0;v<V;v++)
    {
        if(v==S || v==T)
        {
            mf.addEdge(vin(v),vout(v),INT_MAX);

        }
        else
        {
            mf.addEdge(vout(v),vin(v),1);
        }
    }
    for(auto &e:edges)
    {
        int u=e.first;
        int v=e.second;
        mf.addEdge(vout(u),vin(v),INT_MAX);
        mf.addEdge(vout(v),vin(u),INT_MAX);//undirected
    }
    return mf.maxFlow(vout(S),vin(T));

}
pair<int, vector<pair<int,int>>> globalMinCutUsingMaxFlow(
    int V,
    vector<pair<int,int>>& edges
) {
    int bestCut = INT_MAX;
    vector<pair<int,int>> bestEdges;

    for (int s = 0; s < V; s++) {
        for (int t = s + 1; t < V; t++) {
            MaxFlow mf(V);
            originalEdges.clear();

            for (auto &e : edges) {
                int u = e.first;
                int v = e.second;
                originalEdges.push_back({u, v});
                mf.addEdge(u, v, 1);
                mf.addEdge(v, u, 1);
            }

            int cutValue = mf.maxFlow(s, t);

            if (cutValue < bestCut) {
                bestCut = cutValue;
                vector<bool> side = mf.minCut(s);
                bestEdges.clear();

                for (auto &e : originalEdges) {
                    int u = e.first;
                    int v = e.second;
                    if (side[u] && !side[v]) bestEdges.push_back({u, v});
                    else if (side[v] && !side[u]) bestEdges.push_back({v, u});
                }
            }
        }
    }

    return {bestCut, bestEdges};
}

int main()
{
    int n,m,k;
    cin>>n>>m>>k;
    int src=0,des=n+m+1;
    int totalNodes=n+m+2;
    MaxFlow mf(totalNodes);
    originalEdges.resize(k);
    for(int i=1;i<=n;i++)
    {
        mf.addEdge(src,i,1);
    }
    int startB=n+1;
    for(int i=0;i<m;i++)
    {
        mf.addEdge(startB+i,des,1);

    }
    for(int i=0;i<k;i++)
    {
        int a,b;
        cin>>a>>b;
        originalEdges[i]={a,b};
        mf.addEdge(a,b+n,1);
    }
    int matching = mf.maxFlow(src, des);
    cout << matching << "\n";
    for (int boy = 1; boy <= n; boy++) {
        for (int girl = 1; girl <= m; girl++) {
            if (mf.flow[boy][n + girl] == 1) {
                cout << boy << " " << girl << "\n";
            }
        }
    }



}