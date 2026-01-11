#include "parser.h"
#include "constants.h"
#include "functions.h"
#include <cctype>
#include <stdexcept>
#include <iostream>

Parser::Parser(const std::string& expr) : expression(expr), pos(0), currentToken(END) {
    consumeToken();
}

std::shared_ptr<ASTNode> Parser::parse() {
    auto result = parseExpression();
    if (currentToken.type != END) {
        throw SyntaxError("表达式解析完成后仍有未处理的字符");
    }
    return result;
}

void Parser::skipWhitespace() {
    while (pos < expression.length() && std::isspace(expression[pos])) {
        pos++;
    }
}

Token Parser::getNextToken() {
    skipWhitespace();
    
    if (pos >= expression.length()) {
        return Token(END);
    }
    
    char ch = expression[pos];
    
    // 数字
    if (std::isdigit(ch) || ch == '.') {
        size_t start = pos;
        while (pos < expression.length() && (std::isdigit(expression[pos]) || expression[pos] == '.')) {
            pos++;
        }
        std::string numStr = expression.substr(start, pos - start);
        try {
            double value = std::stod(numStr);
            return Token(NUMBER, value);
        } catch (const std::exception&) {
            throw LexicalError("无效的数字格式: " + numStr);
        }
    }
    
    // 标识符（函数名或常量）
    if (std::isalpha(ch)) {
        size_t start = pos;
        while (pos < expression.length() && std::isalnum(expression[pos])) {
            pos++;
        }
        std::string name = expression.substr(start, pos - start);
        
        // 检查是否为常量
        if (Constants::isConstant(name)) {
            return Token(CONSTANT, 0.0, name);
        }
        
        // 检查是否为函数
        if (Functions::isFunction(name)) {
            return Token(FUNCTION, 0.0, name);
        }
        
        throw LexicalError("未知的标识符: " + name);
    }
    
    // 操作符
    if (isOperator(ch)) {
        pos++;
        return Token(OPERATOR, 0.0, "", ch);
    }
    
    // 括号
    if (ch == '(') {
        pos++;
        return Token(LPAREN);
    }
    
    if (ch == ')') {
        pos++;
        return Token(RPAREN);
    }
    
    throw LexicalError("未知字符: " + std::string(1, ch));
}

void Parser::consumeToken() {
    currentToken = getNextToken();
}

bool Parser::isOperator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '^';
}

int Parser::getOperatorPrecedence(char op) {
    switch (op) {
        case '+':
        case '-':
            return 1;
        case '*':
        case '/':
            return 2;
        case '^':
            return 3;
        default:
            return 0;
    }
}

std::shared_ptr<ASTNode> Parser::parseExpression() {
    auto left = parseTerm();
    
    while (currentToken.type == OPERATOR && 
           (currentToken.op == '+' || currentToken.op == '-')) {
        char op = currentToken.op;
        consumeToken(); // 消费操作符
        auto right = parseTerm();
        auto node = std::make_shared<ASTNode>(BIN_OP_NODE);
        node->op = op;
        node->left = left;
        node->right = right;
        left = node;
    }
    
    return left;
}

std::shared_ptr<ASTNode> Parser::parseTerm() {
    auto left = parseFactor();
    
    while (currentToken.type == OPERATOR && 
           (currentToken.op == '*' || currentToken.op == '/' || currentToken.op == '^')) {
        char op = currentToken.op;
        consumeToken(); // 消费操作符
        auto right = parseFactor();
        auto node = std::make_shared<ASTNode>(BIN_OP_NODE);
        node->op = op;
        node->left = left;
        node->right = right;
        left = node;
    }
    
    return left;
}

std::shared_ptr<ASTNode> Parser::parseFactor() {
    Token token = currentToken;
    
    // 处理数字
    if (token.type == NUMBER) {
        consumeToken();
        auto node = std::make_shared<ASTNode>(NUM_NODE);
        node->value = token.value;
        return node;
    }
    
    // 处理常量
    if (token.type == CONSTANT) {
        consumeToken();
        auto node = std::make_shared<ASTNode>(CONSTANT_NODE);
        node->name = token.name;
        return node;
    }
    
    // 处理函数调用
    if (token.type == FUNCTION) {
        std::string funcName = token.name;
        consumeToken(); // 消费函数名
        
        if (currentToken.type != LPAREN) {
            throw SyntaxError("函数调用需要左括号");
        }
        consumeToken(); // 消费左括号
        
        auto node = std::make_shared<ASTNode>(FUNC_CALL_NODE);
        node->name = funcName;
        
        // 解析参数列表
        if (currentToken.type != RPAREN) {
            node->args.push_back(parseExpression());
            while (currentToken.type == OPERATOR && currentToken.op == ',') {
                consumeToken(); // 消费逗号
                node->args.push_back(parseExpression());
            }
        }
        
        if (currentToken.type != RPAREN) {
            throw SyntaxError("缺少右括号");
        }
        consumeToken(); // 消费右括号
        
        return node;
    }
    
    // 处理一元操作符
    if (token.type == OPERATOR && (token.op == '+' || token.op == '-')) {
        char op = token.op;
        consumeToken(); // 消费操作符
        auto operand = parseFactor();
        auto node = std::make_shared<ASTNode>(UNARY_OP_NODE);
        node->op = op;
        node->operand = operand;
        return node;
    }
    
    // 处理括号表达式
    if (token.type == LPAREN) {
        consumeToken(); // 消费左括号
        auto expr = parseExpression();
        if (currentToken.type != RPAREN) {
            throw SyntaxError("缺少右括号");
        }
        consumeToken(); // 消费右括号
        return expr;
    }
    
    throw SyntaxError("意外的标记");
}