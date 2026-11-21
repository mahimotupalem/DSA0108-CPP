#include <iostream>
#include <iomanip>
#include <climits>

int main() {
    for (long long n : {20000LL, 500000LL, 3000000LL}) {
        long long exact = n * 2400LL;
        double approx = 0.0;
        for (long long i = 0; i < n; ++i) approx += 24.0;
        std::cout << "N=" << n << " | Exact Rs: " << exact / 100.0
                  << " | Double Rs: " << std::fixed << std::setprecision(2) << approx
                  << " | Int overflow? " << (exact > INT_MAX) << '\n';
    }
    return 0;
}
