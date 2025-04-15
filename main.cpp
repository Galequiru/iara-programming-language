#include "lexer.hpp"
#include <iostream>

int main(int argc, char **argv) {
    if (argc != 2) {
        std::cout << "Usage: " << argv[0] << " <filename>" << std::endl;
        return 1;
    }

    std::string content = readFile(argv[1]);
    std::cout << content << std::endl;

    Lexer lexer(content);
    Token token;

    do {
        token = lexer.getNextToken();
        token.print();
    } while (token.type != TokenType::EndOfFile);
    
    return 0;
}