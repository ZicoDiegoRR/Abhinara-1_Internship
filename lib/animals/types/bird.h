#ifndef BIRD_STATS
#define BIRD_STATS

#include <vector>
#include <string>
#include <ctime>
#include <cstdlib>
#include "all.h"
using namespace std;

class Bird_Stats: public All_Animal {
    private:
        string name = "bird";
        int reward = 20;
        vector<int> collection_of_chances = {19, 31, 16, 10, 20};

    public:
        vector<int> get_random_success_rate() override;
        int get_reward() override;
        string get_name() override;
};

#endif