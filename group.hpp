#ifndef GROUP_HPP
#define GROUP_HPP

#include <string>
#include <iostream>

#include "fields.hpp"
#include "utils.hpp"


struct Group
{
    double groupProbab(const std::vector<int>& hist, int maxBet) const
    {
        bool found = false;
        int last = 0;
        for (std::vector<int>::const_reverse_iterator it = hist.rbegin(); it != hist.rend() && !found; it++)
        {
            for (const auto& field : rightFields)
            {
                bool equal = *it == field->value;
                found = found || equal;
            }
            if (!found)
                last++;
        }
        // Calculations
        double estimatedProbab = std::pow((static_cast<double>(FIELD_NUMBER - rightFields.size()) / FIELD_NUMBER), last+maxBet);

        return estimatedProbab;
    }

    double rise() const { return (FIELD_NUMBER-1.) / rightFields.size(); }
    int maxBet(double allMoney, double minBet)
    {
        double rise_mult = rise();

        double spent = 0;
        double bet = 0;
        double actBet = minBet;
        bets.clear();
        while(1)
        {
            while ((actBet * rise_mult) - (spent + actBet) <= 0)
                actBet += minBet;
            if (actBet + spent > allMoney)
                break;

            spent += actBet;
            bet++;
            bets.push_back(actBet);
        }
//        std::cerr << bets.size() << "\n";
        return bet;
    }

    std::string toString() const
    {
        std::string printString = ID + ": " + num2string(probab*100., 6);
        bool colored = probab >= COLOR_TH/100.;
        if (colored)
            printString = "\033[1;32m" + printString + "\033[0m";
        printString += " [ ";
        for (const auto& bet : bets)
            printString += num2string(bet, 4) + " ";
        printString += " ]";
        return printString;
    }
    std::string info() const
    {
        std::string ret = ID + " { ";
        for (const auto& rp : rightFields)
            ret += std::to_string(rp->value) + ", ";
        ret += " }";
        return  ret;
    }

    std::string ID;
    std::vector<const Field*> rightFields;
    double probab;
    std::vector<double> bets;
};

#endif // GROUP_HPP
