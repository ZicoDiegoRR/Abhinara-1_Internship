#include <string>
#include <iostream>
#include <unistd.h>
#include "stats.h"
using namespace std;

Stats::Stats(){
    cout << "Welcome, dear hunter." << endl;
    cout << "May I know your name? " << endl;
    cout << "Me: My name is... ";
    cin >> name;

    cout << endl << "Awesome! Welcome, " << name << ", to the forest!" << endl;
    sleep(2);

    cout << endl << "You can earn money by successfully hunting animals." << endl;
    cout << "Purchase better rifles at the shop for better catch." << endl;
    cout << "Good luck!" << endl;

    rifle.set_weapon_in_use(0);
    sleep(4);
}

int Stats::get_money() {return money;}
int Stats::get_weapon_rate() {return rifle.get_rate();}

void Stats::set_name(string input_name) {name = input_name;}
void Stats::set_money(int amount) {money = amount;}
void Stats::add_money(int amount) {money += amount;}
void Stats::access_shop() {money = rifle.shop(money);}
void Stats::get_info() {
    cout << "=========| PLAYER INFO |=========" << endl;
    cout << "Name : " << name << endl;
    cout << "Money: " << money << endl;
    rifle.display_weapon(false);
    sleep(4);
}