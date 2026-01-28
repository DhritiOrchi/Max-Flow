#include <bits/stdc++.h>
using namespace std;

struct MaxFlow {
    int n;
    vector<vector<int>> cap, flow, adj;

    MaxFlow(int n) : n(n) {
        cap.assign(n, vector<int>(n, 0));
        flow.assign(n, vector<int>(n, 0));
        adj.resize(n);
    }

    void addEdge(int u, int v, int c) {
        if (cap[u][v] == 0 && cap[v][u] == 0) {
            adj[u].push_back(v);
            adj[v].push_back(u); // residual
        }
        cap[u][v] += c;
    }

    int bfs(int s, int t, vector<int>& parent) {
        fill(parent.begin(), parent.end(), -1);
        parent[s] = -2;
        queue<pair<int,int>> q;
        q.push({s, INT_MAX});

        while (!q.empty()) {
            //auto [u, f] = q.front(); q.pop();
            int u=q.front().first;
            int f=q.front().second;
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
};

// Extract all distinct routes using forward flow only
vector<vector<int>> extractPaths(MaxFlow &mf, int S, int T) {
    int total = mf.maxFlow(S, T);
    vector<vector<int>> paths;

    for(int i=0; i<total; i++) {
        vector<int> path;
        int cur = S;
        path.push_back(cur);

        while(cur != T) {
            for(int next : mf.adj[cur]) {
                if(mf.flow[cur][next] > 0) {
                    path.push_back(next);
                    mf.flow[cur][next]--; // consume this edge
                    cur = next;
                    break;
                }
            }
        }
        paths.push_back(path);
    }
    return paths;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    MaxFlow mf(n);
    for(int i=0; i<m; i++) {
        int a, b;
        cin >> a >> b;
        a--; b--; // zero-based indexing
        mf.addEdge(a, b, 1);
    }

    int S = 0, T = n-1;
    vector<vector<int>> routes = extractPaths(mf, S, T);

    cout << routes.size() << "\n";
    for(auto &path : routes) {
        cout<<path.size()<<endl;
        for(int node : path) cout << node+1 << " "; // convert back to 1-based
        cout << "\n";
    }
}
