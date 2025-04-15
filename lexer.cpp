#include "lexer.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <unordered_set>

static std::unordered_set<std::string> keywords = {
    "if", "else", "self", "match", "type"
};

static std::unordered_set<std::string> types = {
    "int", "nat", "rac",
    "char", "string",
    "bool"
};

Lexer::Lexer(const std::string& text) 
    : input(text), pos(0), current(text.empty() ? '\0' : text[0]) {}

void Lexer::advance(int positions) {
    pos += positions;
    current = (pos < input.size()) ? input[pos] : '\0';
}

void Lexer::skipWhitespace() {
    while (isspace(current)) advance();
}

Token Lexer::number() {
    bool isDecimal = false;
    std::string value;

    // enquanto ler números
    while (isdigit(current) || current == '.')
    {
        if (current == '.') {
            // termina de ler se encontrar um . a mais
            if (isDecimal) break;
            isDecimal = true;
        }
        value += current;
        advance();
    }

    return {TokenType::Number, value};
}

Token Lexer::stringLiteral() {
    std::string value;
    advance(); // pula as aspas iniciais
    while (current != '"' && current != '\0') { // até achar o fim das aspas
        value += current;
        advance();
    }
    advance(); // pula o fechamento de aspas
    return {TokenType::String, value};
}

Token Lexer::character() {
    std::string value;
    advance(); // pula a aspa simples
    if (current == '\\') { // confere se é um caracter especial
        advance();
        switch (current)
        {
        case 'n': value = "\n"; break;
        case 't': value = "\t"; break;
        case '\\': value = "\\"; break;
        case '\'': value = "\'"; break;
        default:
            auto letra = std::string(1, current);
            advance();
            return {TokenType::Unknown, "'\\"+letra};
        }
    }
    else {
        value += current;
    }

    advance(); // pula o valor do caractere
    if (current != '\'') { // se aa declaração do caraccter não for fechada
        return {TokenType::Unknown, "'"+value};
    }
    advance(); // pula a aspa simples
    return {TokenType::Char, value};
}

Token Lexer::keywordTypeOrIdentifier() {
    std::string value;

    // lê caracteres alphanumericos e _
    while (isalnum(current) || current == '_')
    {
        value += current;
        advance();
    }

    // procura o valor entre keywords
    if (keywords.count(value)) {
        return {TokenType::Keyword, value};
    }

    // procura o valor entre nomes de tipos
    if (types.count(value)) {
        return {TokenType::Type, value};
    }
    
    // o valor é um identificador 
    return {TokenType::Identifier, value};
}

Token Lexer::multiCharOperator(char nextExpected, TokenType singleType, 
    std::string matchedValue, std::string singleValue
) {
    advance(); // pula o caracter atual

    // se o próximo caracter completava o operador
    if (current == nextExpected) {
        advance(); // pula esse caracter
        // forma o operador duplo
        return {TokenType::Operator, matchedValue};
    }
    // retorna o token necessário caso o caracter não complete o operador
    return {singleType, singleValue};
}

Token Lexer::getNextToken() {
    skipWhitespace();
    
    // identifica o fim do arquivo
    if (current == '\0') return {
        TokenType::EndOfFile, ""
    };
    // lê tokens de tamanho variado
    if (isdigit(current)) return number();
    if (isalpha(current)) return keywordTypeOrIdentifier();
    if (current == '"') return stringLiteral();
    if (current == '\'') return character();

    char value = current;
    switch (current)
    {
        // lê operadores simples
        case '+':
        case '-':
        case '*':
        case '/': 
            advance();
            return {
                TokenType::Operator,
                std::string(1, value)
            };
        // lê possíveis operadores duplos
        case '=': return multiCharOperator('=', TokenType::Assign, "==", "=");
        case '!': return multiCharOperator('=', TokenType::Unknown, "!=", "!");
        case '<': return multiCharOperator('=', TokenType::Operator, "<=", "<");
        case '>': return multiCharOperator('=', TokenType::Operator, ">=", ">");
        case '|': return multiCharOperator('|', TokenType::Operator, "||", "|");
        case '&': return multiCharOperator('&', TokenType::Operator, "&&", "&");
        // lê tokens especiais
        case '#': advance(); return {TokenType::Hashtag, "#"};
        case '(': advance(); return {TokenType::ParenOpen, "("};
        case ')': advance(); return {TokenType::ParenClose, ")"};
        case '[': advance(); return {TokenType::BracketOpen, "["};
        case ']': advance(); return {TokenType::BracketClose, "]"};
        case '{': advance(); return {TokenType::KeyOpen, "{"};
        case '}': advance(); return {TokenType::KeyClose, "}"};
        case ':': advance(); return {TokenType::Colon, ":"};
        case ';': advance(); return {TokenType::Semicolon, ";"};
        case '_': advance(); return {TokenType::Discard, "_"};
        // lê um token inesperado
        default: 
        advance(); 
        return {
            TokenType::Unknown,
            std::string(1, value)
        };
    }
}

std::string readFile(const std::string& filename) {
    std::ifstream file(filename);

    // não pôde abrir o arquivo
    if (!file) {
        std::cerr << "Error: Could not open file '" << filename << "'\n";
        exit(1);
    }

    // lê o conteúdo do arquivo em uma string
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}