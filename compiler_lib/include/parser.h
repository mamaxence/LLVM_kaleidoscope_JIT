//
// Created by maxence on 21/03/2021.
//

#ifndef LLVM_KALEIDOSCOPE_PARSER_H
#define LLVM_KALEIDOSCOPE_PARSER_H

#include <utility>

#include "ast.h"
#include "lexer.h"
#include "map"

namespace ckalei {

    const std::string ANONIMOUS_EXPR = "__anon_expr";

    class Parser {
    public:
        Parser(std::unique_ptr<Lexer> lexer) : lexer(std::move(lexer)), binopPrec(std::map<char, int>())
        {

            // Define operators priority, higher is better
            binopPrec['<'] = 10;
            binopPrec['+'] = 20;
            binopPrec['-'] = 20;
            binopPrec['*'] = 40;
        }

    public:
        /// parse input in lexer and get the list of computed ast nodes
        std::vector<std::unique_ptr<ASTNode>> getAstNodes();

    private:
        /// Parse top level expression
        /// A top level expression is an anonymous function (empty prototype)
        /// toplevelexpr ::= expr
        std::unique_ptr<FunctionAST> parseTopLevelExpr()
        {
            auto expr = parseExpr();
            if (!expr) {
                return nullptr;
            }
            return std::make_unique<FunctionAST>(
                    std::make_unique<PrototypeAST>(ANONIMOUS_EXPR, std::vector<std::string>()),
                    std::move(expr)
            );
        }

        // Handle expression parsing
    private:

        /// Parse an expr
        /// expression
        ///     ::= unary binoprhs
        std::unique_ptr<ExprAST> parseExpr();

        /// Parse primary expression
        /// primary
        ///   ::= identifierexpr
        ///   ::= numberexpr
        ///   ::= parenexpr
        ///   ::= ifThenElseExpr
        ///   ::= lorLoopExpr
        std::unique_ptr<ExprAST> parsePrimary();

        /// Parse a number expression
        ///     numberexpr ::= number
        std::unique_ptr<ExprAST> parseNumberExpr();

        /// Parse a parenthesis expression
        ///      parentexpr ::= ( expr )
        std::unique_ptr<ExprAST> parseParentExpr();

        /// Parse a function call
        /// identifierexpr
        ///     ::= identifier
        ///     ::= identifier ( expression* )
        std::unique_ptr<ExprAST> parseIdentifierExpr();

        /// Parse unary expression
        /// unary
        ///     ::= primary
        ///     ::= '!' unary
        std::unique_ptr<ExprAST> parseUnaryExpr();

        /// Parse binary expression
        /// exprPrec: precedence of the operator
        /// binoprhs
        ///     ::= ( '+' unary)*
        std::unique_ptr<ExprAST> parseBinOpRhs(int exprPrec, std::unique_ptr<ExprAST> lhs);

        /// Parse ifThenElse expression
        /// ifThenElseExpr
        ///     ::= 'if' expression 'then' expression'
        ///     ::= 'if' expression 'then' expression  'else' expression
        std::unique_ptr<ExprAST> parseIfThenElse();

        /// Parse for loop expression
        /// forLoopExpr
        ///     ::= 'for' id '=' expression "," expression "," expression "in" expression
        std::unique_ptr<ExprAST> parseForExpr();


        // Handle function definitions
    private:
        /// Parse a function prototype of form : fname(arg1 arg2 arg3)
        /// prototype
        ///     ::= id '(' id* ')'
        ///     :: unary LETTER (id)
        ///     :: binary LETTER number (id, id)
        std::unique_ptr<PrototypeAST> parsePrototype();

        /// Parse a function definition.
        /// a function definition is a function prototype followed by an expression
        /// def
        ///     ::= 'def' prototype expr
        std::unique_ptr<FunctionAST> parseDefinition();

        /// Parse Extern
        /// external
        ///     ::= 'extern' prototype
        std::unique_ptr<PrototypeAST> parseExtern();

        // Some utility func
    private:
        /// Return the newt token and update the curTok content
        Token getNextToken();

        /// Get the precedence of the current token. return -1 if not registered
        int getTokPrecedence();


        /// LogError* - These are little helper functions for error handling.
        std::unique_ptr<ExprAST> logError(const char *Str)
        {
            fprintf(stderr, "LogError: %s\n", Str);
            return nullptr;
        };

        std::unique_ptr<PrototypeAST> logErrorP(const char *Str)
        {
            logError(Str);
            return nullptr;
        }

    private:
        std::unique_ptr<Lexer> lexer;
        Token curTok; // current token
        std::map<char, int> binopPrec;  // defined operators
    };

}


#endif //LLVM_KALEIDOSCOPE_PARSER_H
