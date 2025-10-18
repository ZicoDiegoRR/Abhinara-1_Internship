#ifndef FOX_STATS
#define FOX_STATS

#include <vector>
#include <string>
#include <ctime>
#include <cstdlib>
#include "all.h"
using namespace std;

class Fox_Stats: public All_Animal {
    private:
        string name = "fox";
        int reward = 27;
        vector<int> collection_of_chances = {21, 34, 17, 9, 21};

    public:
        vector<int> get_random_success_rate() override;
        int get_reward() override;
        string get_name() override;
};

#endif