#include <iostream>

[[nodiscard]] int func() { }

int main() {
    int a = func();
    func();
    return a;
}
