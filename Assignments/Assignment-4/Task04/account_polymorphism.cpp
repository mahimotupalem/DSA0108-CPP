#include <iostream>

struct Account {
    double rate() const { return 3.5; }
    virtual double vrate() const { return 3.5; }
    virtual ~Account() = default;
};

struct SeniorAccount : Account {
    double* history = new double[100];
    double rate() const { return 4.0; }
    double vrate() const override { return 4.0; }
    ~SeniorAccount() override { delete[] history; }
};

int main() {
    SeniorAccount s;
    Account* p = &s;
    Account copy = s;
    std::cout << p->rate() << ' ' << p->vrate() << ' ' << copy.vrate() << '\n';

    Account* accounts[2] = {new Account, new SeniorAccount};
    for (Account* a : accounts) std::cout << a->vrate() << '\n';
    for (Account* a : accounts) delete a;
    return 0;
}
