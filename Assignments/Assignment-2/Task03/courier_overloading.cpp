#include <iostream>

struct Parcel { double weightKg; bool express; bool insured; };

double charge(double weightKg, bool express = false, bool insured = false) {
    return 50.0 + weightKg * 30.0 + (express ? 40.0 : 0.0) + (insured ? 25.0 : 0.0);
}
double charge(int weightGrams, bool express = false, bool insured = false) {
    return charge(weightGrams / 1000.0, express, insured);
}
double charge(const Parcel& p) { return charge(p.weightKg, p.express, p.insured); }

int main() {
    std::cout << charge(2) << ' ' << charge(2.0f) << ' ' << charge(Parcel{3.0, true, false}) << '\n';
    return 0;
}
