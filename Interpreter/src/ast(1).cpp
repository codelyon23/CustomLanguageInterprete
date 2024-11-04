// ast.hpp

#pragma once
#include <string>
#include <vector>
#include <memory>

// Classe de base pour tous les nœuds de l'AST
class Node {
public:
    virtual ~Node() = default;
};

// Classe de base pour les instructions
class Statement : public Node {
};

// Classe de base pour les expressions
class Expression : public Node {
};

// Représentation d'un programme entier
class ProgramStatement : public Statement {
public:
    std::vector<std::unique_ptr<Statement>> children;

    void addChild(std::unique_ptr<Statement> stmt) {
        children.push_back(std::move(stmt));
    }

    const std::vector<std::unique_ptr<Statement>>& getBody() const {
        return children;
    }
};

// Représentation d'une déclaration de variable
class VariableDeclaration : public Statement {
public:
    std::string type;
    std::string name;
    std::unique_ptr<Expression> initializer;

    VariableDeclaration(const std::string& type, const std::string& name, std::unique_ptr<Expression> init = nullptr)
        : type(type), name(name), initializer(std::move(init)) {}
};

// Représentation d'une instruction d'affectation
class AssignmentStatement : public Statement {
public:
    std::string identifier;
    std::unique_ptr<Expression> expression;

    AssignmentStatement(const std::string& id, std::unique_ptr<Expression> expr)
        : identifier(id), expression(std::move(expr)) {}
};

// Représentation d'une expression binaire
class BinaryExpression : public Expression {
public:
    std::string op;
    std::unique_ptr<Expression> left;
    std::unique_ptr<Expression> right;

    BinaryExpression(const std::string& operation, std::unique_ptr<Expression> left, std::unique_ptr<Expression> right)
        : op(operation), left(std::move(left)), right(std::move(right)) {}
};

// Représentation d'un littéral numérique
class NumberLiteral : public Expression {
public:
    int value;

    NumberLiteral(int val) : value(val) {}
};

// Représentation d'un littéral de chaîne
class StringLiteral : public Expression {
public:
    std::string value;

    StringLiteral(const std::string& val) : value(val) {}
};

// Représentation d'un identifiant (variable)
class Identifier : public Expression {
public:
    std::string name;

    Identifier(const std::string& name) : name(name) {}
};

// Représentation d'une condition (if/else)
class IfStatement : public Statement {
public:
    std::unique_ptr<Expression> condition;
    std::unique_ptr<Statement> thenBranch;
    std::unique_ptr<Statement> elseBranch;

    IfStatement(std::unique_ptr<Expression> cond, std::unique_ptr<Statement> thenBr, std::unique_ptr<Statement> elseBr = nullptr)
        : condition(std::move(cond)), thenBranch(std::move(thenBr)), elseBranch(std::move(elseBr)) {}
};

// Représentation d'une boucle (while)
class WhileStatement : public Statement {
public:
    std::unique_ptr<Expression> condition;
    std::unique_ptr<Statement> body;

    WhileStatement(std::unique_ptr<Expression> cond, std::unique_ptr<Statement> body)
        : condition(std::move(cond)), body(std::move(body)) {}
};

// Représentation d'une boucle for
class ForStatement : public Statement {
public:
    std::unique_ptr<Statement> initializer;
    std::unique_ptr<Expression> condition;
    std::unique_ptr<Expression> increment;
    std::unique_ptr<Statement> body;

    ForStatement(std::unique_ptr<Statement> init, std::unique_ptr<Expression> cond, std::unique_ptr<Expression> incr, std::unique_ptr<Statement> body)
        : initializer(std::move(init)), condition(std::move(cond)), increment(std::move(incr)), body(std::move(body)) {}
};

// Représentation d'une déclaration de fonction
class FunctionDeclaration : public Statement {
public:
    std::string returnType;
    std::string name;
    std::vector<std::unique_ptr<VariableDeclaration>> parameters;
    std::unique_ptr<ProgramStatement> body;

    FunctionDeclaration(const std::string& retType, const std::string& name, 
                        std::vector<std::unique_ptr<VariableDeclaration>> params, std::unique_ptr<ProgramStatement> body)
        : returnType(retType), name(name), parameters(std::move(params)), body(std::move(body)) {}
};

// Représentation d'un appel de fonction
class FunctionCall : public Expression {
public:
    std::string functionName;
    std::vector<std::unique_ptr<Expression>> arguments;

    FunctionCall(const std::string& funcName, std::vector<std::unique_ptr<Expression>> args)
        : functionName(funcName), arguments(std::move(args)) {}
};
