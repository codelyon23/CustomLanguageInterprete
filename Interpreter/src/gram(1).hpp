

#pragma once
#include "lexer.hpp"
#include "ast.hpp"
#include <memory>
#include <vector>

class Gram {
public:
    Gram(TokenList tokens);
    std::unique_ptr<ProgramStatement> parse();

private:
    TokenList tokens;
    size_t current;

    std::unique_ptr<Statement> parseStatement();
    std::unique_ptr<Expression> parseExpression();
    std::unique_ptr<Identifier> parseIdentifier();
    std::unique_ptr<NumberLiteral> parseNumber();
    std::unique_ptr<StringLiteral> parseString();
    std::unique_ptr<IfStatement> parseIfStatement();
    std::unique_ptr<WhileStatement> parseWhileStatement();
    std::unique_ptr<FunctionDeclaration> parseFunctionDeclaration();
    std::unique_ptr<FunctionCall> parseFunctionCall();
    std::unique_ptr<Expression> parseBinaryExpression(std::unique_ptr<Expression> left);
    
    void consume(TokenType type);
    bool match(TokenType type);
    bool isAtEnd();
    Token peek();
};
