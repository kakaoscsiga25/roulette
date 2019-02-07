#include <unistd.h>
#include <fstream>

#include "utils.hpp"
#include "group.hpp"


int test(std::vector<int>& almosts)
{
    srand(time(0));

    for (auto& a : almosts)
        a = -1;

    Field f(0);
    Group g;
    g.rightFields.push_back(&f);

    std::vector<uint> histogram(3, 0);
    size_t hidx = 0;
    size_t run = 0;
    size_t newRound = 0;
    while(1)
    {
        int val = rand()%(FIELD_NUMBER);
        run++;

        if (val == 0)
        {
            histogram.at(std::min(hidx, histogram.size()-1))++;
            hidx = 0;
            newRound++;
        }
        else
            hidx++;

        // print histogram
        if (run%100000 == 0 && false)
//        if (hidx == 0)
//        if (run >= 100000)
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

                // Calc probab
                double P_act = calcValue;
                double P_neg = (FIELD_NUMBER-1.) / FIELD_NUMBER;
                double probab = P_act * P_neg * diffVal;
                std::string probabStr = "\t" + std::to_string(probab);
                if (probab > 0)
                {
                    probabStr = coloringString(probabStr, StringColor::RED);
                }
                else
                    probabStr = coloringString(probabStr, StringColor::GREEN);
                str += probabStr;

                if (ok)
                    str = coloringString(str, StringColor::GREEN);
                else
                    str = coloringString(str, StringColor::RED);
                std::cout << i+1 << "\t" << str << "\n";
            }
            std::cout << run << "\n";
            std::cout << "\n";

//            usleep(.1 * 1000*1000);

//            break;
//            std::cin.ignore();
        }

        // OK?
        int allOkCount = histogram.size()-2;
        int okCount = 0;
        for (size_t i = 0; i < histogram.size()-1; i++) // skip last
        {
            const auto& h = histogram[i];
            if (i > 0)
            {
                bool ok = true;
                ok = histogram[i-1] > histogram[i];
                if (ok)
                    okCount++;
            }
        }
        int idx = allOkCount - okCount - 1;
        if (idx >= 0 && idx < almosts.size() && almosts.at(idx) == -1)
            almosts.at(idx) = run;
        if (okCount == allOkCount)
        {
//            std::cerr << "allok\n";
//            return run;
            return newRound;
        }
    }

    return 0;
}

int runAllTest()
{
    std::vector<std::vector<int> > results;

    for (int i=0;i<100000;i++)
    {

        std::vector<int> almost(5);
        int run = test(almost);
//        std::cerr << run << "\n";
//        for (auto a : almost)
//            std::cerr << a << "\n";

        results.push_back(std::vector<int>());
        results.back().push_back(run);
        for (auto a : almost)
            results.back().push_back(a);
    }

    std::ofstream of("./result.txt");
    for (auto v : results)
    {
        for (auto i : v)
            of << i << ", ";
        of << "\n";
    }
    of.close();

    return 0;
}

void evaluation()
{
    // read file
    std::ifstream f("./result.txt");
    std::vector<std::vector<int> > results;
    while(!f.eof())
    {
        results.push_back(std::vector<int>());
        std::string line;
        std::getline(f, line);
        size_t pos = 0;
        std::string linecp = line;
//        std::cerr << line << "\n";
        std::string delimiter = ",";
        while ((pos = line.find(delimiter)) != std::string::npos)
        {
            std::string token = line.substr(0, pos);
            int value = std::stoi(token);
//            std::cout << value << std::endl;
            line.erase(0, pos + delimiter.length());
            results.back().push_back(value);
        }
        if (results.back().empty())
            results.pop_back();
    }

    // Calc statistics
    assert(!results.empty());
    for (int idx = 0; idx < results.front().size(); idx++)
    {
        int minv = 1e50, maxv = 0;
        double avg = 0;
        for (int i = 0; i < results.size(); i++)
        {
            int val = results.at(i).at(idx);
            avg += val;
            minv = std::min(minv, val);
            maxv = std::max(maxv, val);
        }
        avg /= results.size();
        double stdDev = 0;
        for (int i = 0; i < results.size(); i++)
            stdDev += std::pow(results.at(i).at(idx) - avg, 2);
        stdDev /= results.size();
        stdDev = std::sqrt(stdDev);

        std::cerr << idx << " error, avg: " << int(avg) << " stdDev: " << int(stdDev) << " min-max:" << minv << " " << maxv << "\n";
    }

    // Calc conf
    double conf = 0.9999; // confidence
    int val = 0;
    bool run = true;
    while (run) {
        int in = 0;
        for (int idx = 0; idx < results.size(); idx++)
        {
            int v = results.at(idx).at(0); // just all
            if (v <= val)
                in++;
        }
        if (in >= conf*results.size())
            run = false;
        else
            val+=1;
    }
    std::cerr << val << " with conf " << conf << " all meas: " << results.size() << "\n";
}

int main()
{
    runAllTest();

    evaluation();
}
