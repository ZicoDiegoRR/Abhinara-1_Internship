#ifndef ANIMAL_HUNT_MECHANISM
#define ANIMAL_HUNT_MECHANISM

//Includes
#include <vector>
#include <string>
#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <ctime>
#include "types/all.h"
#include "include_all_animals.h"
using namespace std;

class Animal: public All_Animal{
    private:
        All_Animal* hunt_animal;
        void select_animal();
        void success_hunt_output(string animal, bool success);
    protected:
        vector<string> activity = {
            "eating", "sleeping", "searching for food",
            "threatened by your presence", "mating"
        };
        vector<int> spawning_chances_sum = {0, 5, 25, 60, 77, 100};
    public:
        int start_hunting(int weapon_chance);
};

#endif