#include <vector>
#include <iostream>
#include <algorithm>

#include "fields.hpp"
#include "group.hpp"
#include "utils.hpp"



std::vector<Group> createGroups(const std::vector<Field>& fields);

int main(int argc, char *argv[])
{
    srand(time(0));
    std::vector<Field> fields;

    for (int i = 0; i < FIELD_NUMBER; i++)
    {
        Field f(i);
        fields.push_back(f);
    }
//    std::cerr << fields.size() <<"\n";

    // Create groups
    std::vector<Group> groups = createGroups(fields);
    for (auto g : groups)
    {
//        std::cerr << g.info() << "\n";
    }

    // TEST
    double exp_value = 0;
    double exp_value_pre = 0;
    uint ws = 0;
    ulong sumRun = 0;

    // Spins
    std::vector<int> history;
    while(1)
    {
        int spinValue;
        spinValue = rand()%FIELD_NUMBER;
        sumRun++;
//        std::cout << "Next number: " << spinValue;
//        std::cin >> spinValue; std::cout << "\n";

        if (spinValue == -1)
            break;
        if (spinValue < 0 || spinValue > fields.size())
        {
            std::cout << "WRONG NUMBER\n";
            continue;
        }

        history.push_back(spinValue);

        // Calc stats
        for (auto& group : groups)
        {
            group.probab = 1. - group.groupProbab(history, group.maxBet(ALL_MONEY, MIN_BET));
        }

        // Sort
        std::sort(groups.begin(), groups.end(), [&](const Group& g1, const Group& g2) -> bool { return g1.probab > g2.probab; } );
//        for (int i = 0; i < groups.size() && i < 5; i++)
//            std::cout << groups[i].toString() << "\n";

        // TEST
        Group testg = groups.front();
        double th = COLOR_TH/100.;
        if (testg.rightFields.size() > 2)
            th = .999;
        if (testg.probab >= th)
        {
            int maxBet = testg.maxBet(ALL_MONEY, MIN_BET);
            auto history_tmp = history;
            double probab_pre = testg.probab;
            int idx = 1;
            bool k = false;
            for (;idx<maxBet;idx++)
            {
                int test = rand()%FIELD_NUMBER;
                sumRun++;
                history_tmp.push_back(test);
                testg.probab = 1. - testg.groupProbab(history_tmp, testg.maxBet(ALL_MONEY, MIN_BET));
                if (testg.probab < probab_pre)
                {
                    k = true;
                    break;
                }
            }

            /*
            int series = maxBet;
            bool run = true;
            while(run)
            {
                int test = rand()%FIELD_NUMBER;
                sumRun++;
                for (const auto& f : testg.rightFields)
                    if(f->value == test)
                        run=false;
                if(run)
                    series++;
            }
            if (series > ws)
            {
                ws = series;
                std::cout << "Worst series: " << ws << " " << testg.ID << " sumrun:" << sumRun << "\n";
            }
            */

            double cost = 0;
            for (int i=0;i<idx;i++)
                cost += testg.bets.at(i);
            double win = testg.rise() * testg.bets.at(idx-1);
            if (!k)
            {
                win = 0;
                history.push_back(testg.rightFields.front()->value); // more hack
            }
            exp_value += win - cost;
        }
        if (exp_value != exp_value_pre)
        {
            std::cout << "Hist:" << history.size() << "Exp value: " << exp_value << " " << testg.ID << "\n";
            exp_value_pre = exp_value;
        }
        if (sumRun > 500)
            break;
        continue;

        std::cout << "Round " << history.size() << "\n";
        std::cout << "History: ";
        for (auto it = history.rbegin(); it != history.rend() && it < history.rbegin()+10; it++)
            std::cout << *it << " ";
        std::cout << "\n";
    }

    return 0;
}

