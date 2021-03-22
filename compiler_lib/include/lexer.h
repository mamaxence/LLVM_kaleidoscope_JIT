//
// (Very simple) tokenizer for the kaleidoscope language
//

#ifndef LLVM_KALEIDOSCOPE_LEXER_H
#define LLVM_KALEIDOSCOPE_LEXER_H


#include <string>
#include <utility>

namespace ckalei{
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
        explicit Lexer(std::string inputText): inputText(std::move(inputText)), lastChar(' '){
            iteText = this->inputText.begin();
        };
        Lexer(const Lexer&) = delete;  // disable copy constructor because it do not copy iterator state

        // return the next token from standard input
        Token getTok();

        std::string getIdentifier(){return identifierStr;}
        [[nodiscard]] double getNumVal() const{return numVal;}
        [[nodiscard]] int getOtherChar() const{return otherChar;}

    private:
        /// return the next char in the stream
        int nextChar();
        std::string inputText;
        std::string::iterator iteText;
        int lastChar;

        std::string identifierStr; // Filled if tok_identifier
        double numVal{}; // Filled if tok_number
        int otherChar{}; // Filled if tok_other
    };
}

#endif //LLVM_KALEIDOSCOPE_LEXER_H
