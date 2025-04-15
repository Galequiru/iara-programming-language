#include "token.hpp"
#include <iostream>

#define case_to_string(x, s) case TokenType::x: return s;

std::string tokenTypeToString(TokenType type) {
    switch (type)
    {
    case_to_string(Number, "number")
    case_to_string(Char, "character")
    case_to_string(String, "string")
    case_to_string(Bool, "boolean")
    case_to_string(Operator, "operator")
    case_to_string(Type, "type")
    case_to_string(Hashtag, "hashtag")
    case_to_string(ParenOpen, "open parentheses")
    case_to_string(ParenClose,"close parentheses")
    case_to_string(BracketOpen, "open bracket")
    case_to_string(BracketClose, "close bracket")
    case_to_string(KeyOpen, "open key")
    case_to_string(KeyClose, "open key")
    case_to_string(Keyword, "keyword")
    case_to_string(Identifier, "identifier")
    case_to_string(Assign, "assign")
    case_to_string(Colon, "colon")
    case_to_string(Semicolon, "semicolon")
    case_to_string(Discard, "discard")
    case_to_string(EndOfFile, "end of file")
    default: return "Unknown";
    }
}

void Token::print() const {
    std::cout << "Token {\n\t" << tokenTypeToString(type) << "\n\t" << value << "\n}\n";
}