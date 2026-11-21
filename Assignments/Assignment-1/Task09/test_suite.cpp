#include <iostream>
#include <cassert>

enum class Zone { Inner = 0, Middle = 1, Outer = 2 };
constexpr long long BASE[] = {1000, 2000, 3000};

long long computeFare(Zone z, int hour) {
    long long f = BASE[static_cast<int>(z)];
    if ((hour >= 8 && hour < 10) || (hour >= 17 && hour < 20)) f += (f * 20) / 100;
    return f;
}

int main() {
    assert(computeFare(Zone::Inner, 8) == 1200);
    assert(computeFare(Zone::Inner, 10) == 1000);
    assert(computeFare(Zone::Outer, 17) == 3600);
    assert(computeFare(Zone::Outer, 20) == 3000);
    long long batchTotal = computeFare(Zone::Inner, 7) + computeFare(Zone::Middle, 9)
                         + computeFare(Zone::Outer, 14) + computeFare(Zone::Outer, 18)
                         + computeFare(Zone::Middle, 21);
    assert(batchTotal == 12000);
    std::cout << "Task 09 tests passed. Batch total: " << batchTotal << " paise\n";
    return 0;
}
