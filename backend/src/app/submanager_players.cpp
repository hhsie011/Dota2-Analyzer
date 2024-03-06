#include "submanager_players.h"

using namespace std;

void Submanager_Players::readFile(string text) {
        ifstream infile;
        infile.open(text);
        string fileTemp;
        if (infile.is_open()) {
            getline(infile,fileTemp);
            while (getline(infile, fileTemp)) {
                stringstream ss(fileTemp);
                string value;
                for (unsigned i = 0; i < PCOLS; i++) {
                    if (getline(ss,value,',')) {
                    players_data[i].push_back(value);
                    }
                }
            }
        }
        // cout << "total entries in memory for Player Rating: " << players_data[0].capacity() <<endl;
        infile.close();
    }

    void Submanager_Players::writeFile(bool flag, char mode) {
        int colCount = 0;
        ofstream outfile;
        if(flag && mode == 'a') { outfile.open(csvFile, std::ios_base::app); }
        else if(!flag && mode == 'a') { outfile.open(csvFile2, std::ios_base::app); }
        else if(flag && mode == 'o') { ( outfile.open(csvFile)); }
        else if(!flag && mode == 'o') { ( outfile.open(csvFile2)); }
        string fileTemp;
        if (outfile.is_open()) {
            {
                if(mode == 'o')
                {
                    outfile << "match_id,account_id,hero_id,player_slot,gold,gold_spent,gold_per_min,xp_per_min,kills,deaths,assists,denies,last_hits,stuns,hero_damage,hero_healing,tower_damage,item_0,item_1,item_2,item_3,item_4,item_5,level,leaver_status,xp_hero,xp_creep,xp_roshan,xp_other,gold_other,gold_death,gold_buyback,gold_abandon,gold_sell,gold_destroying_structure,gold_killing_heros,gold_killing_creeps,gold_killing_roshan,gold_killing_couriers,unit_order_none,unit_order_move_to_position,unit_order_move_to_target,unit_order_attack_move,unit_order_attack_target,unit_order_cast_position,unit_order_cast_target,unit_order_cast_target_tree,unit_order_cast_no_target,unit_order_cast_toggle,unit_order_hold_position,unit_order_train_ability,unit_order_drop_item,unit_order_give_item,unit_order_pickup_item,unit_order_pickup_rune,unit_order_purchase_item,unit_order_sell_item,unit_order_disassemble_item,unit_order_move_item,unit_order_cast_toggle_auto,unit_order_stop,unit_order_taunt,unit_order_buyback,unit_order_glyph,unit_order_eject_item_from_stash,unit_order_cast_rune,unit_order_ping_ability,unit_order_move_to_direction,unit_order_patrol,unit_order_vector_target_position,unit_order_radar,unit_order_set_item_combine_lock,unit_order_continue" << endl;
                    for (unsigned row = 0; row < players_data[0].size(); row++) 
                    {
                        if (!(players_data[0].at(row) == "DELETED")) {
                            for (unsigned col = 0; col < PCOLS; col++) {
                                outfile << players_data[col].at(row) ;
                                if (col != PCOLS - 1) { outfile << ','; }
                            }
                            outfile << "\n";
                        }

                    }
                }
                else if (mode == 'a')
                {
                    int row = players_data[0].size()-1;
                    if (!(players_data[0].at(row) == "DELETED")) {
                        for (unsigned col = 0; col < PCOLS; col++) 
                        {
                            outfile << players_data[col].at(row) ;
                            if (col != PCOLS - 1) { outfile << ','; }
                        }
                        outfile << "\n";
                    }
                } else {
                    cout << "ERROR! SELECTED MODE IS INVALID!" << endl;
                    return;
                }
            }
        }
        outfile.close(); outfile.clear();
    }

    vector<string> Submanager_Players::getHeroPickRate(int limit){
    vector<string> result; // vector to return
    time_t txtMTime = 0;
    struct stat fileInfoTxt; //struct to access txt modified time
    int err = stat("./stat_dump/hero_pr.txt", &fileInfoTxt);
    if (err != 0) { 
        perror("hero_pr.txt"); 
        cout << "Processing Data" << endl;
    }
    else { txtMTime = fileInfoTxt.st_mtime; }
    struct stat fileInfoCsv; //struct to access cvs mod time
    err = stat(csvFile.c_str(), &fileInfoCsv);
    if (err != 0) {
        perror("../../Data/players.csv");
        exit(errno);
    }
    time_t CSVMTime = fileInfoCsv.st_mtime;
    //cout << "CSV Modified = " << CSVMTime << endl; //debug
    //cout << "TXT Modified = " << txtMTime << endl; //debug
    if(CSVMTime > txtMTime){ //if CSV is newer than txt
        readFile(csvFile);
        short cnt = 0; //counter to limit the number of entries displayed
        multiset<string> temp;
        for (unsigned i = 0; i < players_data[2].size(); i++) {    
                    temp.insert(players_data[2].at(i));
        }

        ofstream file;
        file.open("./stat_dump/hero_pr.txt");
        if (!file.is_open()) { file.close(); cout <<"Something went wrong.  The file was corrupt." << endl; return result; }
        for(int i = 0; i <= MAXHERO; i++)
        {
            result.push_back(to_string(temp.count(to_string(i))));
            file << temp.count(to_string(i)) << endl; //dump all data in dummy to txt file
        }    
        file.close();
    }
    else 
    {
        if (CSVMTime < txtMTime ){ //if txt file is newer than csv file
            ifstream file;
            string fileTemp;
            file.open("./stat_dump/hero_pr.txt",  ifstream::in);
            if (!file.is_open()) { file.close(); cout <<"Something went wrong.  The file was corrupt." << endl; return result; }
            int cnt = 0;
            while (getline(file, fileTemp)) {
                if (cnt == limit) { break; }
                result.push_back(fileTemp);
                cnt++;
            }
            file.close();
        }
    }
    return result;
    }

    multiset<string> Submanager_Players::getPlayerGameCount(){ //inserts account ids into a multiset so you can use count() to check number of games played for each unique account
        multiset<string> temp;
        for (unsigned i = 0; i < players_data[1].size(); i++) {
                temp.insert(players_data[1].at(i));
        }
        temp.erase("0"); //remove all players who do not share public match data.
        return temp;
    }

    vector<pair<string,string>> Submanager_Players::getPlayerHero(){
        vector<pair<string,string>> temp;
        for(unsigned i = 0; i < players_data[1].size(); i++){
            if(players_data[1].at(i) != "0"){
                temp.push_back(make_pair(players_data[1].at(i),players_data[2].at(i)));
            }
        }
        sort(temp.begin(),temp.end());
        return temp;
    }
    vector<tuple<string,string,string>> Submanager_Players::getPlayerMatch(){
        vector<tuple<string,string,string>> temp;
        for(unsigned i = 0; i < players_data[1].size(); i++){
            if(players_data[1].at(i) != "0"){
                temp.push_back(make_tuple(players_data[1].at(i),players_data[0].at(i),players_data[3].at(i)));
                // cout << "total entries in memory for Duration Range: " << temp.capacity() << endl;
            }
        }
        sort(temp.begin(),temp.end());
        return temp;
    }
    
    vector<tuple<string,string,string>> Submanager_Players::getHeroMatch(){
        vector<tuple<string,string,string>> temp;
        for(unsigned i = 0; i < players_data[2].size(); i++){
                temp.push_back(make_tuple(players_data[2].at(i),players_data[0].at(i),players_data[3].at(i)));
                // cout << "total entries in memory for Duration Range: " << temp.capacity() << endl;
        }
        sort(temp.begin(),temp.end());
        return temp;
    } 

    vector<tuple<string,string,string>> Submanager_Players::getMatchHero(){
        vector<tuple<string,string,string>> temp;
        for(unsigned i = 0; i < players_data[0].size(); i++){
                //match_id, account_id, hero_id
                temp.push_back(make_tuple(players_data[0].at(i),players_data[2].at(i),players_data[3].at(i)));
                // cout << "total entries in memory for Duration Range: " << temp.capacity() << endl;
        }
        return temp;
    } 

// FEATURE: Finds general information about a how a player plays a hero
// This means how much of their gold they spend and how much gold and xp they gain per minute
vector<tuple<string,string, string,string,string>> Submanager_Players::getXPGoldGeneral(string limit){
    //account_id, hero_id, gold_spent, gold_per_min, xp_per_min
    vector<tuple<string, string, string, string, string>> v1;
    for (unsigned i = 0; i < stoi(limit); i++) {
        v1.push_back(make_tuple(players_data[1].at(i), players_data[2].at(i), players_data[5].at(i), players_data[6].at(i), players_data[7].at(i)));
    }
    return v1;
}

// FEATURE: Finds specific hero by hero information on xp and gold per minute
// Used to compare heroes based on their xp and gold per minute
// Used to compare player personal gold and xp with the hero compared to average over certain limit
// param 1 - limit used to control how many players to look through in consideration
// param 2 - numHeroes is the number of heroes to compare against
// param 3 - modifier is for checking if file is modified 
vector<tuple<string, string, string>> Submanager_Players::getXPGoldByHero(string limit, string numHeroes, string oldSize, string newSize, bool modifier) {
    vector<tuple<string, string, string>> v1;
    vector<int> numEntries;
    for (unsigned i = 0; i < stoi(numHeroes); i++) {
        v1.push_back(make_tuple(to_string(i), "0", "0"));
        numEntries.push_back(0);
    }

    for (unsigned i = 0; i < stoi(limit); i++) {
        get<1>(v1.at(stoi(players_data[1].at(i)))) = to_string(stoi(get<1>(v1.at(stoi(players_data[1].at(i))))) + stoi(players_data[6].at(i)));
        get<2>(v1.at(stoi(players_data[1].at(i)))) = to_string(stoi(get<2>(v1.at(stoi(players_data[1].at(i))))) + stoi(players_data[7].at(i)));
        numEntries.at(stoi(players_data[1].at(i)))++;
    }

    if (modifier) {
        for (unsigned i = stoi(oldSize)-1; i < players_data[0].size(); i++) {
            get<1>(v1.at(stoi(players_data[1].at(i)))) = to_string(stoi(get<1>(v1.at(stoi(players_data[1].at(i))))) + stoi(players_data[6].at(i)));
            get<2>(v1.at(stoi(players_data[1].at(i)))) = to_string(stoi(get<2>(v1.at(stoi(players_data[1].at(i))))) + stoi(players_data[7].at(i)));
            numEntries.at(stoi(players_data[1].at(i)))++;
        }
    }

    for (unsigned i = 0; i < stoi(numHeroes); i++) {
        if (numEntries.at(i) > 0) {
            get<1>(v1.at(i)) = to_string((double)(stoi(get<1>(v1.at(i)))) / (double) numEntries.at(i));
            get<2>(v1.at(i)) = to_string((double)(stoi(get<2>(v1.at(i)))) / (double) numEntries.at(i));
        }
    }
    ofstream oFS("stat_dump/xpgold_diagnostic.txt");
    if (oFS.is_open()) {
        oFS << oldSize << "\n";
        oFS << players_data[0].size();
    }
    oFS.close(); oFS.clear();
    return v1;
}

int Submanager_Players::getSize() {
    return players_data[0].size();
}

    bool Submanager_Players::insertPlayer(vector<string> input){
        for(unsigned i = 0; i < input.size(); i++){
            if(!checkInput(input.at(i))){ return false; }
        }
            players_data[0].push_back(input.at(0));
            players_data[1].push_back(input.at(1));
            players_data[2].push_back(input.at(2));
            players_data[3].push_back(input.at(3));
            players_data[4].push_back("0");
            players_data[5].push_back("0");
            players_data[6].push_back(input.at(4));
            players_data[7].push_back(input.at(5));
            players_data[8].push_back(input.at(6));
            players_data[9].push_back(input.at(7));
            players_data[10].push_back(input.at(8));
            players_data[11].push_back("0");
            players_data[12].push_back(input.at(9));
            players_data[13].push_back("0");
            players_data[14].push_back(input.at(10));
            players_data[15].push_back("0");
            players_data[16].push_back(input.at(11));
            players_data[17].push_back("0");
            players_data[18].push_back("0");
            players_data[19].push_back("0");
            players_data[20].push_back("0");
            players_data[21].push_back("0");
            players_data[22].push_back("0");
            players_data[23].push_back(input.at(12));
            for(unsigned i = 24; i < PCOLS; i++){
                players_data[i].push_back("0");
            }

        return true;
    }

    bool Submanager_Players::checkInput(string input){
        for (unsigned i = 0; i < input.size(); i++) {
            if (!isdigit(input[i])) {
                return false;
            }
        }
        return true;
    }