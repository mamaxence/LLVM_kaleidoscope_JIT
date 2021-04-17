//
// implementation for pretty print visitor
// Created by maxence on 27/03/2021.
//


#include <fmt/core.h>
#include "visitor.h"


namespace ckalei{

    void PPrintorVisitor::visit(NumberExprAST &node)
    {
        str += fmt::format(getLinePrefix() + "NumberExpr({})\n", node.getVal());
    }


    void PPrintorVisitor::visit(VariableExprAST &node)
    {
        str += fmt::format(getLinePrefix() + "VariableExpr({})\n", node.getName());
    }

    void PPrintorVisitor::visit(UnaryExprAST &node)
    {
        str += getLinePrefix() + "UnaryExpr(\n";
        inc++;
        str += fmt::format(getLinePrefix() + "{}\n", node.getOpcode());
        node.getExpr()->accept(*this);
        inc--;
        str += getLinePrefix() + ")\n";
    }

    void PPrintorVisitor::visit(BinaryExprAST &node)
    {
        str += getLinePrefix() + "BinaryExpr(\n";
        inc++;
        node.getLeftExpr()->accept(*this);
        str += fmt::format(getLinePrefix() + "{}\n", node.getOp());
        node.getRightExpr()->accept(*this);
        inc--;
        str += getLinePrefix() + ")\n";
    }

    void PPrintorVisitor::visit(DeclarationExprAST &node)
    {
        str += getLinePrefix() + "DeclarationExpr(\n";
        inc++;
        for (const auto &pair : node.getVars()){
            str += getLinePrefix() + pair.first + "\n"; // name
            if (pair.second){
                inc++;
                str += getLinePrefix() + "=\n";
                pair.second->accept(*this);
                inc--;
            }
        }
        node.getBody()->accept(*this);
        inc--;
        str += getLinePrefix() + ")\n";
    }

    void PPrintorVisitor::visit(CallExprAST &node)
    {
        str += fmt::format(getLinePrefix() + "CallExpr({}(\n",node.getCallee());

        inc++;
        auto const &args = node.getArgs();
        for (auto const& expr: args){
            expr->accept(*this);
        }
        inc--;
        str += getLinePrefix() + ")\n";
    }

    void PPrintorVisitor::visit(PrototypeAST &node)
    {
        if (node.isOperatorProto()){
            str += fmt::format(getLinePrefix() + "Prototype({} {}(\n",node.getName(),node.getPrecedence());
        }else{
            str += fmt::format(getLinePrefix() + "Prototype({}(\n",node.getName());
        }

        inc++;
        auto const &args = node.getArgs();
        for (auto const& argName: args){
            str += getLinePrefix() +  argName + "\n";
        }
        inc--;
        str += getLinePrefix() + ")\n";
    }

    void PPrintorVisitor::visit(IfExprAST &node)
    {
        str += getLinePrefix() + "IfExpr(\n";
        inc++;
        node.getCond()->accept(*this);
        node.getIfExpr()->accept(*this);
        if (node.haveElseMember()){
            node.getElseExpr()->accept(*this);
        }
        inc--;
        str += getLinePrefix() + ")\n";
    }

    void PPrintorVisitor::visit(ForExprAST &node)
    {
        str += getLinePrefix() + "ForExpr(\n";
        inc++;
        str += getLinePrefix() + node.getVarName() + "\n";
        node.getStart()->accept(*this);
        node.getStep()->accept(*this);
        node.getEnd()->accept(*this);
        node.getBody()->accept(*this);
        inc--;
        str += getLinePrefix() + ")\n";
    }

    void PPrintorVisitor::visit(FunctionAST &node)
    {
        str += getLinePrefix() + "Function(\n";
        inc++;
        node.getProto()->accept(*this);
        node.getBody()->accept(*this);
        inc--;
        str += getLinePrefix() + ")\n";
    }

    std::string PPrintorVisitor::getLinePrefix() const
    {
        auto res = std::string();
        for (int i=0; i<inc; i++){
            res += "    ";
        }
        return res;
    }

    const std::string &PPrintorVisitor::getStr() const
    {
        return str;
    }
}

