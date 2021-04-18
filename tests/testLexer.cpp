//
// Created by maxence on 21/03/2021.
//

#include <cstdio>

#include "gtest/gtest.h"
#include "lexer.h"


void assertTokEof(ckalei::Lexer &lexer)
{
    auto tok = lexer.getTok();
    ASSERT_EQ(tok, ckalei::tok_def);
}

void assertTokDef(ckalei::Lexer &lexer)
{
    auto tok = lexer.getTok();
    ASSERT_EQ(tok, ckalei::tok_def);
}

void assertTokExtern(ckalei::Lexer &lexer)
{
    auto tok = lexer.getTok();
    ASSERT_EQ(tok, ckalei::tok_extern);
}

void assertTokIdentifier(ckalei::Lexer &lexer, std::string const& expected)
{
    auto tok = lexer.getTok();
    ASSERT_EQ(tok, ckalei::tok_identifier);
    ASSERT_EQ(lexer.getIdentifier(), expected);
}

void assertTokNumber(ckalei::Lexer &lexer, double expected)
{
    auto tok = lexer.getTok();
    ASSERT_EQ(tok, ckalei::tok_number);
    ASSERT_EQ(lexer.getNumVal(), expected);
}

void assertTokOther(ckalei::Lexer &lexer, int expected)
{
    auto tok = lexer.getTok();
    ASSERT_EQ(tok, ckalei::tok_other);
    ASSERT_EQ(lexer.getOtherChar(), expected);
}

void assertTok(ckalei::Lexer &lexer, ckalei::Token tok){
    ASSERT_EQ(lexer.getTok(), tok);
}

TEST (lexer, simple){
    auto data = R""""(def id=1.2
            # comment
            extern val
            )"""";
    auto lexer = ckalei::Lexer(data);

    assertTokDef(lexer);
    assertTokIdentifier(lexer, "id");
    assertTokOther(lexer, '=');
    assertTokNumber(lexer, 1.2);
    assertTokExtern(lexer);
    assertTokIdentifier(lexer, "val");
}

TEST (lexer, simple2){
    auto data = R""""(
                a = b = dd * 2
                )"""";
    auto lexer = ckalei::Lexer(data);

    assertTokIdentifier(lexer, "a");
    assertTokOther(lexer, '=');
    assertTokIdentifier(lexer, "b");
    assertTokOther(lexer, '=');
    assertTokIdentifier(lexer, "dd");
    assertTokOther(lexer, '*');
    assertTokNumber(lexer, 2);
}
TEST (lexer, simple3){
    auto data = R""""(
                def mult(v1 v2)
                )"""";
    auto lexer = ckalei::Lexer(data);

    assertTokDef(lexer);
    assertTokIdentifier(lexer, "mult");
    assertTokOther(lexer, '(');
    assertTokIdentifier(lexer, "v1");
    assertTokIdentifier(lexer, "v2");
    assertTokOther(lexer, ')');
}

TEST (lexer, control_for){
    auto data = R""""(
            if
            then
            else
            for
            in)"""";
    auto lexer = ckalei::Lexer(data);
    assertTok(lexer, ckalei::tok_if);
    assertTok(lexer, ckalei::tok_then);
    assertTok(lexer, ckalei::tok_else);
    assertTok(lexer, ckalei::tok_for);
    assertTok(lexer, ckalei::tok_in);
}

TEST (lexer, operators){
    auto data = R""""(
            binary
            unary)"""";
    auto lexer = ckalei::Lexer(data);
    assertTok(lexer, ckalei::tok_binary);
    assertTok(lexer, ckalei::tok_unary);
}

TEST (lexer, var){
    auto data = R""""(
            var a = 1)"""";
    auto lexer = ckalei::Lexer(data);
    assertTok(lexer, ckalei::tok_var);
    assertTokIdentifier(lexer, "a");
    assertTokOther(lexer, '=');
    assertTokNumber(lexer, 1);
}
