#pragma once
#include <bits/stdc++.h>
using namespace std;

/*
 * Usage:
 *   debug(x);
 *   debug(a, b, c);
 *   debug(vector, set, map, pair, tuple, etc);
 *
 * Compile locally with:
 *   g++ -DLOCAL main.cpp
 */

// -------------------- basic printers --------------------
void _print(const char *x) { cerr << x; }
void _print(char x) { cerr << '\'' << x << '\''; }
void _print(bool x) { cerr << (x ? "true" : "false"); }

void _print(short x) { cerr << x; }
void _print(int x) { cerr << x; }
void _print(long x) { cerr << x; }
void _print(long long x) { cerr << x; }

void _print(unsigned short x) { cerr << x; }
void _print(unsigned int x) { cerr << x; }
void _print(unsigned long x) { cerr << x; }
void _print(unsigned long long x) { cerr << x; }

void _print(float x) { cerr << x; }
void _print(double x) { cerr << x; }
void _print(long double x) { cerr << x; }

void _print(const string &x) { cerr << '"' << x << '"'; }

// -------------------- pair --------------------
template <class T, class U>
void _print(const pair<T, U> &p) {
    cerr << "{";
    _print(p.first);
    cerr << ", ";
    _print(p.second);
    cerr << "}";
}

// -------------------- tuple --------------------
template <size_t I = 0, class... Ts>
typename enable_if<I == sizeof...(Ts)>::type
_print(const tuple<Ts...>&) {}

template <size_t I = 0, class... Ts>
typename enable_if<I < sizeof...(Ts)>::type
_print(const tuple<Ts...> &t) {
    if (I) cerr << ", ";
    _print(get<I>(t));
    _print<I + 1>(t);
}

template <class... Ts>
void _print(const tuple<Ts...> &t) {
    cerr << "(";
    _print(t);
    cerr << ")";
}

// -------------------- containers --------------------
template <class T>
void _print(const vector<T> &v) {
    cerr << "[ ";
    for (auto &x : v) {
        _print(x);
        cerr << " ";
    }
    cerr << "]";
}

template <class T>
void _print(const deque<T> &v) {
    cerr << "[ ";
    for (auto &x : v) {
        _print(x);
        cerr << " ";
    }
    cerr << "]";
}

template <class T>
void _print(const set<T> &v) {
    cerr << "{ ";
    for (auto &x : v) {
        _print(x);
        cerr << " ";
    }
    cerr << "}";
}

template <class T>
void _print(const multiset<T> &v) {
    cerr << "{ ";
    for (auto &x : v) {
        _print(x);
        cerr << " ";
    }
    cerr << "}";
}

template <class K, class V>
void _print(const map<K, V> &m) {
    cerr << "{ ";
    for (auto &x : m) {
        _print(x);
        cerr << " ";
    }
    cerr << "}";
}

template <class K, class V>
void _print(const unordered_map<K, V> &m) {
    cerr << "{ ";
    for (auto &x : m) {
        _print(x);
        cerr << " ";
    }
    cerr << "}";
}

template <class T>
void _print(const unordered_set<T> &v) {
    cerr << "{ ";
    for (auto &x : v) {
        _print(x);
        cerr << " ";
    }
    cerr << "}";
}

// -------------------- adapters --------------------
template <class T>
void _print(stack<T> s) {
    cerr << "[ ";
    while (!s.empty()) {
        _print(s.top());
        cerr << " ";
        s.pop();
    }
    cerr << "]";
}

template <class T>
void _print(queue<T> q) {
    cerr << "[ ";
    while (!q.empty()) {
        _print(q.front());
        cerr << " ";
        q.pop();
    }
    cerr << "]";
}

template <class T>
void _print(priority_queue<T> q) {
    cerr << "[ ";
    while (!q.empty()) {
        _print(q.top());
        cerr << " ";
        q.pop();
    }
    cerr << "]";
}

// -------------------- recursive variadic output --------------------
void debug_out() { cerr << "\n"; }

template <class Head, class... Tail>
void debug_out(Head H, Tail... T) {
    _print(H);
    if (sizeof...(T)) cerr << ", ";
    debug_out(T...);
}

// -------------------- main debug macro --------------------
#define debug(...) \
    cerr << "[ " << #__VA_ARGS__ << " ] = "; \
    debug_out(__VA_ARGS__)

// -------------------- debug with line info --------------------
#define debugl(...) \
    cerr << "Line " << __LINE__ << ": "; \
    debug(__VA_ARGS__)
