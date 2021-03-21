//
// (Very simple) tokenizer for the kaleidoscope language
//

#ifndef LLVM_KALEIDOSCOPE_LEXER_H
#define LLVM_KALEIDOSCOPE_LEXER_H


#include <string>

namespace kalei_comp{
// tokens returned by the lexer
    enum Token{
        // token end of file
        tok_eof,

        // commands
        tok_def,
        tok_extern,

        // identifier
        tok_identifier,
        tok_number,

        //other
        tok_other,
    };

    // Simple lexer class
    class Lexer{
    public:
        explicit Lexer(FILE* stream): stream(stream){};

        // return the next token from standard input
        Token getTok();

        std::string getIdentifier(){return identifierStr;}
        [[nodiscard]] double getNumVal() const{return numVal;}
        [[nodiscard]] int getOtherChar() const{return otherChar;}

    private:
        FILE * stream;
        std::string identifierStr; // Filled if tok_identifier
        double numVal{}; // Filled if tok_number
        int otherChar{}; // Filled if tok_other
    };
}

#endif //LLVM_KALEIDOSCOPE_LEXER_H
