#include "submanager_ability.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <sys/types.h>
#include <sys/stat.h>
#include <ctime>
#include <iomanip>

using namespace std;

void ability::readFile(string file) 
{
    ifstream infile;
    infile.open(file);
    string fileTemp;
    if (infile.is_open()) {
        ability_data[0].reserve(800);
        ability_data[1].reserve(800);
        getline(infile,fileTemp);
        while (getline(infile, fileTemp)) {
            stringstream ss(fileTemp);
            string value;
            for (unsigned i = 0; i < ABILITY_COLS; i++) { if (getline(ss,value,',')) { ability_data[i].push_back(value);/*   cout << value << endl;  */} }
        }
    }
    infile.close();
}

int ability::get_size()
{
    cout << "HERE in ability get_size()" << endl;
    int result = -1;
    time_t txtMTime = 0;
    struct stat fileInfoText;
    int err = stat("./stat_dump/num_of_abilities.txt", &fileInfoText);
    if (err != 0) {
        perror(" ./stat_dump/num_of_abilities.txt");
    }
    else { txtMTime = fileInfoText.st_mtime; }
    struct stat fileInfoCsv;
    err = stat(fileName.c_str(), &fileInfoCsv);
    if (err != 0) {
        perror("../../Data/ability_ids.csv");
        exit(errno);
    }
    time_t CSVMTime = fileInfoCsv.st_mtime;
    if (CSVMTime < txtMTime ){
        ifstream file;
        file.open("./stat_dump/num_of_abilities.txt",  ifstream::in);
        if (!file.is_open()) { file.close(); cout <<"Something went wrong.  The file was corrupt." << endl; return -1; }
        file >> result;
        file.close();
    }
    else
    {
        readFile(fileName);
        ofstream file;
        file.open("./stat_dump/num_of_abilities.txt");
        if (!file.is_open()) { file.close(); cout <<"Something went wrong.  The file was corrupt." << endl; return -1; }
        result = ability_data[0].size();
        file << result << endl;
        file.close();
    }
    return result;
}

void ability::writeFile(bool flag, char mode) 
{
    int colCount = 0;
    ofstream outfile;

    //check if we need to write
    if (!flag && !requiresWrite) {
        return;
    }
    requiresWrite = false;

    if(flag && mode == 'a') { outfile.open(fileName, std::ios_base::app); }
    else if(!flag && mode == 'a') { outfile.open(fileName2, std::ios_base::app); }
    else if(flag && mode == 'o') { ( outfile.open(fileName)); }
    else if(!flag && mode == 'o') { ( outfile.open(fileName2)); }
    string fileTemp;
    stringstream output;
    if (outfile.is_open()) {
        {
            if(mode == 'o')
            {
                output << "ability_id,ability_name" << endl;
                for (unsigned row = 0; row < ability_data[0].size(); row++) 
                {
                    if (!(ability_data[0].at(row) == "DELETED")) {
                        for (unsigned col = 0; col < ABILITY_COLS; col++) {
                            output << ability_data[col].at(row) ;
                            if (col != ABILITY_COLS - 1) { output << ','; }
                        }
                        output << "\n";
                    }
                }
            }
            else if (mode == 'a')
            {
                int row = ability_data[0].size()-1;
                if (!(ability_data[0].at(row) == "DELETED")) {
                    for (unsigned col = 0; col < ABILITY_COLS; col++) 
                    {
                        output << ability_data[col].at(row) ;
                        if (col != ABILITY_COLS - 1) { output << ','; }
                    }
                    output << "\n";
                }
            } else {
                cout << "ERROR! SELECTED MODE IS INVALID!" << endl;
                return;
            }
        }
        outfile << output.rdbuf();
        output.str("");
    }
    outfile << output.rdbuf();
    outfile.close(); outfile.clear();
    output.str("");
}

string ability::get_ability_name(int ab_id)
{
    string result = "NOT FOUND";
    for ( unsigned i = 0; i < ability_data[0].size(); i++ )
    {
        if (stoi(ability_data[0].at(i)) == ab_id)
        {
            return ability_data[1].at(i);
        }
    }
    return result;
}

bool ability::checkInput(string ability_id) {
    for (unsigned i = 0; i < ability_id.size(); i++) {
        if (ability_id[0] == '-' && i == 0) {
            if (ability_id.size() < 1) {
                return false;
            }
            i++;
        }
        if (!isdigit(ability_id[i])) {
            return false;
        }
    }
    return true;
}

bool ability::insert_ability(int ab_id, string ab_name)
{
    string check = "";
    if(!checkInput(to_string(ab_id))){ return false; }
    requiresWrite = true;
    check = get_ability_name(ab_id);
    if(check == "NOT FOUND")
    {
        if(ab_name == "-1") { ab_name = "N/A"; }
        ability_data[0].push_back(to_string(ab_id));
        ability_data[1].push_back(ab_name);
    }
    else { return false; }
    return true;
}

void modspecline_helper(string filename, string ab_id, string ab_name) {
    fstream input;
    input.open(filename, ios::in | ios::out | ios::binary);
    string line = "";
    bool flag = false;
    if(!input.is_open()) { exit(1); input.close(); }
    else
    {
        while(getline(input, line))
        {
            stringstream ss(line);
            string data1,data2, data3;
            //keep track of size of line
            int prevLineSize = line.length();
            //get current file position
            streampos oldPos = input.tellg(); 
            if(getline(ss, data1, ',') && getline(ss, data2))
            {
                cout << "DATA1: " << data1 << endl;
                if(data1 == ab_id)
                {
                    cout << "ab_id = " << data1 << " , ab_name = " << data2 << endl; 
                    data1 = ab_id;
                    data2 = ab_name;
                    if(oldPos > input.beg || oldPos < input.beg) { oldPos -= prevLineSize + 1; }
                    input.seekp(oldPos);
                    stringstream tempSS;
                    tempSS << data1.c_str() << ',' << data2.c_str() << ',' << data3.c_str();
                    input.write(tempSS.str().c_str(), tempSS.str().size());
                    flag = true;
                }
            }
            if(flag)break;
        }
        if(!flag) { cout << "Ability ID not found.  Cannot make changes to stat file." << endl; }
    }
    input.close(); input.clear();
}

bool ability::update_ability(int ab_id, string ab_name)
{
    string check = "";
    int first = 0, middle, last = ability_data[0].size() - 1;
    if(!checkInput(to_string(ab_id))){ return false; }
    check = get_ability_name(ab_id);
    bool found = false;
    if(check != "NOT FOUND")
    {
        if(ab_name == "-1") { ab_name = "N/A"; }
        for (unsigned i = 0; i < ability_data[0].size(); i++)
        {
            if (ability_data[0].at(i) == to_string(ab_id))
            {
                ability_data[0].at(i) = to_string(ab_id);
                ability_data[1].at(i) = ab_name;
            }
        }
    }
    else { return false; }
    //modspecline_helper(fileName2, to_string(ab_id), ab_name);
    return true;
}

bool ability::delete_ability(int ab_id)
{
    string check = "";
    if(!checkInput(to_string(ab_id))){ return false; }
    requiresWrite = true;
    for (unsigned i = 0; i < ability_data[0].size(); i++)
    {
        if (ability_data[0].at(i) == to_string(ab_id))
        {
            ability_data[0].at(i) = "DELETED";
            return true;
        }
    }
    cout << "ERROR! ABILITY NOT FOUND!" << endl; //error log
    return false;
}