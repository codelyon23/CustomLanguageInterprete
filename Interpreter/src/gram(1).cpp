

#include "gram.hpp"
#include <iostream>

// Constructeur
Gram::Gram(TokenList tokens) : tokens(std::move(tokens)), current(0) { }

// Fonction principale pour parser l'entrée
std::unique_ptr<ProgramStatement> Gram::parse() {
    auto program = std::make_unique<ProgramStatement>();

    while (!isAtEnd()) {
        auto statement = parseStatement();
        if (statement) {
            program->addChild(statement.release());
        }
    }

    return program;
}

// Analyse un énoncé
std::unique_ptr<Statement> Gram::parseStatement() {
    if (match(TokenType::IDENTIFIER)) {
        if (match(TokenType::EQUALS)) {
            auto identifier = parseIdentifier();
            auto expression = parseExpression();
            consume(TokenType::SEMICOLON);
            return std::make_unique<AssignmentStatement>(std::move(*identifier), std::move(expression));
        }
        else {
            current--; // Reculer si ce n'était pas une assignation
            return parseFunctionCall();
        }
    } 
    else if (match(TokenType::IF)) {
        return parseIfStatement();
    } 
    else if (match(TokenType::WHILE)) {
        return parseWhileStatement();
    } 
    else if (match(TokenType::FUNCTION)) {
        return parseFunctionDeclaration();
    }

    return nullptr; // Aucune déclaration reconnue
}

// Analyse une expression
std::unique_ptr<Expression> Gram::parseExpression() {
    return parseBinaryExpression(nullptr);
}

// Analyse un identifiant
std::unique_ptr<Identifier> Gram::parseIdentifier() {
    auto token = peek();
    consume(TokenType::IDENTIFIER);
    return std::make_unique<Identifier>(token.value);
}

// Analyse un littéral numérique
std::unique_ptr<NumberLiteral> Gram::parseNumber() {
    auto token = peek();
    consume(TokenType::NUMBER_LITERAL);
    return std::make_unique<NumberLiteral>(std::stoi(token.value));
}

// Analyse un littéral de chaîne
std::unique_ptr<StringLiteral> Gram::parseString() {
    auto token = peek();
    consume(TokenType::STRING_LITERAL);
    return std::make_unique<StringLiteral>(token.value);
}

// Analyse une déclaration de fonction
std::unique_ptr<FunctionDeclaration> Gram::parseFunctionDeclaration() {
    consume(TokenType::FUNCTION);
    auto name = parseIdentifier();
    consume(TokenType::OPEN_BRACKET);
    
    std::vector<Identifier> parameters;
    while (!match(TokenType::CLOSE_BRACKET)) {
        parameters.push_back(*parseIdentifier());
        match(TokenType::COMMA); // Ignore la virgule
    }
    
    consume(TokenType::OPEN_CURLY); // Ouvrir le corps de la fonction
    auto body = std::make_unique<ProgramStatement>();
    while (!match(TokenType::CLOSE_CURLY)) {
        auto statement = parseStatement();
        if (statement) {
            body->addChild(statement.release());
        }
    }

    return std::make_unique<FunctionDeclaration>(*name, parameters, std::move(body));
}

// Analyse une condition
std::unique_ptr<IfStatement> Gram::parseIfStatement() {
    consume(TokenType::IF);
    consume(TokenType::OPEN_BRACKET);
    auto condition = parseExpression();
    consume(TokenType::CLOSE_BRACKET);
    
    consume(TokenType::OPEN_CURLY); // Ouvrir le corps de la condition
    auto body = std::make_unique<ProgramStatement>();
    while (!match(TokenType::CLOSE_CURLY)) {
        auto statement = parseStatement();
        if (statement) {
            body->addChild(statement.release());
        }
    }

    return std::make_unique<IfStatement>(std::move(condition), std::move(body));
}

// Analyse une boucle
std::unique_ptr<WhileStatement> Gram::parseWhileStatement() {
    consume(TokenType::WHILE);
    consume(TokenType::OPEN_BRACKET);
    auto condition = parseExpression();
    consume(TokenType::CLOSE_BRACKET);
    
    consume(TokenType::OPEN_CURLY); // Ouvrir le corps de la boucle
    auto body = std::make_unique<ProgramStatement>();
    while (!match(TokenType::CLOSE_CURLY)) {
        auto statement = parseStatement();
        if (statement) {
            body->addChild(statement.release());
        }
    }

    return std::make_unique<WhileStatement>(std::move(condition), std::move(body));
}

// Analyse un appel de fonction
std::unique_ptr<FunctionCall> Gram::parseFunctionCall() {
    auto functionName = parseIdentifier();
    consume(TokenType::OPEN_BRACKET);
    
    std::vector<std::unique_ptr<Expression>> arguments;
    while (!match(TokenType::CLOSE_BRACKET)) {
        arguments.push_back(parseExpression());
        match(TokenType::COMMA); // Ignore la virgule
    }
    
    return std::make_unique<FunctionCall>(*functionName, std::move(arguments));
}

// Analyse une expression binaire
std::unique_ptr<Expression> Gram::parseBinaryExpression(std::unique_ptr<Expression> left) {
    // Implémenter l'analyse des opérations binaires
    return left; // Placeholder
}

// Méthodes utilitaires
void Gram::consume(TokenType type) {
    if (!match(type)) {
        std::cerr << "Expected token type: " << static_cast<int>(type) << std::endl;
        // Gérer l'erreur
    }
}

bool Gram::match(TokenType type) {
    if (isAtEnd()) return false;
    if (tokens[current].type != type) return false;
    current++;
    return true;
}

bool Gram::isAtEnd() {
    return current >= tokens.size();
}

Token Gram::peek() {
    return tokens[current];
}
