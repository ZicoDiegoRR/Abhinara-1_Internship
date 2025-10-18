#ifndef PLAY_HUNT
#define PLAY_HUNT

#include <string>
#include <iostream>
#include <unistd.h>
#include "player/stats.h"
#include "animals/animals.h"
#include "weapons/all_rifle.h"
using namespace std;

inline void play() {
    int select_option, hunt_reward;

    cout << "<>=================| HUNTING GAMES |=================<>" << endl;
    cout << "Author: Zico Diego Rio Ramadhonny (5054251023)" << endl << endl;

    Stats stats;
    Animal animal;
    bool game = true;
    while(game) {
        cout << "Select one of the following options by the number:" << endl;
        cout << "1. Hunt an animal in the forest." << endl;
        cout << "2. Enter the shop." << endl;
        cout << "3. Check player info." << endl;
        cout << "4. Exit the game." << endl;
        cout << endl << "Me: Hmm, I'm picking... ";
        cin >> select_option;

        if(cin.fail()) {
            cin.clear();
            cin.ignore();
        } else {
            switch(select_option) {
                case 1:
                    hunt_reward = animal.start_hunting(stats.get_weapon_rate());
                    stats.add_money(hunt_reward);
                    break;
                case 2:
                    stats.access_shop();
                    break;
                case 3:
                    stats.get_info();
                    break;
                case 4:
                    game = false;
                    cout << "Exiting...";
                    break;
                default:
                    cout << "Unknown input. Let's try again..." << endl;
                    sleep(2);
                    break;
            }
        }
    }
}

#endif