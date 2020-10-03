#include "token.h"
#include "variable.h"

#include <iostream>
#include <vector>
#include <stdexcept>

#include <cmath>

Var_table vars;
Token_stream ts;
Token Next_token();
void Clean_up();

void Set_args(std::vector<double>&);
double Expression();
double Term();
double Primary();

double Declaration();

double Statement();
void Calculate();

void SetArgs(std::vector<double>& args) {
    Token t = ts.get();

    if (t.type != Type::LEFT_PAREN) {
        throw std::logic_error("Expected left parenthesis after function");
    }

    int i = 0;
    do {
        args[i++] = Expression();
        t = ts.get();
    } while (t.type == Type::COMMA);


    if (t.type != Type::RIGHT_PAREN) {
        throw std::logic_error("Expected right parenthesis after function");
    }
}

double Expression() {
    double lhs = Term();

    Token t = ts.get();
    while (true) {
        if (t.type == Type::OP_ADD) {
            lhs += Term();
            t = ts.get();
        } else if (t.type == Type::OP_SUBTRACT) {
            lhs -= Term();
            t = ts.get();
        } else {
            ts.putback(std::move(t));
            return lhs;
        }
    }
}

double Term() {
    double lhs = Primary();

    Token t = ts.get();
    while (true) {
        if (t.type == Type::OP_MULTIPLY) {
            lhs *= Primary();
            t = ts.get();
        } else if (t.type == Type::OP_DIVIDE) {
            double rhs = Primary();
            if (rhs == 0) {
                throw std::runtime_error("Division by zero");
            }

            lhs /= rhs;
            t = ts.get();
        } else if (t.type == Type::OP_MOD) {
            double rhs = Primary();
            if (rhs == 0) {
                throw std::runtime_error("Division by zero");
            }

            lhs = std::fmod(lhs, rhs);
            t = ts.get();
        } else if (t.type == Type::OP_EXPONENTIATE) {
            double rhs = Primary();
            lhs = std::pow(lhs, rhs);
            t = ts.get();
        } else {
            ts.putback(std::move(t));
            return lhs;
        }
    }
}

double Primary() {
    Token t = ts.get();

    if (t.type == Type::LEFT_PAREN) {
        double expr = Expression();

        t = ts.get();
        if (t.type != Type::RIGHT_PAREN) {
            throw std::logic_error("Expected right parenthesis");
        }

        return expr;
    } else if (t.type == Type::FUNC_SQRT) {
        std::vector<double> args(1); SetArgs(args);
        return std::sqrt(args[0]);
    } else if (t.type == Type::FUNC_MIN) {
        std::vector<double> args(2); SetArgs(args);
        return std::min(args[0], args[1]);
    } else if (t.type == Type::FUNC_MAX) {
        std::vector<double> args(2); SetArgs(args);
        return std::max(args[0], args[1]);
    } else if (t.type == Type::NUMBER) {
        return t.value;
    } else if (t.type == Type::OP_SUBTRACT) {
        return -Primary();
    } else if (t.type == Type::OP_ADD) {
        return Primary();
    } else if (t.type == Type::VAR_NAME) {
        Token t2 = ts.get();
        if (t2.type == Type::ASSIGN) {
            double value = Expression();
            return vars.Set_value(std::move(t.name), value);
        } else {
            ts.putback(t2);
            return vars.Get_value(t.name);
        }
    } else {
        throw std::logic_error("Expected primary");
    }
}

void Clean_up() {
    ts.ignore();
}

double Declaration() {
    Token t = ts.get();
    if (t.type != Type::VAR_NAME) {
        throw std::logic_error("Variable name expected");
    }

    Token t2 = ts.get();
    if (t2.type != Type::ASSIGN) {
        throw std::logic_error("Symbol '=' is missed in definition " + t.name);
    }

    double value = Expression();
    return vars.Define_name(std::move(t.name), value);
}

double Statement() {
    Token t = ts.get();

    if (t.type == Type::VAR_DEF) {
        return Declaration();
    } else {
        ts.putback(std::move(t));
        return Expression();
    }
}

void Calculate() {
    while (true) {
        try {
            std::cout << "> ";
            Token t = ts.get();

            while (t.type == Type::END_EXPR) { t = ts.get(); }

            if (t.type == Type::QUIT) { return; }

            ts.putback(std::move(t));
            std::cout << "= " << Statement() << std::endl;
        } catch (const std::exception& e) {
            std::cout << e.what() << std::endl;
            Clean_up();
        }
    }
}

int main() {
    try {
        vars.Define_name("pi", 3.1415926535);
        vars.Define_name("e", 2.7182818284);

        Calculate();
    } catch (const std::exception &e) {
        std::cout << e.what() << std::endl;
    } catch (...) {
        std::cout << "Exception" << std::endl;
    }

    return 0;
}