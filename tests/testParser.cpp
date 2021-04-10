//
// Created by maxence on 21/03/2021.
//

#include "gtest/gtest.h"
#include "program.h"


TEST (parser, simple_addition){
    auto data = R""""(
                1 + 2;
                )"""";
    auto expected =
            R""""(Function(
    Prototype(__anon_expr(
    )
    BinaryExpr(
        NumberExpr(1)
        +
        NumberExpr(2)
    )
)
)"""";

    auto program = ckalei::Program(data);
    ASSERT_EQ(program.ppformat(), expected);
}

TEST (parser, simple_multiplication){
    auto data = R""""(
                1 * 2
                )"""";
    auto expected =
            R""""(Function(
    Prototype(__anon_expr(
    )
    BinaryExpr(
        NumberExpr(1)
        *
        NumberExpr(2)
    )
)
)"""";
    auto program = ckalei::Program(data);
    auto res = *program.evaluate();
    ASSERT_EQ(program.ppformat(), expected);
}
/// Test a complex binary operation with parenthesis support
TEST (parser, combined_opperation){
    auto data = R""""(
                1 + a * (2 - 4) - b
                )"""";
    auto expected =
            R""""(Function(
    Prototype(__anon_expr(
    )
    BinaryExpr(
        BinaryExpr(
            NumberExpr(1)
            +
            BinaryExpr(
                VariableExpr(a)
                *
                BinaryExpr(
                    NumberExpr(2)
                    -
                    NumberExpr(4)
                )
            )
        )
        -
        VariableExpr(b)
    )
)
)"""";
    auto program = ckalei::Program(data);
    ASSERT_EQ(program.ppformat(), expected);
}


TEST (parser, function_definition){
    auto data = R""""(
                def mult (v1 v2)
                v1 * v2;
                )"""";
    auto expected =
    R""""(Function(
    Prototype(mult(
        v1
        v2
    )
    BinaryExpr(
        VariableExpr(v1)
        *
        VariableExpr(v2)
    )
)
)"""";
    auto program = ckalei::Program(data);
    ASSERT_EQ(program.ppformat(), expected);
}

TEST (parser, externdef){
    auto data = R""""(
                extern mult(v1 v2)
                )"""";
    auto expected =
            R""""(Prototype(mult(
    v1
    v2
)
)"""";
    auto program = ckalei::Program(data);
    ASSERT_EQ(program.ppformat(), expected);
}

TEST (parser, callexpr){
    auto data = R""""(
                mult(1.34*a b)
                )"""";
    auto expected =
            R""""(Function(
    Prototype(__anon_expr(
    )
    CallExpr(mult(
        BinaryExpr(
            NumberExpr(1.34)
            *
            VariableExpr(a)
        )
        VariableExpr(b)
    )
)
)"""";
    auto program = ckalei::Program(data);
    ASSERT_EQ(program.ppformat(), expected);
}


TEST (parser, extern_def){
    auto data = R""""(
    extern foo(a);
    def foo(b) b;
    foo(1);
                )"""";
    auto expected =
            R""""(Prototype(foo(
    a
)
Function(
    Prototype(foo(
        b
    )
    VariableExpr(b)
)
Function(
    Prototype(__anon_expr(
    )
    CallExpr(foo(
        NumberExpr(1)
    )
)
)"""";
    auto program = ckalei::Program(data);
    ASSERT_EQ(program.ppformat(), expected);
}

TEST (parser, recursive_call){
    auto data = R""""(
    add(mult(2 3) 1)
    (1+(3*2))
                )"""";
    auto expected =
            R""""(Function(
    Prototype(__anon_expr(
    )
    CallExpr(add(
        CallExpr(mult(
            NumberExpr(2)
            NumberExpr(3)
        )
        NumberExpr(1)
    )
)
Function(
    Prototype(__anon_expr(
    )
    BinaryExpr(
        NumberExpr(1)
        +
        BinaryExpr(
            NumberExpr(3)
            *
            NumberExpr(2)
        )
    )
)
)"""";
    auto program = ckalei::Program(data);
    std::cout << program.ppformat();
    ASSERT_EQ(program.ppformat(), expected);
}

TEST (parser, if_then){
    auto data = R""""(
        if 1 then a
                )"""";
    auto expected =
            R""""(Function(
    Prototype(__anon_expr(
    )
    IfExpr(
        NumberExpr(1)
        VariableExpr(a)
    )
)
)"""";
    auto program = ckalei::Program(data);
    std::cout << program.ppformat();
    ASSERT_EQ(program.ppformat(), expected);
}

TEST (parser, if_then_else){
    auto data = R""""(
        if 1 then a else b
                )"""";
    auto expected =
            R""""(Function(
    Prototype(__anon_expr(
    )
    IfExpr(
        NumberExpr(1)
        VariableExpr(a)
        VariableExpr(b)
    )
)
)"""";
    auto program = ckalei::Program(data);
    std::cout << program.ppformat();
    ASSERT_EQ(program.ppformat(), expected);
}
