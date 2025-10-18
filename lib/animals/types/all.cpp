#include <string>
#include <vector>
#include "all.h"
#include "../include_all_animals.h"
using namespace std;

//All_Animal
All_Animal::~All_Animal() {}
vector<int> All_Animal::get_random_success_rate() {return {false, 0};}
int All_Animal::get_reward() {return 0;}
string All_Animal::get_name() {return "Unknown";}

//Derived classes
int Bear_Stats::get_reward() {return reward;}
string Bear_Stats::get_name() {return name;}
vector<int> Bear_Stats::get_random_success_rate() {
    srand(time(0));
    int index = rand() % 5;
    return {index, collection_of_chances[index]};
}

int Bird_Stats::get_reward() {return reward;}
string Bird_Stats::get_name() {return name;}
vector<int> Bird_Stats::get_random_success_rate() {
    srand(time(0));
    int index = rand() % 5;
    return {index, collection_of_chances[index]};
}

int Deer_Stats::get_reward() {return reward;}
string Deer_Stats::get_name() {return name;}
vector<int> Deer_Stats::get_random_success_rate() {
    srand(time(0));
    int index = rand() % 5;
    return {index, collection_of_chances[index]};
}

int Fox_Stats::get_reward() {return reward;}
string Fox_Stats::get_name() {return name;}
vector<int> Fox_Stats::get_random_success_rate() {
    srand(time(0));
    int index = rand() % 5;
    return {index, collection_of_chances[index]};
}

int Sheep_Stats::get_reward() {return reward;}
string Sheep_Stats::get_name() {return name;}
vector<int> Sheep_Stats::get_random_success_rate() {
    srand(time(0));
    int index = rand() % 5;
    return {index, collection_of_chances[index]};
}