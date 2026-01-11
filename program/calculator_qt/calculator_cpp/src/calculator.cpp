#include "calculator.h"
#include "functions.h"
#include "constants.h"
#include <cmath>
#include <stdexcept>

double Calculator::evaluate(std::shared_ptr<ASTNode> node) {
    if (!node) {
        throw EvaluationError("空节点");
    }
    
    switch (node->type) {
        case NUM_NODE:
            return node->value;
            
        case CONSTANT_NODE: {
            if (!Constants::isConstant(node->name)) {
                throw EvaluationError("未知常量: " + node->name);
            }
            return Constants::getValue(node->name);
        }
            
        case BIN_OP_NODE: {
            double left = evaluate(node->left);
            double right = evaluate(node->right);
            return applyOperator(node->op, left, right);
        }
            
        case UNARY_OP_NODE: {
            double operand = evaluate(node->operand);
            return applyUnaryOperator(node->op, operand);
        }
            
        case FUNC_CALL_NODE: {
            std::vector<double> args;
            for (const auto& arg : node->args) {
                args.push_back(evaluate(arg));
            }
            return applyFunction(node->name, args);
        }
            
        default:
            throw EvaluationError("未知节点类型");
    }
}

double Calculator::applyOperator(char op, double left, double right) {
    switch (op) {
        case '+':
            return left + right;
        case '-':
            return left - right;
        case '*':
            return left * right;
        case '/':
            if (right == 0) {
                throw EvaluationError("除零错误");
            }
            return left / right;
        case '^':
            return std::pow(left, right);
        default:
            throw EvaluationError("未知操作符: " + std::string(1, op));
    }
}

double Calculator::applyUnaryOperator(char op, double operand) {
    switch (op) {
        case '+':
            return operand;
        case '-':
            return -operand;
        default:
            throw EvaluationError("未知一元操作符: " + std::string(1, op));
    }
}

double Calculator::applyFunction(const std::string& funcName, const std::vector<double>& args) {
    if (!Functions::isFunction(funcName)) {
        throw EvaluationError("未知函数: " + funcName);
    }
    return Functions::evaluate(funcName, args);
}