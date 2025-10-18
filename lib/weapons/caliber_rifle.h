#include <string>
#include "all_rifle.h"
using namespace std;

class Caliber_Rifle: public All_Rifles {
    private:
        string name = "Caliber Rifle";
        string description = "Capable of piercing through thin woods and having smooth trajectory.";
        int price = 290, rate_booster = 54;

    public:
        string get_name() override;
        string get_desc() override;
        int get_price() override;
        int get_rate() override;
};