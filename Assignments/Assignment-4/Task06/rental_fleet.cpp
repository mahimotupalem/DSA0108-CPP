#include <iostream>
#include <string>
#include <vector>

class Vehicle {
protected:
    std::string regNo;
    double dailyRate;
public:
    Vehicle(std::string r, double rate) : regNo(std::move(r)), dailyRate(rate) {}
    virtual double rentalCharge(int days, double usage) const = 0;
    virtual std::string kind() const = 0;
    virtual ~Vehicle() = default;
};

class Bicycle : public Vehicle {
public:
    Bicycle(std::string r, double rate) : Vehicle(std::move(r), rate) {}
    double rentalCharge(int days, double) const override { return days * dailyRate; }
    std::string kind() const override { return "Bicycle"; }
};

class ElectricCar : public Vehicle {
    double ratePerKwh;
public:
    ElectricCar(std::string r, double rate, double kwhRate)
        : Vehicle(std::move(r), rate), ratePerKwh(kwhRate) {}
    double rentalCharge(int days, double kwhUsed) const override {
        return days * dailyRate + kwhUsed * ratePerKwh;
    }
    std::string kind() const override { return "ElectricCar"; }
};

int main() {
    std::vector<Vehicle*> fleet = {
        new Bicycle("CYC-01", 150.0),
        new ElectricCar("TN-01-EV-99", 2500.0, 12.0)
    };
    for (Vehicle* v : fleet) {
        std::cout << v->kind() << " Charge: Rs " << v->rentalCharge(3, 40.0) << '\n';
        delete v;
    }
    return 0;
}
