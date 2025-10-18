#ifndef ALL_ANIMAL
#define ALL_ANIMAL

#include <string>
#include <vector>
using namespace std;

class All_Animal {
    public:
        virtual ~All_Animal();
        virtual vector<int> get_random_success_rate();
        virtual int get_reward();
        virtual string get_name();
};

#endif