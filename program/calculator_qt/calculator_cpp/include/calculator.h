#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <memory>
#include "parser.h"

class Calculator {
public:
    double evaluate(std::shared_ptr<ASTNode> node);
    
private:
    double applyFunction(const std::string& funcName, const std::vector<double>& args);
    double applyOperator(char op, double left, double right);
    double applyUnaryOperator(char op, double operand);
};

#endif // CALCULATOR_H