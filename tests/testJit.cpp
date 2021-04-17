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

TEST (jit, simple_if){
    auto data = R""""(
    def foo(x)
    if x then
        x * 2
    else
        0.1
    foo(3))"""";
    std::vector<double> expected{6.0};

    auto program = ckalei::Program(data);
    auto res = *program.evaluate();
    testVectorEqual(expected, res);
}

TEST (jit, if_in_if){
    auto data = R""""(
    def foo(x y)
    if x then
        if y then x + y else 1001
    else
        if y then 0 else 1000
    foo(4 2)
    foo(4 0)
    foo(0 2)
    foo(0 0)
)"""";
    std::vector<double> expected{6.0, 1001.0, 0.0, 1000.0};

    auto program = ckalei::Program(data);
    auto res = *program.evaluate();
    testVectorEqual(expected, res);
}

TEST (jit, for){
    auto data = R""""(
    def foo(x y z)
        for i = x, i < y, 1 in z + 1
    foo(1 10 2)
)"""";
    std::vector<double> expected{0.0}; // TODO improve when go variable initialisation

    auto program = ckalei::Program(data);
    auto res = *program.evaluate();
    testVectorEqual(expected, res);
}

TEST (jit, binop){
    auto data = R""""(
    def binary | 10 (a b)
        if (a + b) then
            1
        else
            0
    def binary & 10 (a b)
        if (a * b) then
            1
        else
            0
    1 | 1;
    0 | 1;
    1 | 0;
    0 | 0;
    1 * 1;
    0 * 1;
    1 * 0;
    0 * 0;
    )"""";
    std::vector<double> expected{1, 1, 1, 0, 1, 0, 0, 0};
    auto program = ckalei::Program(data);
    auto res = *program.evaluate();
    testVectorEqual(expected, res);
}

TEST (jit, unary_op){
    auto data = R""""(
    def unary - (v)
        0 - v;
    def unary ! (v)
        if v then
            0
        else
            1;
    -!0;
    2 * -1;
    ! (2* -2);
    )"""";
    std::vector<double> expected{-1, -2, 0};
    auto program = ckalei::Program(data);
    auto res = *program.evaluate();
    testVectorEqual(expected, res);
}

TEST (jit, rec_fib){
    auto data = R""""(
        def fib(x)
            if (x < 3) then
                1
            else
                fib(x-1)+fib(x-2);
        fib(3)
        fib(4)
        fib(5)
        fib(10)
    )"""";
    std::vector<double> expected{2, 3, 5, 55};
    auto program = ckalei::Program(data);
    auto res = *program.evaluate();
    testVectorEqual(expected, res);
}
//
//TEST (jit, variableAssigment){
//    auto data = R""""(
//        a = 1;
//        a
//    )"""";
//    std::vector<double> expected{1};
//    auto program = ckalei::Program(data);
//    auto res = *program.evaluate();
//    testVectorEqual(expected, res);
//}
