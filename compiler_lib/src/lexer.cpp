//
// Created by maxence on 21/03/2021.
//

#include "lexer.h"
namespace kalei_comp{

    Token Lexer::getTok()
    {
        static int lastChar = ' ';

        // skip whitespace
        while (isspace(lastChar)){
            lastChar = getc(stream);
        }

        // parse identifier (token starting with alpha num)
        if (isalpha(lastChar)){
            identifierStr = "";
            identifierStr += lastChar;

            while (isalnum(lastChar = getc(stream))){
                identifierStr += lastChar;
            }

            if (identifierStr == "def"){
                return tok_def;
            }
            if (identifierStr == "extern"){
                return tok_extern;
            }
            return tok_identifier;
        }

        // parse number (token starting with [0-9,.])
        if (isdigit(lastChar) || lastChar == '.'){
            std::string numStr;
            do {
                numStr += lastChar;
                lastChar = getc(stream);
            } while (isdigit(lastChar) || lastChar == '.');
            numVal = strtod(numStr.c_str(), nullptr);
            return tok_number;
        }

        // parse comment
        if (lastChar == '#'){
            do {
                lastChar = getc(stream);
            } while (lastChar != '\n' && lastChar != EOF && lastChar != '\r');

            if (lastChar != EOF){
                return getTok();
            }
        }

        if (lastChar == EOF){
            return tok_eof;
        }

        otherChar = lastChar;
        lastChar = getc(stream);
        return tok_other;
    }
}

