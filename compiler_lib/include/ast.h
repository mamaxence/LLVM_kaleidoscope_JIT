//
// Created by maxence on 21/03/2021.
//

#ifndef LLVM_KALEIDOSCOPE_AST_H
#define LLVM_KALEIDOSCOPE_AST_H

#include <string>
#include <utility>
#include <memory>
#include <vector>

#include "visitor.h"


namespace ckalei {

    class Visitor;

    /// Base class for ast nodes
    class ASTNode{
    public:
        virtual void accept(Visitor& visitor) = 0;
        virtual ~ASTNode() = default;
    };

    /// Base class for all expression node
    class ExprAST: public ASTNode {
    public:
        virtual void accept(Visitor& visitor) = 0;
        virtual ~ExprAST() = default;
    };


    /// Node representing a number
    class NumberExprAST : public ExprAST {
    public:
        explicit NumberExprAST(double val) : val(val)
        {};
        void accept(Visitor& visitor);
        double getVal() const {return val;}

    private:
        double val;
    };

    /// Node representing a variable name
    class VariableExprAST : public ExprAST {
    public:
        explicit VariableExprAST(std::string name) : name(std::move(name))
        {};
        void accept(Visitor& visitor);
        const std::string &getName() const {return name;}

    private:
        std::string name;
    };

    /// Node representing a binary expressing
    class BinaryExprAST : public ExprAST {
    public:
        BinaryExprAST(std::unique_ptr<ExprAST> leftExpr,
                      std::unique_ptr<ExprAST> rightExpr,
                      char op) : rightExpr(std::move(rightExpr)), leftExpr(std::move(leftExpr)), op(op)
        {};
        void accept(Visitor& visitor);

        char getOp() const {return op;}
        const std::unique_ptr<ExprAST> &getRightExpr() const {return rightExpr;}
        const std::unique_ptr<ExprAST> &getLeftExpr() const {return leftExpr;}

    private:
        char op; // operation of the expression
        std::unique_ptr<ExprAST>leftExpr, rightExpr;
    };

    /// Node representing a function call
    class CallExprAST : public ExprAST {
    public:
        CallExprAST(std::string callee,
                    std::vector<std::unique_ptr<ExprAST>> args) : callee(std::move(callee)), args(std::move(args))
        {};
        void accept(Visitor& visitor);

        const std::string &getCallee() const {return callee;}
        const std::vector<std::unique_ptr<ExprAST>> &getArgs() const {return args;}

    private:
        std::string callee;
        std::vector<std::unique_ptr<ExprAST>> args;
    };

    /// Node representing a function prototype
    class PrototypeAST: public ASTNode{
    public:
        PrototypeAST(std::string name, std::vector<std::string> args): name(std::move(name)), args(std::move(args)) {};

        void accept(Visitor& visitor);
        const std::string &getName() const {return name;}
        const std::vector<std::string> &getArgs() const {return args;}

    private:
        std::string name;
        std::vector<std::string> args;
    };

    /// Node representing a function declaration
    class FunctionAST: public ASTNode{
    public:
        FunctionAST(std::unique_ptr<PrototypeAST> proto,
                    std::unique_ptr<ExprAST> body): proto(std::move(proto)), body(std::move(body)) {};

        void accept(Visitor& visitor);
        const std::unique_ptr<PrototypeAST> &getProto() const {return proto;}
        const std::unique_ptr<ExprAST> &getBody() const {return body;}

    private:
        std::unique_ptr<PrototypeAST> proto;
        std::unique_ptr<ExprAST> body;
    };


}

#endif //LLVM_KALEIDOSCOPE_AST_H
