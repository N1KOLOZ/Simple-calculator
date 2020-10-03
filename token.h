#pragma once

#include <iostream>
#include <stdexcept>
#include <string>

enum class Type {
    NUMBER,

    VAR_NAME,
    VAR_DEF,
    ASSIGN,

    OP_ADD,
    OP_SUBTRACT,
    OP_DIVIDE,
    OP_MULTIPLY,
    OP_EXPONENTIATE,

    OP_MOD,

    FUNC_SQRT,
    FUNC_MIN,
    FUNC_MAX,

    LEFT_PAREN,
    RIGHT_PAREN,
    COMMA,
    END_EXPR,

    HELP,
    QUIT,

    DEFAULT,

};

class Token {
public:
    Type type;
    double value;
    std::string name;

    Token() :
            type(Type::DEFAULT),
            value(0),
            name("") {
    }

    Token(Type type_, double value_ = 0) :
            type(type_),
            value(value_),
            name("") {
    }

    Token(Type type_, std::string name_) :
            type(type_),
            value(0),
            name(std::move(name_)) {
    }
};

class Token_stream {
public:
    Token get();
    void putback(Token);
    void ignore(); // ignore tokens before symbol c including
private:
    bool is_full{false};
    Token buffer;
};

Token Token_stream::get() {
    if (is_full) {
        is_full = false;
        return buffer;
    }

    char c = 0;
    if (!(std::cin >> c)) { throw std::logic_error("Expect input data"); }
    if (isspace(c)) {
        return get();
    } else if (isdigit(c) || c == '.') {
        std::cin.putback(c);
        double value;
        std::cin >> value;
        return Token(Type::NUMBER, value);
    } else if (c == '=') {
        return Token(Type::ASSIGN);

    } else if (c == '+') {
        return Token(Type::OP_ADD);
    } else if (c == '-') {
        return Token(Type::OP_SUBTRACT);
    } else if (c == '*') {
        return Token(Type::OP_MULTIPLY);
    } else if (c == '/') {
        return Token(Type::OP_DIVIDE);
    } else if (c == '%') {
        return Token(Type::OP_MOD);
    } else if (c == '^') {
            return Token(Type::OP_EXPONENTIATE);

    } else if (c == ',') {
        return Token(Type::COMMA);
    }  else if (c == '(') {
        return Token(Type::LEFT_PAREN);
    } else if (c == ')') {
        return Token(Type::RIGHT_PAREN);
    } else if (c == 'q') {
        return Token(Type::QUIT);
    } else if (c == ';') {
        return Token(Type::END_EXPR);
    } else if (c == '\n') {
        return Token(Type::END_EXPR); // is needed to change
    } else if (isalpha(c)) {
        std::string name(1, c);

        while (std::cin.get(c) &&
               (isalpha(c) || isdigit(c) || c == '_'))
        { name += c; }
        std::cin.putback(c);

        if (std::cin.peek() == '(') {
            if (name == "sqrt") {
                return Token(Type::FUNC_SQRT);
            } else if (name == "min") {
                return Token(Type::FUNC_MIN);
            } else if (name == "max") {
                return Token(Type::FUNC_MAX);
            } else if (name == "quit") {
                return Token(Type::QUIT);
            } else if (name == "help") {
                return Token(Type::HELP);
            }
        }

        if (name == "let") {
            return Token(Type::VAR_DEF);
        } else {
            return Token(Type::VAR_NAME, std::move(name));
        }
    } else {
        throw std::logic_error("Unknown token");
    }
}

void Token_stream::putback(Token t) {
    if (is_full) {
        throw std::logic_error("Buffer is already full");
    }

    buffer = std::move(t);
    is_full = true;
}

void Token_stream::ignore() {
    const char stop = ';';

    if (is_full) {
        is_full = false;
        if (buffer.type == Type::END_EXPR) { return; }
    }

    char c;
    while (std::cin >> c) {
        if (c == stop) { return; }

    }
}