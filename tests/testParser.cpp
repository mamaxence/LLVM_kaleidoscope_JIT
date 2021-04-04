//
// Created by maxence on 21/03/2021.
//

#include "gtest/gtest.h"
#include "program.h"



TEST (parser, simple_addition){
    auto data = R""""(
                1 + 2
                )"""";
    auto expected =
            R""""(Function(
    Prototype((
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
    Prototype((
    )
    BinaryExpr(
        NumberExpr(1)
        *
        NumberExpr(2)
    )
)
)"""";
    auto program = ckalei::Program(data);
    ASSERT_EQ(program.ppformat(), expected);
}
/// Test a complex binary operation with parenthesis support
TEST (parser, combined_opperation){
    auto data = R""""(
                1 + a * (2 - 4) - b
                )"""";
    auto expected =
            R""""(Function(
    Prototype((
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
    Prototype((
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

//TEST (parser, variable_assignation){
//    auto data = R""""(
//                a = b = dd * 2
//                )"""";
//    auto expected =
//            R""""(Function(
//    Prototype((
//    )
//    BinaryExpr(
//        BinaryExpr(
//            NumberExpr(1)
//            +
//            BinaryExpr(
//                VariableExpr(a)
//                *
//                BinaryExpr(
//                    NumberExpr(2)
//                    -
//                    NumberExpr(4)
//                )
//            )
//        )
//        -
//        VariableExpr(b)
//    )
//)
//)"""";
//    testParser(data, expected);
//}
//
