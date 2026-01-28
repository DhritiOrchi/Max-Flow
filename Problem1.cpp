#include <bits/stdc++.h>
using namespace std;
const int INF = 1e9;
int n, m;
vector<vector<int>> capacity;
vector<vector<int>> original_capacity;
vector<vector<int>> flow;
vector<vector<int>> adj;
vector<pair<int,int>> edges;
int bfs(int s, int t, vector<int>& parent) {
    fill(parent.begin(), parent.end(), -1);
    parent[s] = -2;

    queue<pair<int,int>> q;
    q.push({s, INF});

    while (!q.empty()) {
        int cur = q.front().first;
        int cur_flow = q.front().second;
        q.pop();

        for (int next : adj[cur]) {
            if (parent[next] == -1 && capacity[cur][next] > 0) {
                parent[next] = cur;
                int new_flow = min(cur_flow, capacity[cur][next]);

                if (next == t)
                    return new_flow;

                q.push({next, new_flow});
            }
        }
    }
    return 0;
}
int maxflow(int s, int t) {
    int total_flow = 0;
    vector<int> parent(n);

    int new_flow;
    while ((new_flow = bfs(s, t, parent))) {
        total_flow += new_flow;
        int cur = t;

        while (cur != s) {
            int prev = parent[cur];

            capacity[prev][cur] -= new_flow;
            capacity[cur][prev] += new_flow;

            flow[prev][cur] += new_flow;
            flow[cur][prev] -= new_flow;

            cur = prev;
        }
    }
    return total_flow;
}
int main() {
    cin >> n >> m;
    capacity.assign(n, vector<int>(n, 0));
    original_capacity.assign(n, vector<int>(n, 0));
    flow.assign(n, vector<int>(n, 0));
    adj.assign(n, vector<int>());

    for (int i = 0; i < m; i++) {
        int u, v, c;
        cin >> u >> v >> c;
        capacity[u][v] += c;
        original_capacity[u][v] += c;
        adj[u].push_back(v);
        adj[v].push_back(u);
        edges.push_back({u, v});
    }
    int s, t;
    cin >> s >> t;
    int maxFlow = maxflow(s, t);
    cout << maxFlow << "\n";
    for (auto &e : edges) {
        int u = e.first;
        int v = e.second;
        cout << u << " " << v << " " << flow[u][v] << "/" << original_capacity[u][v] << "\n";
    }
    return 0;
}