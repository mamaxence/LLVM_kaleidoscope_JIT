#include <iostream>

#include "program.h"

int main()
{
    auto code = R""""(
        def binary : 1 (x y) y;
        def fib(x)
            var a = 1, b = 1, c in
            (for i = 2, i < x, 1 in
                c = a + b:
                a = b:
                b  = c):
            b;
        fib(3)
        fib(4)
        fib(5)
        fib(10)
    )"""";
    auto program = ckalei::Program(code);
    auto res = *program.evaluate();
    for (const auto &v :res){
        std::cout << v << "\n";
    }
}
