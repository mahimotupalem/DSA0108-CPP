#include <iostream>
#include <vector>
#include <chrono>

struct Base { virtual double val() const = 0; virtual ~Base() = default; };
struct D1 : Base { double x; explicit D1(double v) : x(v) {} double val() const override { return x * 1.1; } };
struct Flat { int type; double x; };

int main() {
    const int N = 1000000;
    std::vector<Base*> poly(N, new D1(100.0));
    std::vector<Flat> flat(N, {1, 100.0});

    auto t1 = std::chrono::high_resolution_clock::now();
    double s1 = 0;
    for (auto* p : poly) s1 += p->val();
    auto t2 = std::chrono::high_resolution_clock::now();

    double s2 = 0;
    for (const auto& f : flat) {
        switch (f.type) { case 1: s2 += f.x * 1.1; break; default: break; }
    }
    auto t3 = std::chrono::high_resolution_clock::now();

    delete poly[0];
    std::cout << "Virtual us: " << std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count()
              << " | Switch us: " << std::chrono::duration_cast<std::chrono::microseconds>(t3 - t2).count()
              << " | Sums: " << s1 << ' ' << s2 << '\n';
    return 0;
}
