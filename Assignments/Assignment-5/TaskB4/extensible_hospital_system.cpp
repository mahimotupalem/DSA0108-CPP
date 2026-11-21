#include <iostream>
#include <string>
#include <memory>
#include <vector>

class Bill {
public:
    virtual double total() const = 0;
    virtual std::string kind() const = 0;
    virtual ~Bill() = default;
};

class TelemedicineBill : public Bill {
    double fee;
public:
    explicit TelemedicineBill(double f) : fee(f) {}
    double total() const override { return fee; }
    std::string kind() const override { return "Telemedicine"; }
};

class Payer {
public:
    virtual double covered(const Bill& b) const = 0;
    virtual ~Payer() = default;
};

class CorporatePayer : public Payer {
    double cap;
public:
    explicit CorporatePayer(double c) : cap(c) {}
    double covered(const Bill& b) const override { return b.total() > cap ? cap : b.total(); }
};

int main() {
    std::vector<std::unique_ptr<Bill>> bills;
    bills.push_back(std::make_unique<TelemedicineBill>(800.0));
    CorporatePayer corp(1000.0);
    for (const auto& b : bills) {
        std::cout << b->kind() << " Total: " << b->total() << " | Covered: " << corp.covered(*b) << '\n';
    }
    return 0;
}
