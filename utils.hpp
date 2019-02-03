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

#endif // UTILS_HPP
