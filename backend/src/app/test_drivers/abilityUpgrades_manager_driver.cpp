#include "../submanager_ability_upgrades.h"
#include "../submanager_ability.h"
#include <iostream>
using namespace std;

#define LIMIT 5 //change this to adjust the limit

int main()
{
    //g++ test_drivers/abilityUpgrades_manager_driver.cpp submanager_ability_upgrades.cpp -o test
    //./test

    //test size
    ability_upgrades tmp = ability_upgrades();
    vector<pair<int,int>> test = tmp.get_upgrades(LIMIT, "../../Temporary_Data/ability_upgrade_popularity.txt", 1);
    cout << "Top " << test.size() << " Abilities:" << endl;
    for(int i = 0; i < test.size(); i++) 
    {
        cout << test.at(i).first << "," << test.at(i).second << endl;
    } 

    //test upgrade
    // string check = "";
    // string ability_id = "10002";
    // {
    //     ability ob1 = ability();
    //     ob1.readFile(ob1.fileName);
    //     check = ob1.get_ability_name(stoi(ability_id));
    //     //cout << check << endl;
    // }
    // if(check == "NOT FOUND")
    // {
    //     cout << "Ability ID not valid!" << endl;
    // }
    // else
    // {
    //     ability_upgrades ob2 = ability_upgrades();
    //     ob2.readFile(ob2.fileName, true);
    //     //cout << "1" << endl; //debug
    //     ob2.addUpgrade(ability_id, "-1", "-1", "-1", "-1");
    //     //cout << "2" << endl; //debug
    //     ob2.writeFile(0);
    // }
}