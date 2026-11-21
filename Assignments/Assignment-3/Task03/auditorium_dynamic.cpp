#include <iostream>
#include <string>

struct Logger {
    std::string n;
    Logger(const std::string& s) : n(s) { std::cout << '+' << n << ' '; }
    Logger(const Logger& o) : n(o.n + "c") { std::cout << "copy" << n << ' '; }
    ~Logger() { std::cout << '-' << n << ' '; }
};
void use(Logger) { std::cout << "use "; }

class Auditorium {
    int rows, cols;
    char** seats;
public:
    Auditorium(int r, int c) : rows(r), cols(c), seats(r > 0 ? new char*[r] : nullptr) {
        for (int i = 0; i < rows; ++i) {
            seats[i] = new char[cols];
            for (int j = 0; j < cols; ++j) seats[i][j] = 'O';
        }
    }
    ~Auditorium() {
        for (int i = 0; i < rows; ++i) delete[] seats[i];
        delete[] seats;
    }
    void book(int r, int c) { if (r >= 0 && r < rows && c >= 0 && c < cols) seats[r][c] = 'X'; }
};

int main() {
    Logger a("A");
    { Logger b("B"); }
    use(a);
    Logger* p = new Logger("P");
    delete p;
    std::cout << "end\n";

    Auditorium aud(5, 10);
    aud.book(2, 4);
    return 0;
}
