#ifndef BEAR_STATS
#define BEAR_STATS

#include <vector>
#include <string>
#include <ctime>
#include <cstdlib>
#include "all.h"
using namespace std;

class Bear_Stats: public All_Animal {
    private:
        string name = "bear";
        int reward = 90;
        vector<int> collection_of_chances = {14, 25, 12, 8, 13};

    public:
        vector<int> get_random_success_rate() override;
        int get_reward() override;
        string get_name() override;
};

#endif