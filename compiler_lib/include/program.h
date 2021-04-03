//
// Created by maxence on 03/04/2021.
//

#ifndef LLVM_KALEIDOSCOPE_PROGRAM_H
#define LLVM_KALEIDOSCOPE_PROGRAM_H

#include <string>
#include <utility>
#include <memory>

#include "parser.h"

namespace ckalei{

    class Program{
    public:
        Program(const std::string& rawCode): rawCode(rawCode){
            auto lexer = std::make_unique<Lexer>(rawCode);
            parser = std::make_unique<Parser>(std::move(lexer));
            ast_data = parser->getAstNodes();
        };

        /// Return a pprinted representation of the program
        [[nodiscard]] std::string ppformat() const{
            auto pprinter = ckalei::PPrintorVisitor();
            for (auto const& node: ast_data){
                if (node != nullptr){
                    node->accept(pprinter);
                }
            }
            return pprinter.getStr();
        }


    private:
        std::string rawCode;
        std::unique_ptr<Parser> parser;
        std::vector<std::unique_ptr<ASTNode>> ast_data;
    };
} // ckalei

#endif //LLVM_KALEIDOSCOPE_PROGRAM_H
