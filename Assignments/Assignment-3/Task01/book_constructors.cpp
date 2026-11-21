#include <iostream>
#include <string>

class Book {
    std::string title;
    std::string author;
    int copies;
public:
    Book() : Book("Untitled", "Unknown", 0) {}
    explicit Book(std::string t) : Book(std::move(t), "Donated", 1) {}
    Book(std::string t, std::string a, int c)
        : title(std::move(t)), author(std::move(a)), copies(c < 0 ? 0 : c) {}
    void print() const { std::cout << title << " by " << author << " (" << copies << ")\n"; }
};

int main() {
    Book b1, b2("Clean Code"), b3("C++ Primer", "Lippman", 5), b4("Invalid", "Author", -3);
    b1.print(); b2.print(); b3.print(); b4.print();
    return 0;
}
