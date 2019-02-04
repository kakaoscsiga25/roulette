#include <unistd.h>

#include "utils.hpp"
#include "group.hpp"


int main()
{
    srand(time(0));

    Field f(0);
    Group g;
    g.rightFields.push_back(&f);

    std::vector<uint> histogram(50, 0);
    size_t hidx = 0;
    size_t run = 0;
    size_t newRound = 0;
    while(1)
    {
        int val = rand()%(FIELD_NUMBER);
        run++;

        if (val == 10)
        {
            histogram.at(std::min(hidx, histogram.size()-1))++;
            hidx = 0;
            newRound++;
        }
        else
            hidx++;

        // print histogram
//        if (run%100000000 == 0)
//        if (hidx == 0)
        if (run >= 100000)
        {
            for (size_t i = 0; i < histogram.size(); i++)
            {
                const auto& h = histogram[i];
                bool ok = true;
                if (i > 0)
                    ok = histogram[i-1] > histogram[i];
                std::string str = std::to_string(h);

                // Calc real
                double calcValue = std::pow((FIELD_NUMBER-1.) / FIELD_NUMBER, i) * (1./FIELD_NUMBER);
                double shouldBe = calcValue * newRound;
                double diffVal = h - shouldBe;
                std::string diffStr = std::to_string(diffVal);
                if (diffVal < 0.)
                    diffStr = coloringString(diffStr, StringColor::GREEN);
                else
                    diffStr = coloringString(diffStr, StringColor::RED);
                str += "\t" + std::to_string(shouldBe) + "\t" + diffStr;

                if (ok)
                    str = coloringString(str, StringColor::GREEN);
                else
                    str = coloringString(str, StringColor::RED);
                std::cout << i+1 << "\t" << str << "\n";
            }
            std::cout << run << "\n";
            std::cout << "\n";

            usleep(.1 * 1000*1000);

            break;
        }
    }

    return 0;
}
