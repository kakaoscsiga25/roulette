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

std::vector<Group> createGroups(const std::vector<Field>& fields)
{
    std::vector<Group> groups;
    Group group;

    group.ID = "EVEN";
    for (const auto& f : fields)
        if (f.value%2 == 0 && f.value != 0)
            group.rightFields.push_back(&f);
    groups.push_back(group);
    group.rightFields.clear();

    group.ID = "ODD";
    for (const auto& f : fields)
        if (f.value%2 == 1 && f.value != 0)
            group.rightFields.push_back(&f);
    groups.push_back(group);
    group.rightFields.clear();

    group.ID = "1st 12";
    for (const auto& f : fields)
        if (f.value <= 12 && f.value != 0)
            group.rightFields.push_back(&f);
    groups.push_back(group);
    group.rightFields.clear();
    group.ID = "2st 12";
    for (const auto& f : fields)
        if (f.value > 12 && f.value <= 24 && f.value != 0)
            group.rightFields.push_back(&f);
    groups.push_back(group);
    group.rightFields.clear();
    group.ID = "3st 12";
    for (const auto& f : fields)
        if (f.value > 24 && f.value <= 36 && f.value != 0)
            group.rightFields.push_back(&f);
    groups.push_back(group);
    group.rightFields.clear();

    group.ID = "1 to 18";
    for (const auto& f : fields)
        if (f.value <= 18 && f.value != 0)
            group.rightFields.push_back(&f);
    groups.push_back(group);
    group.rightFields.clear();
    group.ID = "19 to 36";
    for (const auto& f : fields)
        if (f.value > 18 && f.value != 0)
            group.rightFields.push_back(&f);
    groups.push_back(group);
    group.rightFields.clear();

    // COLORS
    group.ID = "BLACK";
    for (const auto& f : fields)
        if (f.color == Field::Color::black)
            group.rightFields.push_back(&f);
    groups.push_back(group);
    group.rightFields.clear();
    group.ID = "RED";
    for (const auto& f : fields)
        if (f.color == Field::Color::red)
            group.rightFields.push_back(&f);
    groups.push_back(group);
    group.rightFields.clear();

    group.ID = "ROW 1";
    for (const auto& f : fields)
        if (f.value%3 == 1 && f.value != 0)
            group.rightFields.push_back(&f);
    groups.push_back(group);
    group.rightFields.clear();
    group.ID = "ROW 2";
    for (const auto& f : fields)
        if (f.value%3 == 2 && f.value != 0)
            group.rightFields.push_back(&f);
    groups.push_back(group);
    group.rightFields.clear();
    group.ID = "ROW 3";
    for (const auto& f : fields)
        if (f.value%3 == 0 && f.value != 0)
            group.rightFields.push_back(&f);
    groups.push_back(group);
    group.rightFields.clear();

    // COLS
    {
        group.ID = "COL 1";
        for (const auto& f : fields)
            if ((f.value-1)/3 == 0 && f.value != 0)
                group.rightFields.push_back(&f);
        groups.push_back(group);
        group.rightFields.clear();
        group.ID = "COL 4";
        for (const auto& f : fields)
            if ((f.value-1)/3 == 1 && f.value != 0)
                group.rightFields.push_back(&f);
        groups.push_back(group);
        group.rightFields.clear();
        group.ID = "COL 7";
        for (const auto& f : fields)
            if ((f.value-1)/3 == 2 && f.value != 0)
                group.rightFields.push_back(&f);
        groups.push_back(group);
        group.rightFields.clear();
        group.ID = "COL 10";
        for (const auto& f : fields)
            if ((f.value-1)/3 == 3 && f.value != 0)
                group.rightFields.push_back(&f);
        groups.push_back(group);
        group.rightFields.clear();
        group.ID = "COL 13";
        for (const auto& f : fields)
            if ((f.value-1)/3 == 4 && f.value != 0)
                group.rightFields.push_back(&f);
        groups.push_back(group);
        group.rightFields.clear();
        group.ID = "COL 16";
        for (const auto& f : fields)
            if ((f.value-1)/3 == 5 && f.value != 0)
                group.rightFields.push_back(&f);
        groups.push_back(group);
        group.rightFields.clear();
        group.ID = "COL 19";
        for (const auto& f : fields)
            if ((f.value-1)/3 == 6 && f.value != 0)
                group.rightFields.push_back(&f);
        groups.push_back(group);
        group.rightFields.clear();
        group.ID = "COL 22";
        for (const auto& f : fields)
            if ((f.value-1)/3 == 7 && f.value != 0)
                group.rightFields.push_back(&f);
        groups.push_back(group);
        group.rightFields.clear();
        group.ID = "COL 25";
        for (const auto& f : fields)
            if ((f.value-1)/3 == 8 && f.value != 0)
                group.rightFields.push_back(&f);
        groups.push_back(group);
        group.rightFields.clear();
        group.ID = "COL 28";
        for (const auto& f : fields)
            if ((f.value-1)/3 == 9 && f.value != 0)
                group.rightFields.push_back(&f);
        groups.push_back(group);
        group.rightFields.clear();
        group.ID = "COL 31";
        for (const auto& f : fields)
            if ((f.value-1)/3 == 10 && f.value != 0)
                group.rightFields.push_back(&f);
        groups.push_back(group);
        group.rightFields.clear();
        group.ID = "COL 34";
        for (const auto& f : fields)
            if ((f.value-1)/3 == 11 && f.value != 0)
                group.rightFields.push_back(&f);
        groups.push_back(group);
        group.rightFields.clear();
    }

    // Quads
    for (const auto& f : fields)
    {
        const int& val = f.value;
        if (val == 0 || val%3 == 0 || val >= 34)
            continue;
        group.rightFields.push_back(&f);
        group.rightFields.push_back(&fields.at(val+1));
        group.rightFields.push_back(&fields.at(val+3));
        group.rightFields.push_back(&fields.at(val+4));
        group.ID = "QUAD [ ";
        for (const auto& v : group.rightFields)
            group.ID += std::to_string(v->value) + " ";
        group.ID += "]";
        //        groups.push_back(group);
        group.rightFields.clear();
    }

    // Pairs
    for (const auto& f : fields)
    {
        const int& val = f.value;
        if (val == 0)
            continue;
        if (val%3 != 0)
        {
            group.rightFields.push_back(&f);
            group.rightFields.push_back(&fields.at(val+1));
            group.ID = "PAIR [ ";
            for (const auto& v : group.rightFields)
                group.ID += std::to_string(v->value) + " ";
            group.ID += "]";
            //            groups.push_back(group);
            group.rightFields.clear();
        }
        if (val < 34)
        {
            group.rightFields.push_back(&fields.at(val));
            group.rightFields.push_back(&fields.at(val+3));
            group.ID = "PAIR [ ";
            for (const auto& v : group.rightFields)
                group.ID += std::to_string(v->value) + " ";
            group.ID += "]";
            //            groups.push_back(group);
            group.rightFields.clear();
        }
    }

    // SPECIALS
    group.ID = "PAIR [ 0 1 ]";
    group.rightFields.push_back(&fields.at(0));
    group.rightFields.push_back(&fields.at(1));
    groups.push_back(group);
    group.rightFields.clear();
    group.ID = "PAIR [ 0 2 ]";
    group.rightFields.push_back(&fields.at(0));
    group.rightFields.push_back(&fields.at(2));
    groups.push_back(group);
    group.rightFields.clear();
    group.ID = "PAIR [ 0 3 ]";
    group.rightFields.push_back(&fields.at(0));
    group.rightFields.push_back(&fields.at(3));
    groups.push_back(group);
    group.rightFields.clear();
    group.ID = "TRIP [ 0 1 2 ]";
    group.rightFields.push_back(&fields.at(0));
    group.rightFields.push_back(&fields.at(1));
    group.rightFields.push_back(&fields.at(2));
    groups.push_back(group);
    group.rightFields.clear();
    group.ID = "TRIP [ 0 2 3 ]";
    group.rightFields.push_back(&fields.at(0));
    group.rightFields.push_back(&fields.at(2));
    group.rightFields.push_back(&fields.at(3));
    groups.push_back(group);
    group.rightFields.clear();


    // Singles
    for (const auto& f : fields)
    {
        group.ID = std::to_string(f.value);
        group.rightFields.push_back(&f);
        groups.push_back(group);
        group.rightFields.clear();
    }

    return groups;
}
