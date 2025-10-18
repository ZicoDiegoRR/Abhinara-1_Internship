#ifndef PLAYER_STATS
#define PLAYER_STATS

#include <string>
#include <iostream>
#include <unistd.h>
#include "../weapons/all_rifle.h"
using namespace std;

class Stats {
    private:
        string name;
        int money = 0;
        All_Rifles rifle;

    public:
        Stats();
        void set_name(string input_name);
        void set_money(int amount);
        void add_money(int amount);
        void get_info();
        void access_shop();

        int get_money();
        int get_weapon_rate();
};

#endif