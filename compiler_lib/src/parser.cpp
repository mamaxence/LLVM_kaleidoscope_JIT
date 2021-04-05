//
// Created by maxence on 21/03/2021.
//

#include "parser.h"

namespace ckalei {
    Token Parser::getNextToken()
    {
        return curTok = lexer->getTok();
    }

    std::unique_ptr<ExprAST> Parser::parseNumberExpr()
    {
        auto result = std::make_unique<NumberExprAST>(lexer->getNumVal());
        getNextToken();
        return std::move(result);
    }

    std::unique_ptr<ExprAST> Parser::parseParentExpr()
    {
        getNextToken(); // eat (
        auto content = parseExpr();
        if (!content){ // if there is nothing between parenthesis
            return nullptr;
        }
        if (lexer->getOtherChar() != ')'){
            return logError("expected ')'");
        }
        getNextToken();
        return content;

    }

    std::unique_ptr<ExprAST> Parser::parseIdentifierExpr()
    {
        auto idName = lexer->getIdentifier();
        getNextToken();

        if (curTok!= tok_other || lexer->getOtherChar() != '('){ // this is not an function call
            return std::make_unique<VariableExprAST>(idName);
        }

        // this is a function call
        getNextToken(); // eat (
        auto args = std::vector<std::unique_ptr<ExprAST>>();
        while (curTok != tok_other || lexer->getOtherChar() != ')'){
            args.push_back(parseExpr());
        }
        if (curTok != tok_other || lexer->getOtherChar() != ')'){
            return logError("expected ')'");
        }
        getNextToken(); // eat )
        return std::make_unique<CallExprAST>(idName, std::move(args));
    }

    std::unique_ptr<ExprAST> Parser::parsePrimary()
    {
        if (curTok == tok_identifier) {
            return parseIdentifierExpr();
        } else if (curTok == tok_number){
            return parseNumberExpr();
        } else if (lexer->getOtherChar() == '('){
            return parseParentExpr();
        } else{
            return logError("unknown token when expecting an expression");
        }
    }

    std::unique_ptr<ExprAST> Parser::parseExpr()
    {
        auto lhs = parsePrimary();
        if (!lhs){
            getNextToken();
            return nullptr;
        }
        return parseBinOpRhs(0, std::move(lhs));
    }

    int Parser::getTokPrecedence()
    {
        if (curTok != tok_other){
            return -1;
        }

        int currentChar = lexer->getOtherChar();
        if (!isascii(currentChar)){
            return -1;
        }
        if (!binopPrec.contains(currentChar)){
            return -1;
        } else{
            return binopPrec[currentChar];
        }
    }

    std::unique_ptr<ExprAST> Parser::parseBinOpRhs(int exprPrec, std::unique_ptr<ExprAST> lhs)
    {
        while (true){
            int tokPrec = getTokPrecedence();
            if (tokPrec < exprPrec){
                return lhs;
            }
            // here we know we have a bin expression
            int binaryOp = lexer->getOtherChar();
            getNextToken();
            auto rhs = parsePrimary();
            if (!rhs){
                return nullptr;
            }
            int nextPrec = getTokPrecedence();
            if (tokPrec < nextPrec){
                rhs = parseBinOpRhs(tokPrec+1, std::move(rhs));
                if (!rhs){
                    return nullptr;
                }
            }
            lhs = std::make_unique<BinaryExprAST>(std::move(lhs), std::move(rhs), binaryOp);
        }
    }

    std::unique_ptr<PrototypeAST> Parser::parsePrototype()
    {
        if (curTok != tok_identifier){
            return logErrorP("Expected function name in prototype");
        }
        auto fname = lexer->getIdentifier();
        getNextToken();

        if (curTok != tok_other || lexer->getOtherChar() != '('){
            return logErrorP("Expected (");
        }

        auto argNames = std::vector<std::string>();
        while (getNextToken() == tok_identifier){
            argNames.push_back(lexer->getIdentifier());
        }
        getNextToken(); // eat )
        return std::make_unique<PrototypeAST>(fname, std::move(argNames));
    }

    std::unique_ptr<FunctionAST> Parser::parseDefinition()
    {
        getNextToken(); // eat def
        auto proto = parsePrototype();
        if (!proto){
            return nullptr;
        }
        auto expr = parseExpr();
        if (!expr){
            return nullptr;
        }
        return std::make_unique<FunctionAST>(std::move(proto), std::move(expr));
    }

    std::unique_ptr<PrototypeAST> Parser::parseExtern()
    {
        getNextToken(); // eat extern
        return parsePrototype();
    }

    std::vector<std::unique_ptr<ASTNode>> Parser::getAstNodes()
    {
        auto res = std::vector<std::unique_ptr<ASTNode>>();
        getNextToken(); // get first token;
        while (true){
            if (curTok == tok_eof){
                return res;
            } else if (curTok == tok_def){
                res.push_back(std::move(parseDefinition()));
            } else if(curTok == tok_other && lexer->getOtherChar() == ';')
                {getNextToken();}// do nothing
            else if (curTok == tok_extern){
                res.push_back(std::move(parseExtern()));
            } else{
                res.push_back(std::move(parseTopLevelExpr()));
            }
        }
    }
}
