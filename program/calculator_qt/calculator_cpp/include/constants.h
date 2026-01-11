#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <string>
#include <unordered_map>

class Constants {
public:
    static const std::unordered_map<std::string, double>& getConstants();
    static bool isConstant(const std::string& name);
    static double getValue(const std::string& name);

private:
    static std::unordered_map<std::string, double> constants;
    static bool initialized;
    static void initialize();
};

#endif // CONSTANTS_H