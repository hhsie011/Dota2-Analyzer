#include "submanager_heroNames.h"

using namespace std;

void Submanager_Hero_Names::readFile(string text){
    ifstream infile;
    infile.open(text);
    string fileTemp;
    if (infile.is_open()) {
        getline(infile,fileTemp);
        while (getline(infile, fileTemp)) {
            stringstream ss(fileTemp);
            string value;
            for (unsigned i = 0; i < HNCOLS; i++) {
                if (getline(ss,value,',')) {
                    hname_data[i].push_back(value);
                }
            }
        }
    }
    infile.close();
}

string Submanager_Hero_Names::idByHeroName(string heroName) {
    for (unsigned i = 0; i < hname_data[2].size(); i++) {
        if (hname_data[2].at(i) == heroName) {
            return hname_data[1].at(i);
        }
    }
    return "";
}

void Submanager_Hero_Names::writeFile(bool flag, char mode) {
    int colCount = 0;
    ofstream outfile;

    //check if write is required
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
    if (outfile.is_open()) 
    {
        if(mode == 'o')
        {
            output << "name,hero_id,localized_name" << endl;
            for (unsigned row = 0; row < hname_data[1].size(); row++) 
            {
                if (!(hname_data[1].at(row) == "DELETED")) {
                    for (unsigned col = 0; col < HNCOLS; col++) {
                        output << hname_data[col].at(row) ;
                        if (col != HNCOLS - 1) { output << ','; }
                    }
                    output << "\n";
                }
            }
        }
        else if (mode == 'a')
        {
            int row = hname_data[0].size()-1;
            if (!(hname_data[0].at(row) == "DELETED")) {
                for (unsigned col = 0; col < HNCOLS; col++) 
                {
                    output << hname_data[col].at(row) ;
                    if (col != HNCOLS - 1) { output << ','; }
                }
                output << "\n";
            }
        } 
        else 
        {
            cout << "ERROR! SELECTED MODE IS INVALID!!" << endl;
            return;
        }
    }
    outfile << output.rdbuf();
    output.str(""); 
    outfile.close(); outfile.clear();
}

vector<string> Submanager_Hero_Names::getHnames(int limit){
    vector<string> temp;
    for (unsigned row = 0; row < hname_data[2].size() && row < limit; row++) {
        temp.push_back(hname_data[2].at(row));
    }
    return temp;
}

vector<string> Submanager_Hero_Names::getHids(int limit){
    vector<string> temp;
    for (unsigned row = 0; row < limit && row < hname_data[1].size(); row++) {
        temp.push_back(hname_data[1].at(row));
    }
    return temp;
}

bool Submanager_Hero_Names::checkInput(string hero_name, string hero_id, string localized_name) {
    for (unsigned i = 0; i < hero_id.size(); ++i) {
        if (!isdigit(hero_id[i])) {
            cout << "ERROR! NOT A DIGIT!" << endl;
            return false;
        }
    }
    if (hero_name.rfind("npc_dota_hero_", 0) == std::string::npos || hero_name.size() <= 14) {  // hero name must start with "npc_dota_hero_"
        cout << "ERROR! NOT FORMATTED CORRECTLY!" << endl;
        return false;
    }
    for (unsigned i = 14; i < hero_name.size(); ++i) {
        if (!isalpha(hero_name[i]) && hero_name[i] != '_') {
            cout << "ERROR! NOT AN ALPHABETICAL LETTER OR UNDERSCORE!" << endl;
            return false;
        }
    }
    for (unsigned i = 0; i < localized_name.size(); ++i) {
        if (!isalpha(localized_name[i]) && localized_name[i] != '_') {
            cout << "ERROR! NOT AN ALPHABETICAL LETTER OR UNDERSCORE!" << endl;
            return false;
        }
    }
    if (stoi(hero_id) < 0) {
        cout << "HeroID MUST EXISTS!" << endl;
        return false;
    } 
    return true;
}

//TODO: FIX, BC HERO_NAME IS NOT UNIQUE
bool Submanager_Hero_Names::insertHero(string hero_name, string hero_id, string localized_name) {
    if (!checkInput(hero_name, hero_id, localized_name)) {
        return false;
    }
    for (unsigned i = 0; i < hname_data[0].size(); i++) {
        if (hname_data[1].at(i) == hero_id) {
            return false;
        }
    }
    requiresWrite = true;
    for (unsigned i = 0; i < HNCOLS; ++i) {
        hname_data[i].push_back("0");
    }
    long int numHeroes = hname_data[0].size() - 1;

    if (hero_name == "-1") { hero_name = to_string(numHeroes); }
    hname_data[0].at(hname_data[0].size()-1) = hero_name;

    hname_data[1].at(hname_data[0].size()-1) = hero_id;

    if (localized_name == "-1") { localized_name = "N/A"; }
    hname_data[2].at(hname_data[0].size()-1) = localized_name;

    return true;
}

void modspecline_helper(string filename, string hero_name, string hero_id, string localized_name) {
    fstream input;
    input.open(filename, ios::in | ios::out | ios::binary);
    string line = "";
    bool flag = false;
    // cout << "Loaded" << endl;
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
            if(getline(ss, data1, ',') && getline(ss, data2, ',') && getline(ss, data3))
            {
                if(data2 == hero_id)
                {
                    cout << "hero_name = " << data1 << " , hero_id = " << data2 << " , localized_name = " << data3 << endl; 
                    data1 = hero_name;
                    data2 = hero_id;
                    data3 = localized_name;
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
        if(!flag) { cout << "Hero ID not found.  Cannot make changes to stat file." << endl; }
    }
    input.close(); input.clear();
}

bool Submanager_Hero_Names::updateHero(string hero_name, string hero_id, string localized_name) {
    if (!checkInput(hero_name, hero_id, localized_name)) {
        return false;
    }
    for (unsigned i = 0; i < hname_data[0].size(); ++i) {
        if (hname_data[1].at(i) == hero_id) {
            hname_data[0].at(i) = hero_name;
            hname_data[2].at(i) = localized_name;
        }
    }
    //modspecline_helper(fileName2, hero_name, hero_id, localized_name);
    return true;
}

bool Submanager_Hero_Names::deleteHero(string hero_id) {
    for (unsigned i = 0; i < hero_id.size(); ++i) {
        if (!isdigit(hero_id[i])) {
            cout << "ERROR! NOT A DIGIT!" << endl;
            return false;
        }
    }
    for (unsigned i = 0; i < hname_data[0].size(); ++i) {
        if (hname_data[1].at(i) == hero_id) {
            hname_data[1].at(i) = "DELETED";
            requiresWrite = true;
            return true;
        }
    }
    cout << "ERROR! NOT FOUND!" << endl;
    return false;
}

int Submanager_Hero_Names::getIDSize() {
    return hname_data[1].size();
}