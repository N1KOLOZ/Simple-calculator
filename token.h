#pragma once

#include <iostream>
#include <stdexcept>
#include <string>

enum Type {
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
    END_EXPR,
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
    void ignore(char); // ignore tokens before symbol c including
private:
    bool is_full{false};
    Token buffer;
};

Token Token_stream::get() {
    if (is_full) {
        is_full = false;
        return buffer;
    }

    if (!std::cin) {
        throw std::logic_error("Expect input data");
    }

    char c = 0;
    std::cin >> c;
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

    } else if (c == '(') {
        return Token(Type::LEFT_PAREN);
    } else if (c == ')') {
        return Token(Type::RIGHT_PAREN);
    } else if (c == 'q') {
        return Token(Type::QUIT);
    } else if (c == ';') {
        return Token(Type::END_EXPR);
    } else if (c == '\n') {
        return Token(Type::END_EXPR);
    } else if (isalpha(c)) {
        std::string name(1, c);

        while (std::cin.get(c) &&
               (isalpha(c) || isdigit(c) || c == '_')) {
            name += c;
        }
        std::cin.putback(c);

        if (name == "let") {
            return Token(Type::VAR_DEF);
        } else if (name == "sqrt") {
            return Token(Type::FUNC_SQRT);
        } else if (name == "quit") {
            return Token(Type::QUIT);
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

// IS NEEDED TO CHANGE
void Token_stream::ignore(char stop) {
    if (is_full) {
        is_full = false;
        if (buffer.type == Type::END_EXPR) { return; }
    }

    char c;
    while (std::cin >> c) {
        if (c == stop) { return; }

    }
}