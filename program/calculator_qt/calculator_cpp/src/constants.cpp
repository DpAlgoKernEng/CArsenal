#include "constants.h"
#include <cmath>

std::unordered_map<std::string, double> Constants::constants;
bool Constants::initialized = false;

void Constants::initialize() {
    if (!initialized) {
        constants["pi"] = M_PI;
        constants["e"] = M_E;
        initialized = true;
    }
}

const std::unordered_map<std::string, double>& Constants::getConstants() {
    if (!initialized) {
        initialize();
    }
    return constants;
}

bool Constants::isConstant(const std::string& name) {
    if (!initialized) {
        initialize();
    }
    return constants.find(name) != constants.end();
}

double Constants::getValue(const std::string& name) {
    if (!initialized) {
        initialize();
    }
    auto it = constants.find(name);
    if (it != constants.end()) {
        return it->second;
    }
    return 0.0;
}