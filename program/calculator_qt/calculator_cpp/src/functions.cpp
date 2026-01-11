#include "functions.h"
#include <cmath>
#include <stdexcept>

std::unordered_map<std::string, Functions::FunctionType> Functions::functions;
bool Functions::initialized = false;

void Functions::initialize() {
    if (!initialized) {
        // Trigonometric functions
        functions["sin"] = [](const std::vector<double>& args) -> double {
            if (args.size() != 1) throw std::invalid_argument("sin函数需要1个参数");
            return std::sin(args[0]);
        };
        
        functions["cos"] = [](const std::vector<double>& args) -> double {
            if (args.size() != 1) throw std::invalid_argument("cos函数需要1个参数");
            return std::cos(args[0]);
        };
        
        functions["tan"] = [](const std::vector<double>& args) -> double {
            if (args.size() != 1) throw std::invalid_argument("tan函数需要1个参数");
            return std::tan(args[0]);
        };
        
        // Logarithmic functions
        functions["log"] = [](const std::vector<double>& args) -> double {
            if (args.size() != 1) throw std::invalid_argument("log函数需要1个参数");
            if (args[0] <= 0) throw std::invalid_argument("log函数的参数必须大于0");
            return std::log10(args[0]);
        };
        
        functions["ln"] = [](const std::vector<double>& args) -> double {
            if (args.size() != 1) throw std::invalid_argument("ln函数需要1个参数");
            if (args[0] <= 0) throw std::invalid_argument("ln函数的参数必须大于0");
            return std::log(args[0]);
        };
        
        // Exponential functions
        functions["exp"] = [](const std::vector<double>& args) -> double {
            if (args.size() != 1) throw std::invalid_argument("exp函数需要1个参数");
            return std::exp(args[0]);
        };
        
        // Power functions
        functions["sqrt"] = [](const std::vector<double>& args) -> double {
            if (args.size() != 1) throw std::invalid_argument("sqrt函数需要1个参数");
            if (args[0] < 0) throw std::invalid_argument("sqrt函数的参数不能为负数");
            return std::sqrt(args[0]);
        };
        
        // Absolute value
        functions["abs"] = [](const std::vector<double>& args) -> double {
            if (args.size() != 1) throw std::invalid_argument("abs函数需要1个参数");
            return std::abs(args[0]);
        };
        
        initialized = true;
    }
}

const std::unordered_map<std::string, Functions::FunctionType>& Functions::getFunctions() {
    if (!initialized) {
        initialize();
    }
    return functions;
}

bool Functions::isFunction(const std::string& name) {
    if (!initialized) {
        initialize();
    }
    return functions.find(name) != functions.end();
}

double Functions::evaluate(const std::string& name, const std::vector<double>& args) {
    if (!initialized) {
        initialize();
    }
    auto it = functions.find(name);
    if (it != functions.end()) {
        return it->second(args);
    }
    throw std::invalid_argument("未知函数: " + name);
}