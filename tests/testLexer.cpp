//
// Created by maxence on 21/03/2021.
//

#include <cstdio>

#include "gtest/gtest.h"
#include "lexer.h"


void assertTokEof(kalei_comp::Lexer lexer)
{
    auto tok = lexer.getTok();
    ASSERT_EQ(tok, kalei_comp::tok_def);
}

void assertTokDef(kalei_comp::Lexer lexer)
{
    auto tok = lexer.getTok();
    ASSERT_EQ(tok, kalei_comp::tok_def);
}

void assertTokExtern(kalei_comp::Lexer lexer)
{
    auto tok = lexer.getTok();
    ASSERT_EQ(tok, kalei_comp::tok_extern);
}

void assertTokIdentifier(kalei_comp::Lexer lexer, std::string const& expected)
{
    auto tok = lexer.getTok();
    ASSERT_EQ(tok, kalei_comp::tok_identifier);
    ASSERT_EQ(lexer.getIdentifier(), expected);
}

void assertTokNumber(kalei_comp::Lexer lexer, double expected)
{
    auto tok = lexer.getTok();
    ASSERT_EQ(tok, kalei_comp::tok_number);
    ASSERT_EQ(lexer.getNumVal(), expected);
}

void assertTokOther(kalei_comp::Lexer lexer, int expected)
{
    auto tok = lexer.getTok();
    ASSERT_EQ(tok, kalei_comp::tok_other);
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
    auto lexer = kalei_comp::Lexer(file);

    assertTokDef(lexer);
    assertTokIdentifier(lexer, "id");
    assertTokOther(lexer, '=');
    assertTokNumber(lexer, 1.2);
    assertTokExtern(lexer);
    assertTokIdentifier(lexer, "val");

    fclose(file);
}

