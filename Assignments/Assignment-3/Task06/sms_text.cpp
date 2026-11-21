#include <iostream>
#include <cstring>

class Text {
    char* buf;
public:
    Text(const char* s = "") : buf(new char[std::strlen(s) + 1]) { std::strcpy(buf, s); }
    Text(const Text& o) : buf(new char[std::strlen(o.buf) + 1]) { std::strcpy(buf, o.buf); }
    Text& operator=(const Text& o) {
        if (this != &o) {
            char* tmp = new char[std::strlen(o.buf) + 1];
            std::strcpy(tmp, o.buf);
            delete[] buf;
            buf = tmp;
        }
        return *this;
    }
    ~Text() { delete[] buf; }
    Text operator+(const Text& rhs) const {
        char* tmp = new char[std::strlen(buf) + std::strlen(rhs.buf) + 1];
        std::strcpy(tmp, buf);
        std::strcat(tmp, rhs.buf);
        Text res(tmp);
        delete[] tmp;
        return res;
    }
    bool operator==(const Text& rhs) const { return std::strcmp(buf, rhs.buf) == 0; }
    friend std::ostream& operator<<(std::ostream& os, const Text& t) { return os << t.buf; }
};

int main() {
    Text a("Hello "), b("Metro"), c("!");
    Text msg = a + b + c;
    std::cout << msg << '\n';
    return 0;
}
