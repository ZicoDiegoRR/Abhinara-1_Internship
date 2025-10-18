#ifndef SHEEP_STATS
#define SHEEP_STATS

#include <vector>
#include <string>
#include <ctime>
#include <cstdlib>
#include "all.h"
using namespace std;

class Sheep_Stats: public All_Animal {
    private:
        string name = "sheep";
        int reward = 23;
        vector<int> collection_of_chances = {20, 33, 18, 11, 22};

    public:
        vector<int> get_random_success_rate() override;
        int get_reward() override;
        string get_name() override;
};

#endif