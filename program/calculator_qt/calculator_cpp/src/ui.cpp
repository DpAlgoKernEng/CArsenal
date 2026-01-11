#include "ui.h"
#include <iostream>
#include <limits>

void UI::showWelcome() {
    std::cout << "========================================\n";
    std::cout << "       科学计算器 v1.0\n";
    std::cout << "========================================\n";
    std::cout << "支持基本算术运算、科学函数、常量等\n";
    std::cout << "输入 'help' 查看帮助，输入 'quit' 退出\n";
    std::cout << "========================================\n\n";
}

void UI::showHelp() {
    std::cout << "\n========== 帮助信息 ==========\n";
    std::cout << "支持的运算符:\n";
    std::cout << "  +, -, *, /, ^ (幂运算)\n\n";
    std::cout << "支持的函数:\n";
    std::cout << "  sin, cos, tan, log, ln, exp, sqrt, abs\n\n";
    std::cout << "支持的常量:\n";
    std::cout << "  pi, e\n\n";
    std::cout << "示例:\n";
    std::cout << "  2 + 3 * 4\n";
    std::cout << "  sin(pi/2)\n";
    std::cout << "  sqrt(16) + log(100)\n";
    std::cout << "=============================\n\n";
}

std::string UI::getUserInput() {
    std::cout << ">>> ";
    std::string input;
    std::getline(std::cin, input);
    return input;
}

void UI::showResult(double result) {
    std::cout << "= " << result << "\n\n";
}

void UI::showError(const std::string& error) {
    std::cout << "错误: " << error << "\n\n";
}

bool UI::shouldContinue() {
    return true; // 主循环控制在main函数中
}