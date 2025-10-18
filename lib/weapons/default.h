#include <string>
#include "all_rifle.h"
using namespace std;

class Default_Rifle: public All_Rifles {
    private:
        string name = "Old, Rusty Rifle";
        string description = "Hunting rifle with signs of aging, but it still works despite the unreliability.";
        int price = 0, rate_booster = 0;

    public:
        string get_name() override;
        string get_desc() override;
        int get_price() override;
        int get_rate() override;
};