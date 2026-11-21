#include <iostream>
#include <iomanip>
#include <climits>

int main() {
    std::cout << "sizeof(int): " << sizeof(int) << " | INT_MAX: " << INT_MAX << '\n';
    std::cout << "sizeof(long long): " << sizeof(long long) << " | LLONG_MAX: " << LLONG_MAX << '\n';
    long long totalPaise = 3000000LL * 3600LL;
    std::cout << "Network total paise: " << totalPaise << " (Overflows int? " << (totalPaise > INT_MAX) << ")\n";
    double dSum = 0.0;
    long long iSum = 0;
    for (int i = 0; i < 3000000; ++i) { dSum += 0.10; iSum += 10; }
    std::cout << std::fixed << std::setprecision(6) << "Exact: " << iSum / 100.0 << " | Double: " << dSum << '\n';
    return 0;
}
