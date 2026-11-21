#include <iostream>
#include <vector>
#include <string>

struct Counter {
    int v;
    Counter& operator++() { ++v; return *this; }
    Counter operator++(int) { Counter old = *this; ++v; return old; }
};

class Playlist {
    std::vector<std::string> tracks;
    std::string fallback = "Invalid Track";
public:
    void add(const std::string& t) { tracks.push_back(t); }
    std::string& operator[](int i) {
        if (i < 0 || i >= static_cast<int>(tracks.size())) { std::cerr << "Out of range\n"; return fallback; }
        return tracks[i];
    }
    const std::string& operator[](int i) const {
        if (i < 0 || i >= static_cast<int>(tracks.size())) return fallback;
        return tracks[i];
    }
};

int main() {
    Counter c{5};
    Counter a = c++;
    Counter b = ++c;
    std::cout << a.v << ' ' << b.v << ' ' << c.v << '\n';

    Playlist pl;
    pl.add("Track 1");
    std::cout << pl[0] << '\n';
    return 0;
}
