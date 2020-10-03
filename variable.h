#pragma once

#include <map>

class Var_table {
private:
    std::map<std::string, double> vars;
public:
    bool Is_declared(const std::string& name);
    double Define_name(std::string name, double value);
    double Get_value(const std::string& name);
    double Set_value(std::string name, double value);
};

bool Var_table::Is_declared(const std::string& name) {
    return vars.count(name) == 1;
}

double Var_table::Define_name(std::string name, double value) {
    if (vars.count(name) == 1) {
        throw std::logic_error("Variable is already defined");
    }

    return vars[std::move(name)] = value;
}

double Var_table::Get_value(const std::string& name) {
    if (vars.count(name) == 0) {
        throw std::logic_error("Undefined variable");
    }

    return vars.at(name);
}

double Var_table::Set_value(std::string name, double value) {
    if (vars.count(name) == 0) {
        throw std::logic_error("Undefined variable");
    }

    return vars[std::move(name)] = value;
}
