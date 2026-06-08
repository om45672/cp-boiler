struct MinStack {
    stack<pair<int,int>> st;
    void push(int x) {
        int new_min = st.empty() ? x : min(x, st.top().second);
        st.push({x, new_min});
    }
    void pop() { st.pop(); }
    int top() { return st.top().first; }
    bool empty() { return st.empty(); }
    int getMin() { return st.top().second; }
};

struct MinQueue {
    MinStack in, out;
    void push(int x) { in.push(x); }
    void transfer() {
        while (!in.empty()) {
            int val = in.top(); in.pop();
            int new_min = out.empty() ? val : min(val, out.getMin());
            out.st.push({val, new_min});
        }
    }
    void pop() {
        if (out.empty()) transfer();
        out.pop();
    }
    int getMin() {
        if (in.empty() && out.empty()) return INT_MAX;
        if (in.empty()) return out.getMin();
        if (out.empty()) return in.getMin();
        return min(in.getMin(), out.getMin());
    }
};