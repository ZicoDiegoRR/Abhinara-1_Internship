#include <string>
#include "all_rifle.h"
using namespace std;

class YOLO: public All_Rifles {
    private:
        string name = "\"You Only Launch One\"";
        string description = "Don't worry about missing a shot again.";
        int price = 650, rate_booster = 100;

    public:
        string get_name() override;
        string get_desc() override;
        int get_price() override;
        int get_rate() override;
};