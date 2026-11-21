#include <iostream>
#include <string>
#include <vector>

class Staff {
protected:
    std::string name;
    int id;
    static constexpr double DA_RATE = 0.42;
public:
    Staff(std::string n, int i) : name(std::move(n)), id(i) {}
    virtual double pay() const = 0;
    virtual ~Staff() = default;
};

class Professor : public Staff {
    double basic, research;
public:
    Professor(std::string n, int i, double b, double r) : Staff(std::move(n), i), basic(b), research(r) {}
    double pay() const override { return basic + basic * DA_RATE + research; }
};

class LabAssistant : public Staff {
    double basic;
public:
    LabAssistant(std::string n, int i, double b) : Staff(std::move(n), i), basic(b) {}
    double pay() const override { return basic + basic * DA_RATE; }
};

class GuestFaculty : public Staff {
    double hours, rate;
public:
    GuestFaculty(std::string n, int i, double h, double r) : Staff(std::move(n), i), hours(h), rate(r) {}
    double pay() const override { return hours * rate; }
};

int main() {
    Professor p("Dr. Rao", 1, 80000, 15000);
    LabAssistant l("Kumar", 2, 30000);
    GuestFaculty g("Anita", 3, 20, 1500);
    Staff* staff[] = {&p, &l, &g};
    for (const Staff* s : staff) std::cout << s->pay() << '\n';
    return 0;
}
