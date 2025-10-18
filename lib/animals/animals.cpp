//Includes
#include <vector>
#include <string>
#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <ctime>
#include "types/all.h"
#include "include_all_animals.h"
#include "animals.h"
using namespace std;

void Animal::select_animal() {
    srand(time(0));
    int percentage = rand() % 101;
    int spawned_animal_index = 0;
    for(int i = 0; i < 5; i++) {
        if(percentage >= spawning_chances_sum[i] && percentage <= spawning_chances_sum[i+1]) {
            spawned_animal_index = i;
            break;
        }
    }

    if(spawned_animal_index == 0) {hunt_animal = new Bear_Stats();}
        else if(spawned_animal_index == 1) {hunt_animal = new Bird_Stats();}
        else if(spawned_animal_index == 2) {hunt_animal = new Deer_Stats();}
        else if(spawned_animal_index == 3) {hunt_animal = new Fox_Stats();}
        else {hunt_animal = new Sheep_Stats();}
    }

void Animal::success_hunt_output(string animal, bool success) {
    cout << endl << "You took the shot and..." << endl;
    sleep(3);

    if(success) {
        cout << "You took down the " << animal << "!" << endl;
        cout << "It was a success." << endl;
    } else {
        cout << "You missed the shot and the " << animal << " ran away." << endl;
        cout << "Better luck next time." << endl;
    }
}

int Animal::start_hunting(int weapon_chance) {
    srand(time(0));
    select_animal();
    string inputter;

    vector<int> activity_based_chance = hunt_animal->get_random_success_rate();
    cout << "You found a " << hunt_animal->get_name();
    cout << " " + activity[activity_based_chance[0]] << "." << endl;

    // Chance = 25

    int generate_chance = rand() % 101;
    float overall_rate = generate_chance * (100 - weapon_chance)/100;

    bool success = false;
    if(overall_rate <= activity_based_chance[1]) {
        success = true;
    }

    cout << endl << "(expected rate = " << activity_based_chance[1] << ")" << endl;
    cout << "(the rate may change depending on the RNG and your weapon)" << endl;
    cout << endl << "Do you wish to continue hunting? [Y/n] ";

    cin >> inputter;
    int reward = 0;
    if((char)tolower(inputter[0]) == 'y') {
        success_hunt_output(hunt_animal->get_name(), success);
        if(success) {
            reward = hunt_animal->get_reward();
            cout << "(You got " << reward << " balance)" << endl;
        }
    } else {
        cout << endl << "You decided that it wasn't the right time to hunt ";
        cout << "down the " << hunt_animal->get_name() + "." << endl;
    }

    delete hunt_animal;
    return reward;
}