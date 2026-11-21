#include <iostream>
#include <array>

enum class Zone { Inner = 0, Middle = 1, Outer = 2, Airport = 3 };

class MonthlyPass {
    bool active;
    Zone z1, z2;
public:
    MonthlyPass() : active(false), z1(Zone::Inner), z2(Zone::Inner) {}
    MonthlyPass(Zone a, Zone b) : active(true), z1(a), z2(b) {}
    bool covers(Zone z) const { return active && (z == z1 || z == z2); }
};

class FarePolicy {
    static constexpr std::array<long long, 4> BASE = {1000, 2000, 3000, 6000};
public:
    static long long compute(Zone z, int hour, bool weekend, const MonthlyPass& p) {
        if (p.covers(z)) return 0;
        long long f = BASE[static_cast<size_t>(z)];
        if (weekend) f -= (f * 10) / 100;
        else if ((hour >= 8 && hour < 10) || (hour >= 17 && hour < 20)) f += (f * 20) / 100;
        return f;
    }
};

int main() {
    MonthlyPass pass(Zone::Inner, Zone::Airport);
    std::cout << "Airport w/ Pass: " << FarePolicy::compute(Zone::Airport, 9, false, pass)
              << " | Outer Weekend: " << FarePolicy::compute(Zone::Outer, 9, true, pass) << '\n';
    return 0;
}
