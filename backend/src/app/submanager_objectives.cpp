#include "submanager_objectives.h"

#include <fstream>
#include <sstream>
#include <iostream>
using namespace std;

void Submanager_Objective::readFile(string text) {
    ifstream infile;
    infile.open(text);
    string fileTemp;
    if (infile.is_open()) {
        getline(infile,fileTemp);
        while (getline(infile, fileTemp)) {
            stringstream ss(fileTemp);
            string value;
            for (unsigned i = 0; i < OBJ_COLS; i++) {
                if (getline(ss,value,',')) {
                obj_data[i].push_back(value);
                }
            }
        }
    }
    infile.close();
}

void Submanager_Objective::writeFile(string text) {
    int colCount = 0;
    ofstream outfile(text);
    string fileTemp;
    stringstream output;

    //check for write, even if this writefile is outdated
    if (!requiresWrite) {
        return;
    }
    requiresWrite = false;

    if (outfile.is_open()) {
        for (unsigned row = 0; row < obj_data[0].size(); row++) {
            for (unsigned col = 0; col < OBJ_COLS  - 1; col++) {
                output << obj_data[col].at(row) << ',';
            }
            output << obj_data[OBJ_COLS  - 1].at(row) << "\n";
        }
    }
    outfile << output.rdbuf();
    output.str("");
    outfile.close(); outfile.clear();
}

vector<pair<string,string>> Submanager_Objective::getFirstBlood(int limit){
    vector<pair<string,string>> temp;
    int cnt = 0;
    for(unsigned i = 0; i < obj_data[5].size();i++){
        if(cnt >= limit){break;}
        if(obj_data[5].at(i) == "CHAT_MESSAGE_FIRSTBLOOD"){
            temp.push_back(make_pair(obj_data[0].at(i),obj_data[4].at(i)));
            cnt++;
        }
    }
    return temp;
}

bool Submanager_Objective::checkInput(string match_id, string player) {
    for (unsigned i = 0; i < match_id.size(); i++) {
        if (!isdigit(match_id[i])) {
            return false;
        }
    }
    for (unsigned i = 0; i < player.size(); i++) {
        if (player[0] == '-' && i == 0) {
            if (player.size() < 2) {
                return false;
            }
            i++;
        }
        if (!isdigit(player[i])) {
            if (!(player[i] == '.')) {
                return false;
            }
        }
    }
    if (stod(player) < -1 || stod(player) > 9) {
        return false;
    }
    return true;
}

void Submanager_Objective::addFirstBlood(string match_id, string player) {
    if(!checkInput(match_id, player)) {
        cout << "Error: Invalid input!" << endl;
        return;
    }
    for (unsigned i = 0; i < obj_data[0].size(); i++) {
        if (obj_data[0].at(i) == match_id) {
            cout << "Error! Already Exists!" << endl;
            return;
        }
    }
    requiresWrite = true;
    for (unsigned i = 0; i < OBJ_COLS; i++) {
        obj_data[i].push_back("");
    }
    obj_data[0].at(obj_data[0].size()-1) = match_id;
    obj_data[4].at(obj_data[4].size()-1) = player;
    obj_data[5].at(obj_data[5].size()-1) = "CHAT_MESSAGE_FIRSTBLOOD";
    ofstream oFS(csvfile, ios::app);
    for (unsigned i = 0; i < OBJ_COLS; i++) {
        oFS << obj_data[i].at(obj_data[0].size()-2) << ",";
    }
    oFS << obj_data[OBJ_COLS-1].at(obj_data[0].size()-1);
    oFS.close(); oFS.clear();
}

void modspecline_helper_obj(string filename, string match_id, string player) {
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
            string data1,data2,data3,data4;
            string dump;
            int prevLineSize = line.length();
            streampos oldPos = input.tellg(); 
            if(getline(ss, data1, ',') && getline(ss, data2, ',') && getline(ss, data3, ',') && getline(ss, data4, ',') && getline(ss, dump))
            {
                if(data1 == match_id)
                {
                    cout << "match_id = " << data1 << " , player = " << data4 << endl; 
                    data1 = match_id;
                    data4 = player;
                    if(oldPos > input.beg || oldPos < input.beg) { oldPos -= prevLineSize + 1; }
                    input.seekp(oldPos);
                    stringstream tempSS;
                    tempSS << data1.c_str() << ',' << data2.c_str() << ',' << data3.c_str() << ',' << data4.c_str() << ',' << dump.c_str();
                    input.write(tempSS.str().c_str(), tempSS.str().size());
                    flag = true;
                }
            }
            if(flag)break;
        }
        if(!flag) { cout << "Hero ID not found.  Cannot make changes to stat file." << endl; }
    }
    input.close(); input.clear();
}

void Submanager_Objective::updateFirstBlood(string match_id, string player) {
    if(!checkInput(match_id, player)) {
        cout << "Error: Invalid input!" << endl;
        return;
    }
    for (unsigned i = 0; i < obj_data[0].size(); i++) {
        if (obj_data[0].at(i) == match_id) {
            obj_data[4].at(i) = player;
            //modspecline_helper_obj(csvfile2, match_id, player);
            return;
        }
    }
    cout << "Error: Not found!" << endl;
}

void Submanager_Objective::deleteFirstBlood(string match_id) {
    if(!checkInput(match_id, "")) {
        cout << "Error: Invalid input!" << endl;
        return;
    }
    for (unsigned i = 0; i < obj_data[0].size(); i++) {
        if (obj_data[0].at(i) == match_id) {
            for (unsigned j = 0; j < OBJ_COLS; j++) {
                obj_data[i].erase(obj_data[i].begin()+i);
            }
            requiresWrite = true;
            return;
        }
    }
    cout << "Error: Not found!" << endl;
}
