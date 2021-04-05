#include <iostream>
#include "lexer.h"

#include "program.h"

int main()
{
    auto data = R""""(
                def mult(a b) a*b;
                mult(3 4);
                mult(4 5);
                )"""";
    std::vector<double> expected{12, 20};

    auto program = ckalei::Program(data);
    auto res = *program.evaluate();
}
