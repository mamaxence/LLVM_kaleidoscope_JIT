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
        } else if (curTok == tok_other && lexer->getOtherChar() == '('){
            return parseParentExpr();
        } else if(curTok == tok_if){
            return parseIfThenElse();
        } else if (curTok == tok_for) {
            return parseForExpr();
        } else if (curTok == tok_var){
            return parseDeclarationExpr();
        } else{
            return logError("unknown token when expecting an expression");
        }
    }

    std::unique_ptr<ExprAST> Parser::parseExpr()
    {
        auto lhs = parseUnaryExpr();
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

    std::unique_ptr<ExprAST> Parser::parseUnaryExpr()
    {
        if (curTok != tok_other){
            return std::move(parsePrimary());
        } else {
            char op = (char) lexer->getOtherChar();
            if (op == '(' || op == ','){
                return std::move(parsePrimary());
            }
            getNextToken(); // eat op
            auto expr = parseUnaryExpr();
            return std::make_unique<UnaryExprAST>(op, std::move(expr));
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
            auto rhs = parseUnaryExpr();
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

    std::unique_ptr<ExprAST> Parser::parseDeclarationExpr()
    {
        getNextToken(); // eat 'var'
        std::vector<std::pair<std::string, std::unique_ptr<ExprAST>>> vars;
        // parse var list
        while (true){
            if (curTok != tok_identifier){
                return logError("Expected identifier in declaration");
            }
            auto varName = lexer->getIdentifier();
            getNextToken(); // eat var name
            std::unique_ptr<ExprAST> varVal = nullptr;
            if (curTok == tok_other && lexer->getOtherChar() == '='){
                getNextToken(); // eat '='
                varVal = parseExpr();
            }
            vars.emplace_back(varName, std::move(varVal));
            if (curTok == tok_in){
                getNextToken(); // eat 'in'
                break;
            } else{
                if (curTok != tok_other && lexer->getOtherChar() != ','){return logError("Expected ','");}
                getNextToken(); // eat ','
            }
        }

        // parse body bloc
        auto body = parseExpr();
        if (!body){
            return logError("invalid body");
        }
        return std::make_unique<DeclarationExprAST>(std::move(body), std::move(vars));
    }

    std::unique_ptr<ExprAST> Parser::parseIfThenElse()
    {
        // If part
        getNextToken(); // eat if
        auto condExpr = parseExpr();
        if (not condExpr){
            return logError("Cond statement is invalid");
        }

        // Then part
        if (curTok != tok_then){
            return logError("Expected 'thenExpr'");
        }
        getNextToken(); // eat thenExpr
        auto thenExpr = parseExpr();
        if (not thenExpr){
            return logError("thenExpr content is invalid");
        }

        // Else part
        std::unique_ptr<ExprAST> elseExpr = nullptr;
        if (curTok == tok_else){
            getNextToken();
            elseExpr = parseExpr();
            if (not elseExpr){
                return logError("elseExpr content is invalid");
            }
            return std::make_unique<IfExprAST>(std::move(condExpr), std::move(thenExpr), std::move(elseExpr));
        } else{
            return std::make_unique<IfExprAST>(std::move(condExpr), std::move(thenExpr));
        }
    }

    std::unique_ptr<ExprAST> Parser::parseForExpr()
    {
        getNextToken(); // eat for

        if (curTok != tok_identifier){
            return logError("Expected Expression");
        }
        auto varName = lexer->getIdentifier();
        getNextToken(); // eat identifier

        if (curTok != tok_other && lexer->getOtherChar() != '='){
            return logError("Expected '='");
        }
        getNextToken(); // eat '='

        // start expr
        auto startExpr = parseExpr();
        if (not startExpr){
            return logError("startExpr content is invalid");
        }
        if (curTok != tok_other && lexer->getOtherChar() != ','){
            return logError("Expected ','");
        }
        getNextToken(); // eat ','

        // end expr
        auto endExpr = parseExpr();
        if (not endExpr){
            return logError("endExpr content is invalid");
        }
        if (curTok != tok_other && lexer->getOtherChar() != ','){
            return logError("Expected ','");
        }
        getNextToken(); // eat ','

        // step expr
        auto stepExpr = parseExpr();
        if (not stepExpr){
            return logError("stepExpr content is invalid");
        }

        if (curTok != tok_in){
            return logError("Expected 'in'");
        }
        getNextToken(); // eat 'in'

        auto bodyExpr = parseExpr();
        if (not bodyExpr){
            return logError("bodyExpr content is invalid");
        }

        return std::make_unique<ForExprAST>(std::move(startExpr),
                                            std::move(stepExpr),
                                            std::move(endExpr),
                                            std::move(bodyExpr),
                                            varName);
    }

    enum ProtoKind{
        function,
        binary,
        unary,
    };

    std::unique_ptr<PrototypeAST> Parser::parsePrototype()
    { // TODO bug if unary / (a b) -> create a binary op with precedence 0
        // Parse a function prototype
        bool isOperator = false;
        unsigned precedence = 0;
        std::string name;
        ProtoKind kind;

        if (curTok == tok_identifier){
            name = lexer->getIdentifier();
            getNextToken();
            kind = ProtoKind::function;
        } else if (curTok == tok_binary) {// Parse a binary op declaration : "binary" LETTER number
            getNextToken(); // eat binary
            if (curTok != tok_other){return logErrorP("expected operator");}
            name = "binary";
            char op = (char) lexer->getOtherChar();
            name.push_back(op);
            isOperator = true;
            getNextToken(); // eat operator name
            if (curTok != tok_number){return logErrorP("Expected precedence");}
            if (lexer->getNumVal() < 1){return logErrorP("Precedence must be > 1");}
            precedence = (int) lexer->getNumVal();
            getNextToken(); // eat precedence val
            kind = ProtoKind::binary;
            binopPrec[op] = precedence;
        } else if (curTok == tok_unary){ // Parse unary operator declaration
            getNextToken(); // eat unary
            if (curTok != tok_other){return logErrorP("expected operator");}
            char op = (char) lexer->getOtherChar();
            name = "unary";
            name.push_back(op);
            isOperator = true;
            getNextToken(); // eat operator name
            kind = ProtoKind::unary;
        }else{
            return logErrorP("Expected function name in prototype");
        }

        // parse args section
        if (curTok != tok_other || lexer->getOtherChar() != '('){return logErrorP("Expected (");}

        auto argNames = std::vector<std::string>();
        while (getNextToken() == tok_identifier){
            argNames.push_back(lexer->getIdentifier());
        }
        if (curTok != tok_other || lexer->getOtherChar() != ')'){return logErrorP("Expected )");};
        getNextToken(); // eat )

        // Check args number consistency
        if (kind == binary && argNames.size() != 2){ return logErrorP("Binary op need two args");}
        else if (kind == unary && argNames.size() != 1){ return logErrorP("Binary op need one arg");}

        return std::make_unique<PrototypeAST>(name, std::move(argNames), isOperator, precedence);
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
