#ifndef TOKEN_HEADER
#define TOKEN_HEADER

#include <string>

// tipos de token possível
enum class TokenType {
    Number,
    Char,
    String,
    Bool,
    Operator,
    Type,
    Hashtag,
    ParenOpen,
    ParenClose,
    BracketOpen,
    BracketClose,
    KeyOpen,
    KeyClose,
    Keyword,
    Identifier,
    Assign,
    Colon,
    Semicolon,
    Discard,
    EndOfFile,
    Unknown
};

// transforma em string o tipo de token
std::string tokenTypeToString(TokenType type);

// define a estrutura dos tokens
struct Token {
    TokenType type;
    std::string value;

    void print() const;
};

#endif //TOKEN_HEADER