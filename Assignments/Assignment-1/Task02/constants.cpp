#include <iostream>

enum class Zone : int { Inner = 0, Middle = 1, Outer = 2 };
constexpr int ZONE_COUNT = 3;
constexpr long long BASE_FARE_PAISE[ZONE_COUNT] = {1000, 2000, 3000};
constexpr int PEAK_SURCHARGE_PERCENT = 20;
constexpr long long MIN_BALANCE_PAISE = 5000;

int main() {
    std::cout << "Inner: " << BASE_FARE_PAISE[static_cast<int>(Zone::Inner)]
              << " | Middle: " << BASE_FARE_PAISE[static_cast<int>(Zone::Middle)]
              << " | Outer: " << BASE_FARE_PAISE[static_cast<int>(Zone::Outer)] << '\n';
    return 0;
}
