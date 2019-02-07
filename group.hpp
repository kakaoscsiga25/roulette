#ifndef GROUP_HPP
#define GROUP_HPP

#include <string>
#include <iostream>

#include "fields.hpp"
#include "utils.hpp"


struct Group
{
    Group(size_t histSize = 5)
    {
        winHistory = std::vector<uint>(histSize, 0);
    }

    bool win(int number) const
    {
        for (const auto& rp : rightFields)
            if (rp->value == number)
                return true;
        return false;
    }

    void update(int newNumber)
    {
//        winHistory[1]++;
//        return;

        if (win(newNumber))
        {
//            std::cerr << "WIN\n";
            size_t idx = counter;
            if (idx < winHistory.size())
                winHistory.at(idx)++;
            counter = 0;
        }
        else
            counter++;
    }

    double groupProbability() const { return static_cast<double>(rightFields.size()) / FIELD_NUMBER; }

    double multiplier() const { return (FIELD_NUMBER-1) / rightFields.size(); }

    std::vector<int> calcNs() const
    {
        std::vector<int> Ns;
        assert(!winHistory.empty());
        for (int i = 0; i < winHistory.size() - 1; i++)
        {
            uint h1 = winHistory.at(i);
            uint h2 = winHistory.at(i+1);
            int N = -1;
            if (h2 > h1)
            {
                double P1 = groupProbability();
                double P2 = (1.-groupProbability()) * groupProbability();
                // Find N
                N = 0;
                bool run = true;
                while (P1*N + h1 < P2*N + h2)
                    N++;
            }
            Ns.push_back(N);
        }
        return Ns;
    }

    double expValue() const
    {
        assert(!winHistory.empty());
        if (counter >= winHistory.size()-1)
            return 0;

        uint h1 = winHistory.at(counter);
        uint h2 = winHistory.at(counter+1);
        if (h1 >= h2)
            return 0;

        std::vector<int> Ns = calcNs();
        int N = Ns.at(counter);
        double multip = multiplier();

        const int W = 100;

//        double expValue = N * (h2-h1) / multip;
//        double expValue = (h2-h1)*multip - N;
        double expValue = static_cast<double>(N) / multip;
        return expValue;
    }

    void print() const
    {
        std::cerr << ID << "\n";
        for (const auto& wh : winHistory)
            std::cerr << wh << ", ";
        std::cerr << "\n";
        for (const auto& n : calcNs())
            std::cerr << n << ", ";
        std::cerr << "\n";
        std::cerr << "counter " << counter << "\n";
    }


    int counter = 0;

    std::string ID;
    std::vector<const Field*> rightFields;
    std::vector<uint> winHistory;
};


std::vector<Group> createGroups(const std::vector<Field>& fields)
{
    std::vector<Group> groups;
    Group group;

    group.ID = "TEST";
    group.rightFields.push_back(&fields[0]);
    groups.push_back(group);
    group.rightFields.clear();


    group.ID = "EVEN";
    for (const auto& f : fields)
        if (f.value%2 == 0 && f.value != 0)
            group.rightFields.push_back(&f);
//    groups.push_back(group);
    group.rightFields.clear();

    return groups;


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

    // Sixes
    for (int i = 1; i < FIELD_NUMBER-4; i+=3)
    {
        group.ID = "SIX [" + std::to_string(i) + "," + std::to_string(i+3) + "]";
        for (int ii = 0; ii < 6; ii++)
            group.rightFields.push_back(&fields.at(i+ii));
        groups.push_back(group);
        group.rightFields.clear();
    }

    return groups;
}


#endif // GROUP_HPP
