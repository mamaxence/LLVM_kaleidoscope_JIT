//
// Created by maxence on 03/04/2021.
//

#ifndef LLVM_KALEIDOSCOPE_PROGRAM_H
#define LLVM_KALEIDOSCOPE_PROGRAM_H

#include <string>
#include <utility>
#include <memory>

#include "parser.h"

#include "llvm/Support/TargetSelect.h"


namespace ckalei{



    class Program{
    public:
        Program(const std::string& rawCode): rawCode(rawCode){

            llvm::InitializeNativeTarget();
            llvm::InitializeNativeTargetAsmPrinter();
            llvm::InitializeNativeTargetAsmParser();

            auto lexer = std::make_unique<Lexer>(rawCode);
            parser = std::make_unique<Parser>(std::move(lexer));
            astData = parser->getAstNodes();

        };

        /// Return a pprinted representation of the program
        [[nodiscard]] std::string ppformat() const
        {
            auto pprinter = ckalei::PPrintorVisitor();
            for (auto const& node: astData){
                if (node != nullptr){
                    node->accept(pprinter);
                }
            }
            return pprinter.getStr();
        }

        /// Return a string containing assembly representation of the ast
        [[nodiscard]] std::string getAssembly(bool debug=false) const
        {
            auto compiler = CodeGenVisitor();
            return compiler.getAssembly(astData, debug);
        }

        /// Return a list of double containing the evaluation of the program
        [[nodiscard]] std::unique_ptr<std::vector<double>> evaluate() const
        {
            auto compiler = CodeGenVisitor();
            return std::move(compiler.evaluate(astData));
        };


    private:
        std::string rawCode;
        std::unique_ptr<Parser> parser;
        std::vector<std::unique_ptr<ASTNode>> astData;

    };
} // ckalei

#endif //LLVM_KALEIDOSCOPE_PROGRAM_H
