#include "../submanager_ability.h"
#include <iostream>
using namespace std;

int main()
{
    // commands to execute: 
    // g++ test_drivers/ability_manager_driver.cpp submanager_ability.cpp -o test
    // ./test

    //readFile function test
/*     ability read = ability();
    read.readFile(read.fileName); */

    
    //size function test
    /* ability test = ability();
    int size =  test.get_size();
    if(size == -1) { cout << "Something went wrong in ability's get_size()." << endl; exit(1); }
    cout << "Number of abilities in DOTA 2 are " << size << endl;  */

    //match name test
    /* ability aname = ability();
    string name =  aname.get_ability_name(10002);
    if(name == ""){cout << "Something went wrong in ability's get_ability_name function." << endl; exit(1); }
    else if (name == "NOT FOUND" )
    {
        cout << "That ability id does not exist." << endl;
    }
    else
    {
        cout << "Name of ability is: " << name << endl;
    }*/

    ///update_ability test
    // ability test = ability();
    // test.readFile(test.fileName);
    // test.update_ability(5017, "asdf"); 
    // test.writeFile(0, 'o');
    
    ability test = ability();
    test.readFile(test.fileName);
    
    bool result = test.insert_ability(2, "test");
    if (result) {
        test.writeFile(0,'a');
    }
}