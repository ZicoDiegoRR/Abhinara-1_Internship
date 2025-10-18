#include <string>
#include "all_rifle.h"
using namespace std;

class Bolt_Rifle: public All_Rifles {
    private:
        string name = "Bolt Action Rifle";
        string description = "A rifle with manual bullet loading and precise aim";
        int price = 130, rate_booster = 38;

    public:
        string get_name() override;
        string get_desc() override;
        int get_price() override;
        int get_rate() override;
};