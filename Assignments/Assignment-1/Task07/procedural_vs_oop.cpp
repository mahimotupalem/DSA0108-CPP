#include <iostream>

constexpr long long MIN_BALANCE_PAISE = 5000;
bool procDeduct(long long& bal, long long f) {
    if ((bal - f) < MIN_BALANCE_PAISE) return false;
    bal -= f;
    return true;
}

class SmartCard {
    long long balancePaise;
public:
    explicit SmartCard(long long init) : balancePaise(init < 0 ? 0 : init) {}
    bool canTravel(long long f) const { return (balancePaise - f) >= MIN_BALANCE_PAISE; }
    bool deduct(long long f) {
        if (!canTravel(f)) return false;
        balancePaise -= f;
        return true;
    }
    long long balance() const { return balancePaise; }
};

int main() {
    long long b = 7000;
    procDeduct(b, 1500);
    SmartCard c(7000);
    c.deduct(1500);
    std::cout << "Procedural: " << b << " | OOP: " << c.balance() << '\n';
    return 0;
}
