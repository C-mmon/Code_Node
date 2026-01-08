#include <bits/stdc++.h>
using namespace std;

struct Router {
    double x, y;
};

double dist(const Router &a, const Router &b) {
    double dx = a.x - b.x;
    double dy = a.y - b.y;
    return sqrt(dx * dx + dy * dy);
}

// returns true if dst can receive a non-colliding message
bool reachableNoCollision(const vector<Router> &routers,
                          int src, int dst, double R) {
    int n = routers.size();

    // build adjacency list with weights = distances (within range R)
    vector<vector<pair<int,double>>> g(n);
    for (int i = 0; i < n; ++i) {
        for (int j = i+1; j < n; ++j) {
            double d = dist(routers[i], routers[j]);
            if (d <= R) {
                g[i].push_back({j, d});
                g[j].push_back({i, d});
            }
        }
    }

    const double INF = 1e100;
    vector<double> bestTime(n, INF);
    vector<bool> collided(n, false);

    // (time, node)
    using State = pair<double,int>;
    priority_queue<State, vector<State>, greater<State>> pq;

    bestTime[src] = 0.0;
    pq.push({0.0, src});

    const double EPS = 1e-9;

    while (!pq.empty()) {
        auto [t, u] = pq.top();
        pq.pop();

        // stale state
        if (t > bestTime[u] + EPS) continue;

        // if this node is already known to have a collision at its bestTime,
        // it cannot forward messages.
        if (collided[u]) continue;

        if (u == dst) {
            // first time we pop dst with non-collided arrival => success
            return true;
        }

        for (auto &[v, w] : g[u]) {
            double nt = t + w;

            if (nt + EPS < bestTime[v]) {
                // strictly better time: update
                bestTime[v] = nt;
                collided[v] = false;      // so far unique best arrival
                pq.push({nt, v});
            } else if (fabs(nt - bestTime[v]) <= EPS) {
                // another path reaching v at the same best time => collision
                collided[v] = true;
                // we do NOT push this state, and v is no longer usable
            }
            // if nt > bestTime[v] + EPS, ignore (slower path)
        }
    }

    // if we never popped dst with non-colliding arrival, not reachable
    return false;
}

int main() {
    vector<Router> routers = {
        {0, 0},   // A = 0
        {0, 8},   // B = 1
        {0, 17},  // C = 2
        {11, 0}   // D = 3
    };
    double R = 10.0;
    int src = 0, dst = 3;

    bool ok = reachableNoCollision(routers, src, dst, R);
    cout << (ok ? "reachable" : "not reachable") << "\n";
}
