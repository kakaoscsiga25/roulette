#ifndef FIELDS_HPP
#define FIELDS_HPP

#include <vector>
#include <math.h>
#include <cmath>
#include <assert.h>


struct Field
{
    enum Color
    {
        black = 1,
        red = 2,
        green = 3,
        unk = 0
    };
    Field(int val) : value(val), color(unk)
    {
        std::vector<int> reds = { 1,3,5,7,9, 12,14,16,18,19, 21,23,25,27, 30,32,34,36};
        std::vector<int> blacks = {2,4,6,8, 10,11,13,15,17, 20,22,24,26,28,29, 31,33,35};
        if (value == 0) color = green;
        for (const auto& v : reds)
            if (v == value)
            {
                assert(color == unk);
                color = red;
            }
        for (const auto& v : blacks)
            if (v == value)
            {
                assert(color == unk);
                color = black;
            }
        assert (color != unk);

    }

    double calcProbability(const std::vector<int>& hist, int fieldsSize, double& overall) const
    {
        bool found = false;
        int last = 0;
        int numOfGet = 0;
        int numOfOther = 0;
        for (std::vector<int>::const_reverse_iterator it = hist.rbegin(); it != hist.rend(); it++)
        {
            bool equal = *it == value;
            found = found || equal;
            if (!found)
                last++;
            if (equal)
                numOfGet++;
            else
                numOfOther++;
        }
        // Calculations
        double estimatedProbab = std::pow((static_cast<double>(fieldsSize-1) / fieldsSize), last+1);
        overall = static_cast<double>(numOfGet) / numOfOther;

        return estimatedProbab;
    }
    int value;
    Color color;
};

#endif // FIELDS_HPP
