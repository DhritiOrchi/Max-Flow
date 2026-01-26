#include <bits/stdc++.h>
using namespace std;
const int INF = 1e9;
int N, K, M;
vector<vector<int>> capacity;
vector<vector<int>> flow;
vector<vector<int>> adj;
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
int maxflow(int s, int t, int total_nodes) {
    int total_flow = 0;
    vector<int> parent(total_nodes);
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
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cin >> N >> K >> M;
    int S = N;
    int T = N + 1;
    int total_nodes = N + 2;
    capacity.assign(total_nodes, vector<int>(total_nodes, 0));
    flow.assign(total_nodes, vector<int>(total_nodes, 0));
    adj.assign(total_nodes, vector<int>());
    for (int i = 0; i < K; i++) {
        capacity[S][i] = 1;
        adj[S].push_back(i);
        adj[i].push_back(S);
    }
    for (int i = 0; i < M; i++) {
        int u, v;
        cin >> u >> v;
        capacity[u][v] = 1;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    for (int i = K; i < N; i++) {
        capacity[i][T] = 1;
        adj[i].push_back(T);
        adj[T].push_back(i);
    }
    int result = maxflow(S, T, total_nodes);
    cout << result << "\n";
    for (int u = 0; u < K; u++) {
        for (int v : adj[u]) {
            if (v >= K && v < N && flow[u][v] == 1) {
                cout << u << " " << v << "\n";
            }
        }
    }
    return 0;
}
