#include <string>
#include "all_rifle.h"
using namespace std;

class Sem_Auto_Rifle: public All_Rifles {
    private:
        string name = "Semi-automatic Rifle";
        string description = "Never shoots one, but multiple bullets at the same time.";
        int price = 400, rate_booster = 79;

    public:
        string get_name() override;
        string get_desc() override;
        int get_price() override;
        int get_rate() override;
};