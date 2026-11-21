#include <iostream>
#include <string>

class SavingsAccount {
    std::string accNo;
    double balance;
    double txns[10]{};
    int count = 0;
public:
    SavingsAccount(std::string a, double b) : accNo(std::move(a)), balance(b) {}
    void deposit(double amt);
    bool withdraw(double amt);
    void miniStatement() const;
};

void SavingsAccount::deposit(double amt) {
    if (amt <= 0) return;
    balance += amt;
    txns[count++ % 10] = amt;
}
bool SavingsAccount::withdraw(double amt) {
    if (amt <= 0 || amt > balance) return false;
    balance -= amt;
    txns[count++ % 10] = -amt;
    return true;
}
void SavingsAccount::miniStatement() const {
    std::cout << "Account " << accNo << " Balance: " << balance << '\n';
    int n = count < 10 ? count : 10;
    int start = count < 10 ? 0 : count % 10;
    for (int i = 0; i < n; ++i) std::cout << txns[(start + i) % 10] << ' ';
    std::cout << '\n';
}

int main() {
    SavingsAccount s("SB101", 5000);
    for (int i = 1; i <= 12; ++i) s.deposit(i * 100);
    s.miniStatement();
    return 0;
}
