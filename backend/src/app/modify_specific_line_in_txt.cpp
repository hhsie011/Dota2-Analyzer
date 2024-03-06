#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#define AB_ID "5396"

using namespace std;

int main()
{
    fstream input;
    input.open("../../Temporary_Data/ability_upgrade_popularity_copy.txt", ios::in | ios::out | ios::binary);
    string line = "";
    bool flag = false;
    // cout << "Loaded" << endl;
    if(!input.is_open()) { exit(1); input.close(); }
    else
    {
        // cout << "File opened" << endl;
        while(getline(input, line))
        {
            // cout << "Running" << endl;
            stringstream ss(line);
            string data1,data2;
            //keep track of size of line
            int prevLineSize = line.length();
            //cout << "prev line size = " << prevLineSize << endl;
            //get current file position
            streampos oldPos = input.tellg(); 
            if(getline(ss, data1, ',') && getline(ss, data2))
            {
                // cout << "data1 = " << data1 << " & data2 = " << data2 << endl; //debug prints every single line before the AB_ID is seen
                if(data1 == AB_ID)
                {
                    cout << "data1 = " << data1 << " & data2 = " << data2 << endl; 
                    int temp = stoi(data2);
                    temp++;
                    data2 = to_string(temp);
                    cout << "data1 = " << data1 << " & data2 = " << data2 << endl; 
                    if(oldPos > input.beg || oldPos < input.beg) { oldPos -= prevLineSize + 1; }
                    input.seekp(oldPos);
                    stringstream tempSS;
                    tempSS << data1.c_str() << ',' << data2.c_str();
                    // input << data1.c_str() << ',' << data2.c_str();
                    input.write(tempSS.str().c_str(), tempSS.str().size());
                    flag = true;
                }
            }
            if(flag)break;
        }
        if(!flag) { cout << "Abilitiy ID not found.  Cannot make changes to stat file." << endl; }
    }
    input.close(); input.clear();
    return 0;
}