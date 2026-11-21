#include <iostream>
#include <string>

class BusPool {
    int cap, issued = 0;
public:
    explicit BusPool(int c) : cap(c) {}
    int nextSeat() { return issued < cap ? ++issued : 0; }
    int totalIssued() const { return issued; }
};

class Ticket {
    std::string passenger;
    int seat;
public:
    Ticket(std::string n, BusPool& pool) : passenger(std::move(n)), seat(pool.nextSeat()) {}
    void print() const { std::cout << passenger << ": " << (seat ? "Seat " + std::to_string(seat) : "Waiting") << '\n'; }
};

int main() {
    BusPool b1(2), b2(2);
    Ticket t1("Asha", b1), t2("Bala", b1), t3("Chitra", b1), t4("Dev", b2);
    t1.print(); t2.print(); t3.print(); t4.print();
    return 0;
}
