#include <iostream>
#include <iomanip>
#include <limits>
#include <sstream>

int main() {
    std::istringstream in("abc\n9\n2\n");
    int hour = 0;
    while (!(in >> hour)) {
        in.clear();
        in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    long long fare = 3600;
    std::cout << "Hour: " << hour << " | Fare: Rs " << fare / 100 << '.'
              << std::setw(2) << std::setfill('0') << fare % 100 << '\n';
    return 0;
}
