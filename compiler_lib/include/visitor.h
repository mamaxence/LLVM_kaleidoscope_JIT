//
// Created by maxence on 23/03/2021.
//

#ifndef LLVM_KALEIDOSCOPE_VISITOR_H
#define LLVM_KALEIDOSCOPE_VISITOR_H

#include <iostream>
#include <map>


#include "ast.h"
#include "llvm/IR/Value.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/IRBuilder.h"

namespace ckalei{

    class NumberExprAST;
    class VariableExprAST;
    class BinaryExprAST;
    class CallExprAST;
    class PrototypeAST;
    class FunctionAST;

    class Visitor{
    public:
        virtual void visit(const NumberExprAST& node) = 0;
        virtual void visit(const VariableExprAST& node) = 0;
        virtual void visit(const BinaryExprAST& node) = 0;
        virtual void visit(const CallExprAST& node) = 0;
        virtual void visit(const PrototypeAST& node) = 0;
        virtual void visit(const FunctionAST& node) = 0;
    };

    class CodeGenVisitor: public Visitor{

    public:
        void visit(const NumberExprAST& node) override;
        void visit(const VariableExprAST& node) override;
        void visit(const BinaryExprAST& node) override;
        void visit(const CallExprAST& node) override;
        void visit(const PrototypeAST& node) override;
        void visit(const FunctionAST& node) override;

    private:
        llvm::Value* logErrorV(const char* str){
            fprintf(stderr, "LogError: %s\n", str);
            return nullptr;
        }

        llvm::Value lastValue;
        llvm::LLVMContext context;
        llvm::IRBuilder<> builder;
        std::unique_ptr<llvm::Module> module;
        std::map<std::string, llvm::Value *> namedValues;

    };

    /// Visitor for producing prety print of ast
    class PPrintorVisitor: public Visitor{

    public:
        PPrintorVisitor(): inc(0){};

        /// Pretty print NumberExpr:
        /// NumberExpr($val)
        void visit(const NumberExprAST& node) override;
        /// Pretty print VariableExpr
        /// VariableExpr($varname)
        void visit(const VariableExprAST& node) override;
        /// Pretty print BinaryExpr
        /// BinaryExpr(
        ///     $leftOp
        ///     $op
        ///     $rightOp
        /// )
        void visit(const BinaryExprAST& node) override;
        /// Pretty print CallExpr
        /// CallExpr($callee(
        ///     $args*
        /// )
        void visit(const CallExprAST& node) override;
        /// Pretty print Prototype
        /// Prototype($name(
        ///     $args*
        /// )
        void visit(const PrototypeAST& node) override;
        /// Pretty print Function
        /// Function(
        ///     $prototype
        ///     $expr
        /// )
        void visit(const FunctionAST& node) override;

        [[nodiscard]] const std::string &getStr() const;

    private:
        /// Get the prefix for a logged line
        [[nodiscard]] std::string getLinePrefix() const;
        int inc;
        std::string str;
    };

}

#endif //LLVM_KALEIDOSCOPE_VISITOR_H
