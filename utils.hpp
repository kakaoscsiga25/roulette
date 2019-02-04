#ifndef UTILS_HPP
#define UTILS_HPP

#include <sstream>
#include <math.h>


double COLOR_TH = .1;
//double COLOR_TH = 99.99999;
double ALL_MONEY = 7;
double MIN_BET = 1.;

const int FIELD_NUMBER = 37;


template <class T>
std::string num2string(const T& num, int precision = 2)
{
    std::stringstream ss;
    ss.precision(precision);
    ss << num;
    return ss.str();
}

enum StringColor
{
    NORMAL = 0,
    RED = 1,
    GREEN = 2,
};

std::string coloringString(const std::string& str, StringColor color)
{
    std::string prefix = "";
    if (color == StringColor::RED)
        prefix = "\033[1;31m";
    if (color == StringColor::GREEN)
        prefix = "\033[1;32m";

    std::string postfix = "\033[0m";

    return prefix + str + postfix;
}

#endif // UTILS_HPP
