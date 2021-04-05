//
// Created by maxence on 05/04/2021.
//


#include "gtest/gtest.h"
#include "program.h"

void testVectorEqual(const std::vector<double>& v1, const std::vector<double>& v2)
{
    ASSERT_EQ(v1.size(), v2.size()) << "vectors size differ";
    for (int i=0; i<v1.size(); i++){
        ASSERT_EQ(v1[i], v2[i]) << "vectors elements: "<< i <<" differ.";
    }
}

TEST (jit, simple_addition){
    auto data = R""""(
                1 + 2;
                )"""";
     std::vector<double> expected{3};

    auto program = ckalei::Program(data);
    auto res = *program.evaluate();
    testVectorEqual(expected, res);
}

TEST (jit, multiple_operation){
    auto data = R""""(
                1 + 2; 3*4-1;
                1 - 2 - 4
                )"""";
    std::vector<double> expected{3, 11, -5};

    auto program = ckalei::Program(data);
    auto res = *program.evaluate();
    testVectorEqual(expected, res);
}

TEST (jit, function_call){
    auto data = R""""(
                def mult(a b) a*b;
                mult(3 4)
                )"""";
    std::vector<double> expected{12};

    auto program = ckalei::Program(data);
    auto res = *program.evaluate();
    testVectorEqual(expected, res);
}

TEST (jit, double_function_call){
    auto data = R""""(
                def mult(a b) a*b;
                mult(3 4);
                mult(4 5);
                )"""";
    std::vector<double> expected{12, 20};

    auto program = ckalei::Program(data);
    auto res = *program.evaluate();
    testVectorEqual(expected, res);
}

TEST (jit, internal_call){
    auto data = R""""(
                def mult(a b) a*b;
                def add(a b) a+b;
                add(mult(2 3) 1)
                )"""";
    std::vector<double> expected{7};

    auto program = ckalei::Program(data);
    std::cout << program.ppformat();
    auto res = *program.evaluate();
    testVectorEqual(expected, res);
}

TEST (jit, external){
    auto data = R""""(
                extern sin(x)
                sin(1.0)
                )"""";
    std::vector<double> expected{sin(1.0)};

    auto program = ckalei::Program(data);
    auto res = *program.evaluate();
    testVectorEqual(expected, res);
}
