#include "token.h"

#include <iostream>
#include <vector>
#include <stdexcept>

#include <cmath>

Token_stream ts;

Token Next_token();

double Expression();

double Term();

double Primary();

double Expression() {
    double lhs = Term();

    Token t = ts.get();
    while (true) {
        if (t.type == '+') {
            lhs += Term();
            t = ts.get();
        } else if (t.type == '-') {
            lhs -= Term();
            t = ts.get();
        } else {
            ts.putback(t);
            return lhs;
        }
    }
}

double Term() {
    double lhs = Primary();

    Token t = ts.get();
    while (true) {
        if (t.type == '*') {
            lhs *= Primary();
            t = ts.get();
        } else if (t.type == '/') {
            double rhs = Primary();
            if (rhs == 0) {
                throw std::runtime_error("Division by zero");
            }
            lhs /= rhs;
            t = ts.get();
        } else if (t.type == '%') {
            double rhs = Primary();
            if (rhs == 0) {
                throw std::runtime_error("Division by zero");
            }
            lhs = std::fmod(lhs, rhs);
            t = ts.get();
        } else {
            ts.putback(t);
            return lhs;
        }
    }
}

double Primary() {
    Token t = ts.get();

    if (t.type == '(') {
        double expr = Expression();

        t = ts.get();
        if (t.type != ')') {
            throw std::logic_error("Expected right parenthesis");
        }

        return expr;
    } else if (t.type == 'n') {
        return t.value;
    } else if (t.type == '-') {
        return -Primary();
    } else if (t.type == '+') {
        return Primary();
    } else {
        throw std::logic_error("Expected primary");
    }
}


int main() {
    double expr = 0;
    while (std::cin) {
        try {
            std::cout << "> ";
            Token t = ts.get();

            while (t.type == ';') { t = ts.get(); }

            if (t.type == 'q') { return 0; }

            ts.putback(t);
            expr = Expression();
            std::cout << "= " << expr << '\n';

        } catch (const std::exception &e) {
            std::cerr << e.what() << '\n';
            std::cin.clear();
        } catch (...) {
            std::cerr << "Exception\n";
            std::cin.clear();
        }
    }

    return 0;
}
