#include <iostream>

enum class Zone { Inner = 0, Middle = 1, Outer = 2 };

class MetroFareEngine {
    static constexpr long long BASE[] = {1000, 2000, 3000};
    static constexpr long long MIN_BAL = 5000;
public:
    static long long fare(Zone z, int h) {
        long long f = BASE[static_cast<int>(z)];
        if ((h >= 8 && h < 10) || (h >= 17 && h < 20)) f += (f * 20) / 100;
        return f;
    }
    static bool tap(long long& bal, Zone z, int h) {
        long long f = fare(z, h);
        if ((bal - f) < MIN_BAL) return false;
        bal -= f;
        return true;
    }
};

int main() {
    long long bal = 10000;
    MetroFareEngine::tap(bal, Zone::Outer, 9);
    std::cout << "Final Design Remaining Balance: " << bal << " paise\n";
    return 0;
}
