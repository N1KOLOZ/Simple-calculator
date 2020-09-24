#pragma once

#include <map>

class Variable {
public:
    std::string name;
    double value;
};

std::map<std::string, double> vars;

bool Is_declared(const std::string& name) {
    return vars.count(name) == 1;
}

double Define_var(std::string name, double value) {
    if (vars.count(name) == 1) {
        throw std::logic_error("Variable is already defined");
    }

    vars[std::move(name)] = value;

    return value;
}

double Get_value(const std::string& name) {
    if (vars.count(name) == 0) {
        throw std::logic_error("Undefined variable");
    }

    return vars.at(name);
}

void Set_value(std::string name, double value) {
    if (vars.count(name) == 0) {
        throw std::logic_error("Undefined variable");
    }

    vars[std::move(name)] = value;
}
