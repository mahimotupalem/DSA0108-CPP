#include <iostream>
#include <cmath>
#include <stdexcept>

class Complex {
    double re, im;
public:
    Complex(double r = 0.0, double i = 0.0) : re(r), im(i) {}
    Complex operator+(const Complex& o) const { return {re + o.re, im + o.im}; }
    Complex operator-(const Complex& o) const { return {re - o.re, im - o.im}; }
    Complex operator*(const Complex& o) const { return {re * o.re - im * o.im, re * o.im + im * o.re}; }
    Complex operator/(const Complex& o) const {
        double denom = o.re * o.re + o.im * o.im;
        if (denom == 0.0) throw std::domain_error("Division by zero impedance");
        return {(re * o.re + im * o.im) / denom, (im * o.re - re * o.im) / denom};
    }
    double magnitude() const { return std::hypot(re, im); }
    friend std::ostream& operator<<(std::ostream& os, const Complex& z) {
        return os << z.re << (z.im >= 0 ? " + j" : " - j") << std::abs(z.im);
    }
};

int main() {
    Complex z1(10.0, 5.0), z2(20.0, -10.0);
    Complex series = z1 + z2;
    Complex parallel = (z1 * z2) / (z1 + z2);
    std::cout << "Series: " << series << " | |Z|=" << series.magnitude() << '\n';
    std::cout << "Parallel: " << parallel << " | |Z|=" << parallel.magnitude() << '\n';
    return 0;
}
