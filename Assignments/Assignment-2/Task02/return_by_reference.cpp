#include <iostream>
#include <vector>

int& highestMark(int marks[], int n) {
    int idx = 0;
    for (int i = 1; i < n; ++i) if (marks[i] > marks[idx]) idx = i;
    return marks[idx];
}

int main() {
    int marks[] = {72, 85, 91, 68, 88};
    highestMark(marks, 5) += 2;
    std::cout << "Moderated highest mark: " << marks[2] << '\n';
    return 0;
}
