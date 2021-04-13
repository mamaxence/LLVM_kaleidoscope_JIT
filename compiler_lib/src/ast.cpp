//
// Created by maxence on 27/03/2021.
//

#include "ast.h"
namespace ckalei{


    void NumberExprAST::accept(Visitor &visitor)
    {visitor.visit(*this);}

    void VariableExprAST::accept(Visitor &visitor)
    {
        visitor.visit(*this);
    }

    void BinaryExprAST::accept(Visitor &visitor)
    {
        visitor.visit(*this);
    }

    void CallExprAST::accept(Visitor &visitor)
    {
        visitor.visit(*this);
    }

    void PrototypeAST::accept(Visitor &visitor)
    {
        visitor.visit(*this);
    }

    void FunctionAST::accept(Visitor &visitor)
    {
        visitor.visit(*this);
    }

    void IfExprAST::accept(Visitor &visitor)
    {
        visitor.visit(*this);
    }

    void ForExprAST::accept(Visitor &visitor)
    {
        visitor.visit(*this);
    }
}
