//
// Created by maxence on 21/03/2021.
//

#include "lexer.h"
namespace ckalei{

    Token Lexer::getTok()
    {

        // skip whitespace
        while (isspace(lastChar)){
            lastChar = nextChar();
        }

        // parse identifier (token starting with alpha num)
        if (isalpha(lastChar)){
            identifierStr = "";
            identifierStr += lastChar;

            while (isalnum(lastChar = nextChar())){
                identifierStr += lastChar;
            }

            if (identifierStr == "def"){
                return tok_def;
            } else if (identifierStr == "extern"){
                return tok_extern;
            } else if (identifierStr == "if"){
                return tok_if;
            } else if (identifierStr == "then"){
                return tok_then;
            } else if (identifierStr == "else"){
                return tok_else;
            } else if (identifierStr == "for"){
                return tok_for;
            } else if (identifierStr == "in"){
                return tok_in;
            }
            return tok_identifier;
        }

        // parse number (token starting with [0-9,.])
        if (isdigit(lastChar) || lastChar == '.'){
            std::string numStr;
            do {
                numStr += lastChar;
                lastChar = nextChar();
            } while (isdigit(lastChar) || lastChar == '.');
            numVal = strtod(numStr.c_str(), nullptr);
            return tok_number;
        }

        // parse comment
        if (lastChar == '#'){
            do {
                lastChar = nextChar();
            } while (lastChar != '\n' && lastChar != EOF && lastChar != '\r');

            if (lastChar != EOF){
                return getTok();
            }
        }

        if (lastChar == EOF){
            return tok_eof;
        }

        otherChar = lastChar;
        lastChar = nextChar();
        return tok_other;
    }

    int Lexer::nextChar()
    {
        if (iteText != inputText.end()){
            return *(iteText++);
        }
        return EOF;
    }
} // ckalei

