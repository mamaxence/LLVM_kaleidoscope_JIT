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
#include "llvm/IR/Function.h"
#include "llvm/IR/Verifier.h"

namespace ckalei{

    class ASTNode;
    class NumberExprAST;
    class VariableExprAST;
    class BinaryExprAST;
    class CallExprAST;
    class PrototypeAST;
    class FunctionAST;

    class Visitor{
    public:
        virtual void visit(NumberExprAST& node) = 0;
        virtual void visit(VariableExprAST& node) = 0;
        virtual void visit(BinaryExprAST& node) = 0;
        virtual void visit(CallExprAST& node) = 0;
        virtual void visit(PrototypeAST& node) = 0;
        virtual void visit(FunctionAST& node) = 0;
    };

    class CodeGenVisitor: public Visitor{

    public:
        /// Generate code for NumberExpr. Set lastValue.
        void visit(NumberExprAST& node) override;
        /// Generate code for VariableExpr. Set lastValue.
        void visit(VariableExprAST& node) override;
        /// Generate code for Binary expression. Set lastValue.
        void visit(BinaryExprAST& node) override;
        /// Generate code for CallExpr AST. Set lastValue.
        void visit(CallExprAST& node) override;
        /// Generate code for Prototype node. Set lastFunction.
        void visit(PrototypeAST& node) override;
        void visit(FunctionAST& node) override;

    private:
        /// Log an error durring code creation
        llvm::Value* logErrorV(const char* str){
            fprintf(stderr, "LogError: %s\n", str);
            return nullptr;
        }

        llvm::Value* lastValue; // Contain the last value if defined
        llvm::Function* lastFunction; // Contain the last function if defined
        llvm::LLVMContext context;
        llvm::IRBuilder<> builder;
        std::unique_ptr<llvm::Module> module;
        std::map<llvm::StringRef, llvm::Value *> namedValues;

    };

    /// Visitor for producing prety print of ast
    class PPrintorVisitor: public Visitor{

    public:
        PPrintorVisitor(): inc(0){};

        /// Pretty print NumberExpr:
        /// NumberExpr($val)
        void visit(NumberExprAST& node) override;
        /// Pretty print VariableExpr
        /// VariableExpr($varname)
        void visit(VariableExprAST& node) override;
        /// Pretty print BinaryExpr
        /// BinaryExpr(
        ///     $leftOp
        ///     $op
        ///     $rightOp
        /// )
        void visit(BinaryExprAST& node) override;
        /// Pretty print CallExpr
        /// CallExpr($callee(
        ///     $args*
        /// )
        void visit(CallExprAST& node) override;
        /// Pretty print Prototype
        /// Prototype($name(
        ///     $args*
        /// )
        void visit(PrototypeAST& node) override;
        /// Pretty print Function
        /// Function(
        ///     $prototype
        ///     $expr
        /// )
        void visit(FunctionAST& node) override;

        [[nodiscard]] const std::string &getStr() const;

    private:
        /// Get the prefix for a logged line
        [[nodiscard]] std::string getLinePrefix() const;
        int inc;
        std::string str;
    };

}

#endif //LLVM_KALEIDOSCOPE_VISITOR_H