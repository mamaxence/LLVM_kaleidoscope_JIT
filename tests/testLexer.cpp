//
// Created by maxence on 21/03/2021.
//

#include <cstdio>

#include "gtest/gtest.h"
#include "lexer.h"


void assertTokEof(ckalei::Lexer lexer)
{
    auto tok = lexer.getTok();
    ASSERT_EQ(tok, ckalei::tok_def);
}

void assertTokDef(ckalei::Lexer lexer)
{
    auto tok = lexer.getTok();
    ASSERT_EQ(tok, ckalei::tok_def);
}

void assertTokExtern(ckalei::Lexer lexer)
{
    auto tok = lexer.getTok();
    ASSERT_EQ(tok, ckalei::tok_extern);
}

void assertTokIdentifier(ckalei::Lexer lexer, std::string const& expected)
{
    auto tok = lexer.getTok();
    ASSERT_EQ(tok, ckalei::tok_identifier);
    ASSERT_EQ(lexer.getIdentifier(), expected);
}

void assertTokNumber(ckalei::Lexer lexer, double expected)
{
    auto tok = lexer.getTok();
    ASSERT_EQ(tok, ckalei::tok_number);
    ASSERT_EQ(lexer.getNumVal(), expected);
}

void assertTokOther(ckalei::Lexer lexer, int expected)
{
    auto tok = lexer.getTok();
    ASSERT_EQ(tok, ckalei::tok_other);
    ASSERT_EQ(lexer.getOtherChar(), expected);
}

TEST (lexer, simple){
    char * data = R""""(def id=1.2
            # coment
            extern val
            )"""";
    FILE * file = fopen("test.txt", "w+");
    fputs(data, file);
    fseek(file, 0, SEEK_SET);
    auto lexer = ckalei::Lexer(file);

    assertTokDef(lexer);
    assertTokIdentifier(lexer, "id");
    assertTokOther(lexer, '=');
    assertTokNumber(lexer, 1.2);
    assertTokExtern(lexer);
    assertTokIdentifier(lexer, "val");

    fclose(file);
}

