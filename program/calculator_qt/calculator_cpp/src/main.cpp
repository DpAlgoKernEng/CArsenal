#include "ui.h"
#include "parser.h"
#include "calculator.h"
#include "error.h"
#include <iostream>
#include <string>

int main() {
    UI::showWelcome();
    
    while (true) {
        std::string input = UI::getUserInput();
        
        // 检查退出命令
        if (input == "quit" || input == "exit") {
            std::cout << "感谢使用科学计算器！\n";
            break;
        }
        
        // 检查帮助命令
        if (input == "help") {
            UI::showHelp();
            continue;
        }
        
        // 跳过空输入
        if (input.empty()) {
            continue;
        }
        
        try {
            // 解析表达式
            Parser parser(input);
            std::shared_ptr<ASTNode> ast = parser.parse();
            
            // 计算结果
            Calculator calc;
            double result = calc.evaluate(ast);
            
            // 显示结果
            UI::showResult(result);
        } catch (const CalcError& e) {
            UI::showError(e.what());
        } catch (const std::exception& e) {
            UI::showError("未知错误: " + std::string(e.what()));
        }
    }
    
    return 0;
}