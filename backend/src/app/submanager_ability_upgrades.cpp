#include "submanager_ability_upgrades.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <sys/types.h>
#include <sys/stat.h>
#include <ctime>
#include <iomanip>
#include <algorithm>
#include <bits/stdc++.h>

using namespace std;

void ability_upgrades::readFile(string file) 
{
        ifstream infile;
        infile.open(file);
        string fileTemp;
        if (infile.is_open()) {
            getline(infile,fileTemp);
            while (getline(infile, fileTemp)) {
                stringstream ss(fileTemp);
                string value;
                for (unsigned i = 0; i < abu_COLS; i++) {
                    if (getline(ss,value,',')) {
                    ability_data[i].push_back(value);
                    }
                }
            }
        }
        // cout << "total entries in memory for Player Rating: " << players_data[0].capacity() <<endl;
        infile.close();
}

/*void ability_upgrades::readFile(string file, bool flag)  //this wasn't working so I commented it out
{
    int N = 8949599;  
    int i = 0;
    ability_data[0].reserve(N); //memory fragmentation mitagation
    ability_data[1].reserve(N); //memory fragmentation mitagation
    ability_data[2].reserve(N); //memory fragmentation mitagation
    ability_data[3].reserve(N); //memory fragmentation mitagation
    ability_data[4].reserve(N); //memory fragmentation mitagation
    ability_data[5].reserve(N); //memory fragmentation mitagation    
    ifstream infile;
    infile.open(file);
    string fileTemp;
    if (infile.is_open()) {
        getline(infile,fileTemp);
        while (getline(infile, fileTemp)) {
            stringstream ss(fileTemp);
            string value;
            for (unsigned i = 0; i < 6; i++) { 
                if (getline(ss,value,',')) 
                { 
                    if(i == 0) { ability_data[i].push_back(value);}
                    if(i == 1) { ability_data[i].push_back(value);}
                    if(i == 2) { ability_data[i].push_back(value);}
                    if(i == 3) { ability_data[i].push_back(value);}
                    if(i == 4) { ability_data[i].push_back(value);}
                    if(i == 5) { ability_data[i].push_back(value);}
                }
            }
            //i++;
            //if(i == 14) break;
        }
    }
    infile.close();
    cout << "max capacity = " << ability_data[0].capacity() << endl;
}*/

// helper function to sort the elements by
// second of pair in descending order
bool sortbydesc(const pair<int,int> &a, const pair<int,int> &b)
{
    return a.second>b.second;
}

vector<pair<int,int>> ability_upgrades::get_upgrades(int limit, string filename, bool modified)
{
    time_t start, end;
    double p1, p2, p3, p4, p5, p6; // for optimization
    vector< pair<int,int>> result; // vector to return
    time_t txtMTime = 0;
    struct stat fileInfoTxt; //struct to access stats txt modified time
    int err = stat(filename.c_str(), &fileInfoTxt);
    if (err != 0) { 
        perror("ability_upgrade_popularity.txt"); 
        cout << "Processing Data" << endl;
    }
    else { txtMTime = fileInfoTxt.st_mtime; }
    struct stat fileInfoCsv; //struct to access cvs mod time
    if(modified) { err = stat(fileName2.c_str(), &fileInfoCsv); }
    else { err = stat(fileName.c_str(), &fileInfoCsv); }
    if (err != 0) {
        perror(fileName.c_str());
        exit(errno);
    }
    time_t CSVMTime = fileInfoCsv.st_mtime;
    if(CSVMTime > txtMTime){ //if CSV is newer than txt
        if(modified) { readFile(fileName2); }
        else { readFile(fileName); }
        vector<pair<int,int>> process; //create a dummy variable
        result.reserve(limit); //memory fragmentation mitagation
        process.reserve(600); //memory fragmentation mitagation

        time(&start);
        vector<string> temp = ability_data[0]; //duplicate data member for operation on data
        time(&end);
        p1 = double(end - start);
        time(&start);
        sort(temp.begin(),temp.end()); //sort duplicate
        vector<string> copy = temp;
        time(&end);
        p2 = double(end - start);
        time(&start);
        auto iter = unique(temp.begin(), temp.end()); //keep all unique data
        time(&end);
        p3 = double(end - start);
        time(&start);
        temp.erase(iter, temp.end()); //erase the rest of the data
        time(&end);
        p4 = double(end - start);
        short cnt = 0; //counter to limit the number of entries displayed
        time(&start);
        for(int i = 0; i < temp.size(); i++)
        {
            auto start = find(copy.begin(), copy.end(), temp.at(i));
            int tmp = count(start, copy.end(), temp.at(i)); //count number of occurences for each entry in the original data 
            process.push_back(make_pair(stoi(temp.at(i)),tmp)); //push a pair of the (ability number, number of times upgraded)
            cnt++; //increment limit counter
        }
        time(&end);
        p5 = double(end - start);
        time(&start);
        sort(process.begin(), process.end(), sortbydesc); //prepare dummy data 
        time(&end);
        p6 = double(end - start);
        for(int i = 0; i < limit; i++)
        {
            result.push_back(process.at(i));
        }
        sort(result.begin(), result.end(), sortbydesc); //prepare display data
        ofstream file;
        file.open(filename);
        if (!file.is_open()) { file.close(); cout <<"Something went wrong.  The file was corrupt." << endl; return result; }
        for(int i = 0; i < process.size(); i++)
        {
            file << process[i].first << "," << process[i].second << endl; //dump all data in dummy to txt file
        } 
        file.close();
    }
    else 
    {
        if (CSVMTime < txtMTime ){ //if txt file is newer than csv file
            result.reserve(limit); //memory fragmentation mitigation
            ifstream file;
            string fileTemp;
            file.open(filename,  ifstream::in);
            if (!file.is_open()) { file.close(); cout <<"Something went wrong.  The file was corrupt." << endl; return result; }
            int cnt = 0;
            while (getline(file, fileTemp)) {
                if (cnt == limit) { break; }
                stringstream ss(fileTemp);
                string value;
                for (unsigned i = 0; i < 2; i++) { 
                    if (getline(ss,value,',')) 
                    { 
                        int first, second;
                        if(i == 0) { first = stoi(value);}
                        else if(i == 1) 
                        { //read each line of the txt file
                            second = stoi(value); 
                            result.push_back(make_pair(first,second)); //create pair based off the line and push into result
                            cnt++;
                        }
                    }
                }
            }
            file.close();
        }
    }
    return result;
}

void ability_upgrades::writeFile(bool permanent) 
{
    //if update is the only thing called, update will write to file already, so no need to call writeFile
    if (!requiresWrite && !permanent) {
        return;
    }
    requiresWrite = false;

    ofstream outfile;
    if(!permanent) { outfile.open(fileName2); }
    else { outfile.open(fileName); }
    string fileTemp;
    stringstream output;
    int colCount = 0;
    if (outfile.is_open()) 
    {
        for (unsigned row = 0; row < ability_data[0].size(); row++) {
            for (unsigned col = 0; col < abu_COLS - 1; col++) {
                output << ability_data[col].at(row) << ',';
            }
        output << ability_data[abu_COLS-1].at(row) << "\n";
        }
    }
    outfile << output.rdbuf();
    outfile.close(); outfile.clear();
    output.str("");
}   

bool ability_upgrades::checkInput(string ability_id, string level, string time, string player_slot, string match_id) {
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
    for (unsigned i = 0; i < level.size(); i++) {
        if (level[0] == '-' && i == 0) {
            if (level.size() < 2) {
                return false;
            }
            i++;
        }
        if (!isdigit(level[i])) {
            return false;
        }
    }
    for (unsigned i = 0; i < time.size(); i++) {
        if (time[0] == '-' && i == 0) {
            if (time.size() < 2) {
                return false;
            }
            i++;
        }
        if (!isdigit(time[i])) {
            return false;
        }
    }
    for (unsigned i = 0; i < player_slot.size(); i++) {
        if (player_slot[0] == '-' && i == 0) {
            if (player_slot.size() < 2) {
                return false;
            }
            i++;
        }
        if (!isdigit(player_slot[i])) {
            return false;
        }
    }
    for (unsigned i = 0; i < match_id.size(); i++) {
        if (match_id[0] == '-' && i == 0) {
            if (match_id.size() < 2) {
                return false;
            }
            i++;
        }
        if (!isdigit(match_id[i])) {
            return false;
        }
    }
    return true;
}

bool ability_upgrades::addUpgrade(string ability_id, string level, string time, string player_slot, string match_id) {
    if (!checkInput(ability_id, level, time, player_slot, match_id)) {
        cout << "INVALID ENTRY: TRY AGAIN!" << endl;
        return false;
    }
    for (unsigned i = 0; i < abu_COLS; i++) {
        ability_data[i].push_back("0");
    }
    long int tmp = ability_data[0].size() - 1;

    //ability id
    //handles case where "-1" is passed in 
    if(ability_id == "-1") { ability_id = to_string(tmp++);}
    ability_data[0].at(ability_data[0].size()-1) = ability_id;    

    //level
    //handles case where "-1" is passed in 
    if(level == "-1") { level = "N/A"; }
    ability_data[1].at(ability_data[0].size()-1) = level;

    //time
    //handles case where "-1" is passed in    
    if(time == "-1") { time = "N/A"; }
    ability_data[2].at(ability_data[0].size()-1) = time;  

    //player_slot
    //handles case where "-1" is passed in   
    if(player_slot == "-1") { player_slot = "N/A"; }
    ability_data[3].at(ability_data[0].size()-1) = player_slot;

    //match_id
    //handles case where "-1" is passed in    
    if(match_id == "-1") { match_id = "N/A"; }
    ability_data[4].at(ability_data[0].size()-1) = match_id;  

    //Writes directly to end of file
    ofstream oFS(fileName2, ios::app);
    oFS << "\n" << ability_id << ',' << level << "," << time << "," << player_slot << "," << match_id;
    oFS.close(); oFS.clear();
    return true;
}

void specific_line_modify_helper(string filename, string ability_id, string level, string time, string player_slot, string match_id) {
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
            string data1, data2, data3, data4, data5;
            //keep track of size of line
            int prevLineSize = line.length();
            //get current file position
            streampos oldPos = input.tellg(); 
            if(getline(ss, data1, ',') && getline(ss, data2, ',') && getline(ss, data3, ',') && getline(ss, data4, ',') && getline(ss, data5))
            {
                if(data1 == ability_id)
                {
                    cout << "ability_id = " << data1 << " , level = " << data2 << " , time = " << data3 << " , player_slot = " << data4 << 
                        " , match_id = " << data5 << endl; 
                    data2 = level;
                    data3 = time;
                    data4 = player_slot;
                    data5 = match_id;
                    if(oldPos > input.beg || oldPos < input.beg) { oldPos -= prevLineSize + 1; }
                    input.seekp(oldPos);
                    stringstream tempSS;
                    tempSS << data1.c_str() << ',' << data2.c_str() << ',' << data3.c_str() << ',' << data4.c_str() << ',' << data5.c_str();
                    input.write(tempSS.str().c_str(), tempSS.str().size());
                    flag = true;
                }
            }
            if(flag)break;
        }
        if(!flag) { cout << "Abilitiy ID not found.  Cannot make changes to stat file." << endl; }
    }
    input.close(); input.clear();
}

void ability_upgrades::updateUpgrade(string ability_id, string level, string time, string player_slot, string match_id) {
    for (unsigned i = 0; i < ability_data[0].size(); i++) {
        if (!checkInput(ability_id, level, time, player_slot, match_id)) {
            cout << "INVALID ENTRY: TRY AGAIN!" << endl;
            return;
        }
        if (ability_data[0].at(i) == ability_id) {
            ability_data[1].at(i) = level;
            ability_data[2].at(i) = time;
            ability_data[3].at(i) = player_slot;
            ability_data[4].at(i) = match_id;
            return;
        }
    }
    specific_line_modify_helper(fileName2, ability_id, level, time, player_slot, match_id); 
}

void ability_upgrades::deleteUpgrade(string ability_id) {
    for (unsigned i = 0; i < ability_id.size(); i++) {
        if (!isdigit(ability_id[i])) {
            cout << "INVALID ID" << endl;
            return;
        }
    }
    for (unsigned i = 0; i < ability_data[0].size(); i++) {
        if (ability_data[0].at(i) == ability_id) {
            for (unsigned j = 0; j < abu_COLS; j++) {
                ability_data[j].erase(ability_data[j].begin()+i);
            }
            return;
        }
    }
    requiresWrite = true;
}

void ability_upgrades::printData() {
    for (unsigned i = 0; i < ability_data[0].size(); i++) {
        for (unsigned j = 0; j < abu_COLS; j++) {
            cout << ability_data[j].at(i) << " ";
        }
    }
}