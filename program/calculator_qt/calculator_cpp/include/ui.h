#ifndef UI_H
#define UI_H

#include <string>

class UI {
public:
    static void showWelcome();
    static void showHelp();
    static std::string getUserInput();
    static void showResult(double result);
    static void showError(const std::string& error);
    static bool shouldContinue();
};

#endif // UI_H