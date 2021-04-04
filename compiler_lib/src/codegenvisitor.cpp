//
// Created by maxence on 28/03/2021.
//

#include "visitor.h"

namespace ckalei{

    void CodeGenVisitor::visit(NumberExprAST &node)
    {
        lastValue = llvm::ConstantFP::get(*context, llvm::APFloat(node.getVal()));
    }

    void CodeGenVisitor::visit(VariableExprAST &node)
    {
        llvm::Value *v = namedValues[node.getName()];
        if (!v){
            lastValue = logErrorV("Unknown variable name");
        }
        lastValue = v;
    }

    void CodeGenVisitor::visit(BinaryExprAST &node)
    {
        // Get left and right values
        node.getLeftExpr()->accept(*this);
        auto lv = lastValue;
        node.getRightExpr()->accept(*this);
        auto rv = lastValue;
        if (!lv || !rv){
            lastValue = nullptr;
            return;
        }

        switch (node.getOp()) {
            case '+':
                lastValue = builder->CreateFAdd(lv, rv, "addtmp");
                return;
            case '-':
                lastValue = builder->CreateFSub(lv, rv, "subtmp");
                return;
            case '*':
                lastValue = builder->CreateFMul(lv, rv, "multmp");
                return;
            case '<':
                lv = builder->CreateFCmpULT(lv, rv, "cmptmp");
                // convert bool 0/1 to double 0.0 or 1.0
                lastValue = builder->CreateUIToFP(lv, llvm::Type::getDoubleTy(*context), "booltmp");
                return;
            default:
                lastValue = logErrorV("invalid binary operator");
        }
    }

    void CodeGenVisitor::visit(CallExprAST &node)
    {
        llvm::Function *calleeF = module->getFunction(node.getCallee());
        if (!calleeF){
            lastValue = logErrorV("Function not found");
            return;
        }

        if (calleeF->arg_size() != node.getArgs().size()){
            lastValue = logErrorV("Invalid number of arguments");
            return;
        }

        std::vector<llvm::Value *> argsVals;
        for (auto const& args: node.getArgs()){
            args->accept(*this);
            if (!lastValue){ // return in case of failure
                return;
            }
            argsVals.push_back(lastValue);
        }

        lastValue = builder->CreateCall(calleeF, argsVals, "calltmp");
    }

    void CodeGenVisitor::visit(PrototypeAST &node)
    {
        std::vector<llvm::Type *> argsTypes(node.getArgs().size(), llvm::Type::getDoubleTy(*context));
        auto signature = llvm::FunctionType::get(llvm::Type::getDoubleTy(*context), argsTypes, false);

        auto func = llvm::Function::Create(signature,
                                              llvm::Function::ExternalLinkage,
                                              node.getName(),
                                              module.get());

        // Set function args names
        for (int i=0; i<node.getArgs().size(); i++){
            func->getArg(i)->setName(node.getArgs()[i]);
        }
        lastFunction = func;
    }

    void CodeGenVisitor::visit(FunctionAST &node)
    {
        // Check if function name is already declared
        llvm::Function* function = module->getFunction(node.getProto()->getName());
        if (!function){ // if it is not set, get the proto by parsing the declared prototype
            node.getProto()->accept(*this);
            function = lastFunction;
        }

        if (!function){ // if no prototype, an error occured
            lastFunction = nullptr;
            return;
        }

        if (!function->empty()){
            lastFunction = (llvm::Function*) logErrorV("Function can not be redefined");
            return;
        }

        // Create the block for the function
        llvm::BasicBlock *bb = llvm::BasicBlock::Create(*context, "entry", function);
        builder->SetInsertPoint(bb);

        // Create a nue named value table containing functions args
        namedValues.clear();
        for (auto &arg: function->args()){
            namedValues[arg.getName()] = &arg;
        }

        node.getBody()->accept(*this);
        auto retVal =  lastValue;
        if (retVal){
            builder->CreateRet(retVal);
            llvm::verifyFunction(*function);
            lastFunction = function;
            return;
        }
        function->eraseFromParent();
        lastFunction = nullptr;
    }
}
