#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <vector>
#include <memory>
#include "error.h"

// Token类型枚举
enum TokenType {
    NUMBER,
    OPERATOR,
    FUNCTION,
    CONSTANT,
    LPAREN,
    RPAREN,
    END
};

// Token结构
struct Token {
    TokenType type;
    double value;      // 当type为NUMBER时使用
    std::string name;  // 当type为FUNCTION或CONSTANT时使用
    char op;           // 当type为OPERATOR时使用
    
    Token(TokenType t, double v = 0.0, const std::string& n = "", char o = 0)
        : type(t), value(v), name(n), op(o) {}
};

// AST节点类型枚举
enum NodeType {
    NUM_NODE,
    BIN_OP_NODE,
    UNARY_OP_NODE,
    FUNC_CALL_NODE,
    CONSTANT_NODE
};

// AST节点结构
struct ASTNode {
    NodeType type;
    double value;              // 当type为NUM_NODE时使用
    char op;                   // 当type为BIN_OP_NODE或UNARY_OP_NODE时使用
    std::string name;          // 当type为FUNC_CALL_NODE或CONSTANT_NODE时使用
    std::shared_ptr<ASTNode> left;   // 左子树
    std::shared_ptr<ASTNode> right;  // 右子树
    std::shared_ptr<ASTNode> operand; // 操作数（用于一元运算）
    std::vector<std::shared_ptr<ASTNode>> args; // 函数参数列表
    
    ASTNode(NodeType t) : type(t), value(0), op(0) {}
};

// 解析器类
class Parser {
public:
    Parser(const std::string& expression);
    std::shared_ptr<ASTNode> parse();

private:
    std::string expression;
    size_t pos;
    Token currentToken;
    
    std::vector<Token> tokenize();
    Token getNextToken();
    void consumeToken();
    
    std::shared_ptr<ASTNode> parseExpression();
    std::shared_ptr<ASTNode> parseTerm();
    std::shared_ptr<ASTNode> parseFactor();
    
    void skipWhitespace();
    bool isOperator(char c);
    int getOperatorPrecedence(char op);
};

#endif // PARSER_H