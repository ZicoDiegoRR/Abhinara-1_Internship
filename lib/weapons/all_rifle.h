#ifndef ALL_RIFLE
#define ALL_RIFLE

#include <string>
#include <vector>
using namespace std;

class All_Rifles {
    private:
        All_Rifles* weapon;
        vector<int> available = {0, 1, 2, 3, 4};

    public:
        virtual string get_name();
        virtual string get_desc();
        virtual int get_price();
        virtual int get_rate();
        void set_weapon_in_use(int type_index);
        void display_weapon(bool shop);
        int shop(int money);
};

#endif