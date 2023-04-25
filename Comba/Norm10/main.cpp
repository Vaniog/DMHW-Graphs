#include <iostream>
#include <bits/stdc++.h>

using namespace std;
using ll = long long;

std::vector<ll> fact;
std::set<std::set<set<int>>> sets;

void Count(const std::map<int, char>& s, std::set<set<int>> cur_set) {
    ll cnt = 0;
    if (s.size() % 2 != 0)
        return;

    if (s.empty()) {
        sets.insert(cur_set);
        cout << sets.size() << "\n";
        return;
    }

    for (auto it = s.begin(); it != s.end(); ++it) {
        auto j = it;
        ++j;
        if (j == s.end())
            j = s.begin();

        if (s.size() == 2 && j == s.begin())
            continue;
        if (
            (it->second == 'U' &&j->second == 'A')
                || (it->second == 'A' &&j->second == 'U')
                || (it->second == 'C' &&j->second == 'G')
                || (it->second == 'G' &&j->second == 'C')
            ) {



            auto s_2 = s;
            s_2.erase(it->first);
            s_2.erase(j->first);

            cur_set.insert({it->first, j->first});
            Count(s_2, cur_set);


            cur_set.erase({it->first, j->first});
        }
    }
}

int main() {
    fact.emplace_back(1);
    for (ll i = 1; i < 100; i++) {
        fact.emplace_back(fact.back() * i);
    }
    string s = "CGUAAUUACGGCAUUAGCAU";
    std::map<int, char> s_s;

    for (int i = 0; i < s.size(); i++){
        s_s.insert({i, s[i]});
    }

    Count(s_s, {});
    cout << sets.size();
    return 0;
}
