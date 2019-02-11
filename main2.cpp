#include <algorithm>

#include "utils.hpp"
#include "fields.hpp"
#include "group.hpp"


int main()
{
    srand(time(0));

    // Create fields
    std::vector<Field> fields;
    for (int i = 0; i < FIELD_NUMBER; i++)
        fields.push_back(Field(i));

    int all = 0;
    for (int x=0;x<1000;x++)
    {

    // Create groups
    std::vector<Group> groups = createGroups(fields);
    std::vector<int> bet(groups.size(), 0);

    // RUN
    int MONEY = 0;
    for (size_t spinCounter = 0; spinCounter < 1000000; spinCounter++)
    {
        // Bet
        bool cont = false;
        for (size_t i = 0; i < groups.size(); i++)
        {
            const auto& g = groups[i];
//            g.print();
            double val = g.expValue();
            if (val > 0)
            {
                bet[i] = 1;
//                std::cerr << "Bet for: " << g.ID << "\n";
            }
            else
                bet[i] = 0;
            MONEY -= bet[i];
        }

        // hacky
        int item = rand()%FIELD_NUMBER;
//        spinCounter++;


        // hack
//        if (groups.front().counter > groups.front().winHistory.size())
//            groups.front().counter = 0;

//        std::cerr << groups.front().counter << "\n";

        for (auto& g : groups)
        {
            g.update(item);
//            if (g.counter > g.winHistory.size())
//                cont = true;
        }
//        if (cont)
//            continue;

        // sort group items
//        std::sort(groups.begin(), groups.end(), [&] (const Group& g1, const Group& g2) { return 0; } );

//        std::cerr << spinCounter << ". spin: " << item << "\n";

        for (size_t i = 0; i < groups.size(); i++)
        {
            const auto& g = groups[i];
//            g.print();

            bool win = g.win(item);
            if (bet[i])
            {
                if (win)
                    MONEY += g.multiplier() * bet[i];
            }
        }
//        std::cerr << "My money: " << MONEY << "\n\n";
    }
    all += MONEY;
    std::cerr << all << "\n";
    }



    return 0;
}
