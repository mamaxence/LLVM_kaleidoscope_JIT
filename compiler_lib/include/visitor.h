//
// Created by maxence on 23/03/2021.
//

#ifndef LLVM_KALEIDOSCOPE_VISITOR_H
#define LLVM_KALEIDOSCOPE_VISITOR_H

#include <iostream>
#include <map>



#include "llvm/IR/Value.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Verifier.h"
#include "llvm/IR/LegacyPassManager.h"

#include "llvm/Transforms/InstCombine/InstCombine.h"
#include "llvm/Transforms/Scalar.h"
#include "llvm/Transforms/Scalar/GVN.h"

#include "ast.h"
#include "KaleidoscopeJIT.h"

namespace ckalei{

    class ASTNode;
    class NumberExprAST;
    class VariableExprAST;
    class BinaryExprAST;
    class UnaryExprAST;
    class CallExprAST;
    class IfExprAST;
    class ForExprAST;
    class PrototypeAST;
    class FunctionAST;

    class Visitor{
    public:
        virtual void visit(NumberExprAST& node) = 0;
        virtual void visit(VariableExprAST& node) = 0;
        virtual void visit(BinaryExprAST& node) = 0;
        virtual void visit(UnaryExprAST& node) = 0;
        virtual void visit(CallExprAST& node) =  0;
        virtual void visit(IfExprAST& node) = 0;
        virtual void visit(ForExprAST& node) = 0;
        virtual void visit(PrototypeAST& node) = 0;
        virtual void visit(FunctionAST& node) = 0;
    };

    /// Visitor for code generation
    class CodeGenVisitor: public Visitor{

    public:
        CodeGenVisitor();
        /// Generate code for NumberExpr. Set lastValue.
        void visit(NumberExprAST& node) override;
        /// Generate code for VariableExpr. Set lastValue.
        void visit(VariableExprAST& node) override;
        /// Generate code for Binary expression. Set lastValue.
        void visit(BinaryExprAST& node) override;
        /// Generate code for unary expression. Set lastValue.
        void visit(UnaryExprAST& node) override;
        /// Generate code for CallExpr AST. Set lastValue.
        void visit(CallExprAST& node) override;
        /// Generate code for IfThenElse node. Set lastValue
        void visit(IfExprAST& node) override;
        /// Generate code for For Expr, Set lastValue
        void visit(ForExprAST& node) override;
        /// Generate code for Prototype node. Set lastFunction.
        void visit(PrototypeAST& node) override;
        /// Generate code for Function node. Set lastFunction.
        void visit(FunctionAST& node) override;

    public:
        /// Return assembly transcript of the astData
        [[nodiscard]] std::string getAssembly(const std::vector<std::unique_ptr<ASTNode>>& astData);
        /// Return an evaluation of the current node. Valid only if current node is an expression
        std::unique_ptr<std::vector<double>> evaluate(const std::vector<std::unique_ptr<ASTNode>>& astData);

    private:
        /// Return computed assembly code for lastFunc
        [[nodiscard]] std::string ppformat() const;
        /// Top level handling of top level expression
        void handleTopLevelExpression(FunctionAST& node);
        /// Top level handling of function definition
        void handleTopLevelDefinition(FunctionAST& node);
        /// Top level handling of extern declaration
        void handleTopLevelExtern(PrototypeAST& node);
        /// Search for the Function IR for the given name. First search in the current module, then in the declared
        /// functionProto map. It not found, return nullptr.
        llvm::Function *getFunction(const std::string& name);

        /// Log an error durring code creation
        llvm::Value* logErrorV(const char* str){
            fprintf(stderr, "LogError: %s\n", str);
            return nullptr;
        }
        /// Initialise a new module and its associated context and pass manager. To be called after each expression
        /// Creation
        void initModuleAndPassManager();

        std::unique_ptr<llvm::orc::KaleidoscopeJIT> jit;

        llvm::Value* lastValue{}; // Contain the last value if defined
        llvm::Function* lastFunction{}; // Contain the last function if defined
        std::unique_ptr<llvm::LLVMContext> context;
        std::unique_ptr<llvm::IRBuilder<>> builder;
        std::unique_ptr<llvm::Module> module;
        std::map<llvm::StringRef, llvm::Value *> namedValues; // Contain reference to named values in context
        std::map<std::string, std::unique_ptr<PrototypeAST>> functionProtos;

        std::unique_ptr<llvm::legacy::FunctionPassManager> passManager;
        std::unique_ptr<std::vector<double>> evaluationRes;

        bool jitTopLevel;
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
        /// Pretty print UnaryExpr node
        /// UnaryExpr(
        ///     $op
        ///     $expr
        /// )
        void visit(UnaryExprAST& node) override;
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
        /// Pretty print IfExpr
        /// IfExpr(
        ///     $cond
        ///     $ifExpr
        ///     $elseExpr?
        /// )
        void visit(IfExprAST& node) override;
        /// Pretty print ForExpr
        /// ForExprAST(
        ///     $varName
        ///     $startExpr
        ///     $stepExpr
        ///     $endExpr
        ///     $bodyExpr
        ///)
        void visit(ForExprAST& node) override;
        /// Pretty print Prototype
        /// Prototype($operator? $name $precedence?(
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
