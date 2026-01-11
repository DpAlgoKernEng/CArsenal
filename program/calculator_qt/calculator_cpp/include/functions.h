#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <string>
#include <vector>
#include <functional>
#include <unordered_map>

class Functions {
public:
    using FunctionType = std::function<double(const std::vector<double>&)>;

    static const std::unordered_map<std::string, FunctionType>& getFunctions();
    static bool isFunction(const std::string& name);
    static double evaluate(const std::string& name, const std::vector<double>& args);

private:
    static std::unordered_map<std::string, FunctionType> functions;
    static bool initialized;
    static void initialize();
};

#endif // FUNCTIONS_H