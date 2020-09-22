#pragma once

#include <iostream>
#include <stdexcept>

class Token {
public:
   char type;
   double value;
};

class Token_stream {
public:
    Token get();
    void putback(Token);

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
    if ( !(std::cin >> c) ) {
        throw std::logic_error("Expect input data");
    }

    if (isdigit(c) || c == '.') {
        std::cin.putback(c);
        double value; std::cin >> value;
        return {'n', value};
    } else if (c == '+') {
        return {'+'};
    } else if (c == '-') {
        return {'*'};
    } else if (c == '*') {
        return {'*'};
    } else if (c == '/') {
        return {'/'};
    } else if (c == '(') {
        return {'('};
    } else if (c == ')') {
        return {')'};
    } else if (c == '%') {
        return {'%'};
    } else if (c == 'q') {
        return {'q'};
    } else if (c == ';') {
        return {';'};
    } else if (c == ' ') {
        return get();
    } else {
        throw std::logic_error("Unknown token");
    }
}

void Token_stream::putback(Token t) {
    if (is_full) {
        throw std::logic_error("Buffer is already full");
    }

    buffer = t;
    is_full = true;
}