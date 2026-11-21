#include <iomanip>
#include <iostream>

class Money {
    long long paise;
public:
    Money(long long rupees = 0, int p = 0) : paise(rupees * 100 + p) {}
    Money operator+(const Money& rhs) const {
        Money res; res.paise = paise + rhs.paise; return res;
    }
    Money operator-(const Money& rhs) const {
        Money res; res.paise = paise - rhs.paise; return res;
    }
    Money& operator+=(const Money& rhs) { paise += rhs.paise; return *this; }
    bool operator==(const Money& rhs) const { return paise == rhs.paise; }
    bool operator<(const Money& rhs) const { return paise < rhs.paise; }
    friend std::ostream& operator<<(std::ostream& os, const Money& m) {
        os << "Rs " << m.paise / 100 << '.' << std::setw(2) << std::setfill('0') << std::abs(m.paise % 100) << std::setfill(' ');
        return os;
    }
};

int main() {
    Money a(120, 75), b(79, 25);
    Money c = a + b;
    std::cout << c << '\n' << std::boolalpha << (c == Money(200)) << '\n';
    std::cout << "Carry test: " << (Money(0, 75) + Money(0, 50)) << '\n';
    return 0;
}
