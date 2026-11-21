#include <iostream>
#include <cmath>

class Time {
    int h, m;
public:
    Time(int hh, int mm) : h(hh), m(mm) {}
    friend inline int minutesBetween(const Time& a, const Time& b);
    friend inline bool earlier(const Time& a, const Time& b);
};

inline int minutesBetween(const Time& a, const Time& b) {
    return std::abs((a.h * 60 + a.m) - (b.h * 60 + b.m));
}
inline bool earlier(const Time& a, const Time& b) {
    return (a.h * 60 + a.m) < (b.h * 60 + b.m);
}

int main() {
    Time departure(9, 45), arrival(13, 10);
    std::cout << minutesBetween(departure, arrival) << ' ' << std::boolalpha << earlier(arrival, departure) << '\n';
    return 0;
}
