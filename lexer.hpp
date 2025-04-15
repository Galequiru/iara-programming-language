#ifndef LEXER_HEADER
#define LEXER_HEADER

#include "token.hpp"
#include <string>

class Lexer {
private:
    std::string input;
    size_t pos;
    char current;

    // avança caracteres, default 1
    void advance(int positions = 1);
    
    void skipWhitespace();
    
    // lê um número do código fonte
    Token number();
    
    // lê uma string do código fonte
    Token stringLiteral();
    
    // lê um caracter do código fonte
    Token character();
    
    // lê um possível keyword/tipo/identificador do codigo fonte
    Token keywordTypeOrIdentifier();
    
    // confere se o próximo caracter faz parte do mesmo operador
    Token multiCharOperator(char nextExpected, TokenType singleType, 
                            std::string matchedValue, std::string singleValue);
public:
    Lexer(const std::string& text);
    // lê caracteres no código fonte até formar um token
    Token getNextToken();
};

// tenta ler o conteúdo do arquivo passando ao lexer
std::string readFile(const std::string& filename);

#endif //LEXER_HEADER