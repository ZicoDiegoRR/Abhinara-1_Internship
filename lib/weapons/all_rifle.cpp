#include <string>
#include <iostream>
#include <unistd.h>
#include "all_rifle.h"
#include "include_all_rifle.h"
using namespace std;

// Define all functions
string Default_Rifle::get_name() {return name;}
string Default_Rifle::get_desc() {return description;}
int Default_Rifle::get_price() {return price;}
int Default_Rifle::get_rate() {return rate_booster;}

string Bolt_Rifle::get_name() {return name;}
string Bolt_Rifle::get_desc() {return description;}
int Bolt_Rifle::get_price() {return price;}
int Bolt_Rifle::get_rate() {return rate_booster;}

string Caliber_Rifle::get_name() {return name;}
string Caliber_Rifle::get_desc() {return description;}
int Caliber_Rifle::get_price() {return price;}
int Caliber_Rifle::get_rate() {return rate_booster;}

string Sem_Auto_Rifle::get_name() {return name;}
string Sem_Auto_Rifle::get_desc() {return description;}
int Sem_Auto_Rifle::get_price() {return price;}
int Sem_Auto_Rifle::get_rate() {return rate_booster;}

string YOLO::get_name() {return name;}
string YOLO::get_desc() {return description;}
int YOLO::get_price() {return price;}
int YOLO::get_rate() {return rate_booster;}

//All_Rifles
string All_Rifles::get_name() {return weapon->get_name();}
string All_Rifles::get_desc() {return weapon->get_desc();}
int All_Rifles::get_price() {return weapon->get_price();}
int All_Rifles::get_rate() {return weapon->get_rate();}
void All_Rifles::set_weapon_in_use(int type_index) {
    if(type_index == 0) {weapon = new Default_Rifle();}
    else if(type_index == 1) {weapon = new Bolt_Rifle();}
    else if(type_index == 2) {weapon = new Caliber_Rifle();}
    else if(type_index == 3) {weapon = new Sem_Auto_Rifle();}
    else if(type_index == 4) {weapon = new YOLO();}
}

void All_Rifles::display_weapon(bool shop) {
    if(!shop) {cout << "=========| WEAPON INFO |=========" << endl;}

    cout << "Name: " << weapon->get_name() << endl;
    cout << "Description: " << weapon->get_desc() << endl;
    cout << "Rate value: " << weapon->get_rate() << endl;
    if(shop) {cout << "Price: " << weapon->get_price();}
    cout << endl << endl;
}
int All_Rifles::shop(int money) {
    All_Rifles* temporary_pointer = weapon;
    int select_weapon;
    string inputter;

    int final_money = money;
    if(available.size() != 1) {
        cout << "=========| SHOP |=========" << endl;
        for(int i = 1; i < available.size(); i++) {
            set_weapon_in_use(available[i]);
            cout << i << ".";
            display_weapon(true);
        }

        cout << endl << endl << "Which one would you like to buy? ";
        cin >> select_weapon;
        if(!cin.fail() && select_weapon < available.size() && select_weapon > 0) {
            set_weapon_in_use(available[select_weapon]);
            cout << endl << "Are you sure you want to buy " << weapon->get_name();
            cout << " that costs " << weapon->get_price() << "? [Y/n]" << endl;
            cout << "(Your balance is " << money << ")" << endl;
            cin >> inputter;
            if((char)tolower(inputter[0]) == 'y') {
                if(money >= weapon->get_price()) {
                    int price = weapon -> get_price();
                    final_money -= price;
                    cout << "Successfully bought " << weapon->get_name() << "." << endl;
                    cout << "(Remaining balance = " << final_money << ")" << endl;
                    temporary_pointer = weapon; 
                    available.erase(available.begin() + select_weapon);
                } else {
                    cout << "Your balance isn't enough." << endl;
                    cout << "(Required = " << weapon->get_price() << ", but you only have " << money << ")";
                    cout << endl;
                }
            } else {
                cout << "Cancelling the order..." << endl;
            }
        } else {
            cin.clear();
            cin.ignore();
            cout << "Unknown input. Leaving the shop instead..." << endl;
        }
        weapon = temporary_pointer;
    } else {
        cout << "=========| SHOP |=========" << endl;
        cout << "Oops, it looks like the shop ran out of rifles." << endl;
        cout << "Exiting the shop..." << endl;
    }

    sleep(2);
    return final_money;
}