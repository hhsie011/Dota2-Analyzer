#include "submanager_playerRatings.h"

using namespace std;

int main(){
    Submanager_Player_Ratings s;
    s.readFile("../../Data/player_ratings.csv");
    s.getWinRates();
    cout << s.getWinRateById("236579") << endl;
}