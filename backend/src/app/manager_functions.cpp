#include "manager.h"

using namespace std;

// helper function to sort in top abilties
bool sortbysecdesc(const pair<int,int> &a, const pair<int,int> &b)
{
    return a.second > b.second;
}

// FEATURE: Finds average xp and gold per minute for each hero based on a certain number of matches
// param 1 - limit on the number of matches taken into consideration
// param 2 - number of Heroes to perform comparison on
// param 3 - boolean used to describe if data is modified (as in, insert/delete/update)
void analytics_compare_xp_and_gold_heroes(string limit, string numHeroes, bool modified1, bool modified2) {
    cout << "Start analytics compare xp and gold..." << endl;
    //get required information from players file
    Submanager_Players spl = Submanager_Players();
    if (modified1) {
        spl.readFile(spl.csvFile2); // read from temporary_data csv
    } else {
        spl.readFile(spl.csvFile); // read from original csv
    }
    vector<tuple<string,string,string>> v1;
    Submanager_Hero_Names hnames = Submanager_Hero_Names();
    hnames.readFile(hnames.fileName); 
    string filename,filename2 = "";
    if (modified2) {
        hnames.readFile(hnames.fileName2);
    } else {
        hnames.readFile(hnames.fileName);
    }
    if (modified1 || modified2) {
        ifstream inFS("stat_dump/xpgold_diagnostic.txt");
        string temp;
        if (inFS.is_open()) {
            getline(inFS, temp);
            string oldSize = temp;
            getline(inFS, temp);
            string newModSize = temp;
            v1 = spl.getXPGoldByHero(limit, numHeroes, oldSize, newModSize, true);
        }
        inFS.close(); inFS.clear();
    } else {
        v1 = spl.getXPGoldByHero(limit, numHeroes, "0", "0", false);
    }
    cout << "Done!" << endl;
    ostringstream oSS;
    oSS << "{\n\t\"gold_xp_comparison\": [\n";
    vector<string> resNames = hnames.getHnames(stoi(numHeroes));
    int offset = 1;
    for (unsigned i = 0; i < v1.size() && i < resNames.size(); i++) 
    {
        oSS << "\t{\n";
        if (stoi(get<0>(v1.at(i))) == 23) { offset = 2; }
        oSS << "\t\t\"hero_id\": \"" << stoi(get<0>(v1.at(i))) + offset << "\",\n";
        oSS << "\t\t\"hero_name\": \"" << resNames.at(i) << "\",\n";
        oSS << "\t\t\"average_xp_per_min\": \"" << fixed << setprecision(2) << stod(get<1>(v1.at(i))) << "\",\n";
        oSS << "\t\t\"average_gold_per_min\": " << fixed << setprecision(2) << stod(get<2>(v1.at(i))) << "\n\t}";
        if (!(i == v1.size()-1)) 
        {
            oSS << ",\n";
        }
    }
    oSS << "\n\t]\n}";
    ofstream outfile(out13);
    if (outfile.is_open()) 
    {
        outfile << oSS.str();
    }
    outfile.close();
}

// FEATURE: SEARCH top abilities upgraded
// param 1 - limit of abilities to retrieve data for
// param 2 - boolean used to determine if files were modified
void search_top_abilities(string limit, bool modifier)
{
    vector<pair<int,int>> tmp;
    string filename_output = out1;
    ability ab = ability();
    if (modifier) { ab.readFile(ab.fileName2); } // read from temporary_data csv
    else { ab.readFile(ab.fileName); } // read from original csv
    
    ability_upgrades abu = ability_upgrades();
    string filename,filename2 = "";
    if (modifier) { filename = "../../Temporary_Data/ability_upgrade_popularity.txt"; tmp = abu.get_upgrades(stoi(limit), filename, modifier); }
    else { filename = "./stat_dump/ability_upgrade_popularity.txt"; tmp = abu.get_upgrades(stoi(limit), filename, modifier); }
    //cout << limit << endl; //debug
    ostringstream oSS;
    oSS << "{\n\t\"abilities_upgraded\": [\n";
    for (unsigned i = 0; i < tmp.size(); i++) 
    {
        oSS << "\t{\n";
        oSS << "\t\t\"id\": \"" << tmp.at(i).first << "\",\n";
        oSS << "\t\t\"name\": \"" << ab.get_ability_name(tmp.at(i).first) << "\",\n";
        oSS << "\t\t\"numTimes_upgraded\": " << (tmp.at(i).second) << "\n\t}";
        if (!(i == tmp.size()-1)) 
        {
            oSS << ",\n";
        }
    }
    oSS << "\n\t]\n}";
    ofstream outfile(out1);
    if (outfile.is_open()) 
    {
        outfile << oSS.str();
    }
    outfile.close();
}

// FEATURE: SEARCH top abilities upgraded
// param 1 - limit of heroes to retrieve data for
// param 2 - boolean used to determine if files were modified
// param 3 - temporary vector to hold newly inserted heroes
// param 4 - boolean to check if matches were modified
void search_heroes(string limit, bool modified1, vector<vector<string>> tmp_new, bool modified3)
{
    //variables
    vector<string> h_ids;
    vector<string> h_names;
    vector<string> h_pickRate;
    vector<tuple<string,string,string>> h_id_match_playerSlot; 
    vector<bool> win_loss;
    int matchCount;
    vector<double> result;
    vector<double> result_win;

    //output file
    string filename_output = out3;

    //preliminary error handling and timer setup
    time_t txtMTime = 0;
    struct stat fileInfoTxt; //struct to access txt modified time
    int err = stat("./stat_dump/hero_stats.txt", &fileInfoTxt);
    if (err != 0) { 
        perror("hero_stats.txt"); 
        cout << "Processing Data" << endl;
    }
    else { txtMTime = fileInfoTxt.st_mtime; }
    struct stat fileInfoCsv; //struct to access cvs mod time
    string extra = "../../Data/players.csv";
    err = stat(extra.c_str(), &fileInfoCsv);
    if (err != 0) {
        perror("../../Data/players.csv");
        exit(errno);
    }
    time_t CSVMTime = fileInfoCsv.st_mtime;

    //if CSV is newer than txt, do below
    if (CSVMTime > txtMTime)
    { 
        Submanager_Hero_Names hn;
        if (modified1) { hn.readFile(hn.fileName2); } // read from temporary_data csv
        else { hn.readFile(hn.fileName); } // read from original csv

        // helper function getIDSize() gets the largest limit used to fix display when data is modified
        // limit is used in JSON output instead
        h_ids = hn.getHids(hn.getIDSize()); 
        h_names = hn.getHnames(hn.getIDSize());

        // initialize Submanager_Players object and get required information
        // Required information includes hero pick rate and hero_id, match_id, and player_slot
        Submanager_Players pls;
        pls.readFile(pls.csvFile);
        h_pickRate = pls.getHeroPickRate(hn.getIDSize());
        h_id_match_playerSlot = pls.getHeroMatch();
        
        // sort based on first entry, which is hero_id
        sort(h_id_match_playerSlot.begin(),h_id_match_playerSlot.end());

        // Initialize Submanger_Match object and get required information
        Submanager_Match mat;
        if (modified3) { mat.readFile(mat.fileName2); } // read from temporary_data csv
        else { mat.readFile(mat.fileName); } // read from original csv

        //Required information from Match Object is win_loss and number of matches considered
        win_loss = mat.findWinLoss();
        matchCount = mat.getMatchCount();

        //debug here is kept for vital information when server is running
        cout << "Search Heroes: h_pickrate: size: " << h_pickRate.size() << endl; //debug
        cout << "Search Heroes: h_ids: size: " << h_ids.size() << endl; //debug
        for (int i = 0; i < h_ids.size(); i++)
        {
            cout << "i = " << i << endl; //debug
            cout << "stoi(h_ids.at(i))) = " << stoi(h_ids.at(i)) << endl;
            double num = stod(h_pickRate.at(stoi(h_ids.at(i))));
            cout << "HERE #" << i << endl; //debug
            if(num == 0){
                result.push_back(0);
                result_win.push_back(0);
                continue;
            }
            cout << num << ", " << matchCount << endl; //debug
            num /= matchCount;
            result.push_back(num);

            // Binary Search Variables
            int first = 0;
            int last = h_id_match_playerSlot.size() - 1;
            int middle;
            bool found = false;
            // Binary Search
            while (!found && first <= last)
            {
                middle = ( first + last ) / 2;
                if (get<0>(h_id_match_playerSlot.at(middle)) == h_ids.at(i)) { found = true; }
                else if (get<0>(h_id_match_playerSlot.at(middle)) > h_ids.at(i)) { last = middle - 1; }
                else { first = middle + 1; }
            }
            found = false;
            while (!found && middle >= 0 )
            {
                middle--;
                if (get<0>(h_id_match_playerSlot.at(middle)) != h_ids.at(i))
                {
                    found = true;    
                    middle++;
                    break;                
                }
            }

            // Calcuations for results from wins and pickRate
            result_win.push_back(0);
            for (int j = middle; j < h_id_match_playerSlot.size(); j++)
            {
                if (get<0>(h_id_match_playerSlot.at(j)) != h_ids.at(i)) { break; }
                bool temporary = win_loss.at(stoi(get<1>(h_id_match_playerSlot.at(j))));
                if (temporary && (stoi(get<2>(h_id_match_playerSlot.at(j))) < 100)) { result_win.at(result_win.size() - 1) += 1; }
                else if (!temporary && (stoi(get<2>(h_id_match_playerSlot.at(j))) > 100)) { result_win.at(result_win.size() - 1) += 1; }
            }
            cout << "result_win.size() = " << result_win.size() << endl;
            result_win.at(result_win.size() - 1) /= stod(h_pickRate.at(stoi(h_ids.at(i))));
        }

        // Open file and place information into the file
        ofstream file;
        file.open("./stat_dump/hero_stats.txt");
        if (!file.is_open()) { file.close(); cout << "Something went wrong.  The file was corrupt." << endl; }
        for (int i = 0; i < hn.getIDSize(); i++) { file << h_ids.at(i) << "," << h_names.at(i) << "," << result_win.at(i) << "," << result.at(i) << endl; /*dump all data in dummy to txt File*/ }    
        file.close();
    }
    else
    {
        if (CSVMTime < txtMTime )
        { //if txt file is newer than csv file
            ifstream file;
            string fileTemp;
            if (modified1) { file.open("../../Temporary_Data/hero_stats.txt", ifstream::in); }
            else { file.open("./stat_dump/hero_stats.txt", ifstream::in); }
            if (!file.is_open()) { file.close(); cout << "Something went wrong.  The file was corrupt." << endl; }

            while (getline(file, fileTemp)) 
            {
                stringstream ss(fileTemp);
                string value;
                for (unsigned i = 0; i < 4; i++) 
                { 
                    if (getline(ss,value,',')) //read each line of the txt file, create pair based off the line and push into result
                    { 
                        if (i == 0) { h_ids.push_back(value); }
                        else if (i == 1) { h_names.push_back(value); }
                        else if (i == 2) { result_win.push_back(stod(value)); }
                        else if (i == 3) { result.push_back(stod(value)); }
                    }
                }
            }
            file.close();
        }
    }

    // Output to stringstream, which saves time compared to writing directly to file
    ostringstream oSS;
    oSS << "{\n";
    oSS << "\t\"heroes\": [\n";
    unsigned display;
    if (modified1) { display = stoi(limit) - tmp_new.at(1).size(); }
    else { display = stoi(limit); } 
    for (unsigned i = 0; i < display; i++) 
    {
        oSS << "\t{\n";
        oSS << "\t\t\"id\": \"" << h_ids.at(i) << "\",\n";
        oSS << "\t\t\"name\": \"" << h_names.at(i) << "\",\n";
        oSS << "\t\t\"win_rate\": \"";
        oSS << fixed << setprecision(1) << result_win.at(i) * 100;
        oSS << "\",\n";
        oSS << "\t\t\"pick_rate\": \"";
        oSS << fixed << setprecision(2) << result.at(i);
        oSS << "\"\n\t\t}";
        if (modified1 && tmp_new.at(1).size() > 0 || !(i == stoi(limit)-1)) 
        {
            oSS << ",\n";
        }
    }

    // Checks if HeroNames csv has been modified
    if (modified1) {
        for (unsigned i = 0; i < tmp_new.at(1).size(); i++)
        {
            stringstream newSS(tmp_new.at(1).at(i));
            string val;

            oSS << "\t{\n";
            for (unsigned k = 0; k < 4; k++) 
            { 
                if (getline(newSS,val,',')) 
                { 
                    if (k == 0) 
                    { 
                        oSS << "\t\t\"id\": \"" << val << "\",\n"; 
                    }
                    else if (k == 1) 
                    { 
                        oSS << "\t\t\"name\": \"" << val << "\",\n"; 
                    }
                    else if (k == 2) 
                    { 
                        oSS << "\t\t\"win_rate\": \"";
                        oSS << fixed << setprecision(1) << stod(val) * 100;
                        oSS << "\",\n";
                    }
                    else if (k == 3) 
                    { 
                        oSS << "\t\t\"pick_rate\": \"";
                        oSS << fixed << setprecision(2) << stod(val);
                        oSS << "\"\n\t\t}";
                    }
                }
            }
            if (tmp_new.at(1).size()-1 != 0) 
            {
                oSS << ",\n";
            }
        }
    }

    oSS << "\n\t]\n}";
    
    ofstream outfile(out3);
    if (outfile.is_open()) 
    {
        outfile << oSS.str();
    }
}

void search_players(string limit, bool modified2, vector<vector<string>> tmp_new)
{
    Submanager_Player_Ratings pr = Submanager_Player_Ratings();
    if (modified2) { pr.readFile(pr.fileName2); }
    else { pr.readFile(pr.fileName); }
    
    vector<string> tmp = pr.getAccountIDs(stoi(limit));

    ostringstream oSS;
    oSS << "{\n\t\"players\": [\n";
    unsigned display;
    if (modified2) { display =  tmp.size() - tmp_new.at(2).size(); }
    else { display = tmp.size(); }
    for (unsigned i = 0; i < display; i++) 
    {
        oSS << "\t{\n";
        oSS << "\t\t\"id\": \"" << tmp.at(i) << "\",\n";
        oSS << "\t\t\"win_rate\": \"";
        oSS << fixed << setprecision(1) << pr.getTotalWins(stoi(tmp.at(i))) / (double)pr.getTotalMatches(stoi(tmp.at(i))) * 100;
        oSS << "\",\n";
        oSS << "\t\t\"total_wins\": " << pr.getTotalWins(stoi(tmp.at(i))) << ",\n";
        oSS << "\t\t\"total_matches\": " << pr.getTotalMatches(stoi(tmp.at(i))) << "\n\t}";
        if (modified2 && tmp_new.at(2).size() > 0 || !(i == tmp.size()-1)) 
        {
            oSS << ",\n";
        }
    }
    if (modified2)
    {
        string tmp_id, tmp_win, tmp_match, tmp_mu, tmp_sigma;
        for (unsigned i = 0; i < tmp_new.at(2).size(); i++)
        {
            stringstream ss(tmp_new.at(2).at(i));
            string value;
            for (unsigned k = 0; k < 5; k++)
            {
                if (getline(ss, value, ','))
                {
                    if (k == 0) { tmp_id = value; }
                    else if (k == 1) { tmp_win = value; }
                    else if (k == 2) { tmp_match = value; }
                }
            }
            oSS << "\t{\n";
            oSS << "\t\t\"id\": \"" << tmp_id << "\",\n";
            oSS << "\t\t\"win_rate\": \"";
            oSS << fixed << setprecision(1) << stod(tmp_win) / stod(tmp_match) * 100;
            oSS << "\",\n";
            oSS << "\t\t\"total_wins\": " << tmp_win << ",\n";
            oSS << "\t\t\"total_matches\": " << tmp_match << "\n\t}";
            if (tmp_new.at(2).size()-1 != 0) { oSS << ",\n"; }
        }
    }
    oSS << "\n\t]\n}";
    ofstream outfile(out2);
    if (outfile.is_open()) { outfile << oSS.str(); }
    outfile.close();
}

void search_first_blood_time(string limit, bool modified3, vector<vector<string>> tmp_new)
{
    vector<pair<string,string>> fb;
    {
        Submanager_Objective a;
        a.readFile(a.csvfile);
        fb = a.getFirstBlood(stoi(limit)*2);
    }
    vector<bool> winloss;
    {
        Submanager_Match b;
        if (modified3) { b.readFile(b.fileName2); } // read from temporary_data csv
        else { b.readFile(b.fileName); } // read from original csv

        winloss = b.findWinLoss();
    }
    vector<string> result;
    for (unsigned i = 0; i < fb.size(); i++)
    {
        if (stoi(fb.at(i).second) <= 4 && winloss.at(i) == true) { result.push_back(fb.at(i).first); }
        else if (stoi(fb.at(i).second) >= 4 && winloss.at(i) == false) { result.push_back(fb.at(i).first); }
    }
    Submanager_Match m = Submanager_Match();
    if (modified3) { m.readFile(m.fileName2); }
    else { m.readFile(m.fileName); }
    Submanager_Cluster c = Submanager_Cluster();
    c.readFile("../../Data/cluster_regions.csv");
    ostringstream oSS;
    oSS << "{\n";
    oSS << "\t\"matches\": [\n";
    unsigned k = min(stoi(limit), (int)result.size());
    if (modified3) { k -= tmp_new.at(3).size(); }
    for (unsigned i = 0; i < k; i++) {
        oSS << "\t{\n";
        oSS << "\t\t\"id\": \"" << result.at(i) << "\",\n";
        oSS << "\t\t\"win_type\": \"";
        if (m.checkRadiantWin(result.at(i))) { oSS << "Radiant"; }
        else { oSS << "Dire"; }
        oSS << "\",\n";
        oSS << "\t\t\"first_blood_time\": " << m.getFirstBloodTime(result.at(i)) << ",\n";
        oSS << "\t\t\"region_id\": " << m.getCluster(result.at(i)) << ",\n";
        oSS << "\t\t\"region\": \"" << c.getClusterName(m.getCluster(result.at(i))) << "\",\n ";
        oSS << "\t\t\"likes\": " << m.getLikes(result.at(i)) << ",\n";
        oSS << "\t\t\"dislikes\": " << m.getDislikes(result.at(i)) << "\n\t}";
        if (modified3 && tmp_new.at(3).size() > 0 || !(i == k - 1)) { oSS << ",\n"; }
    }
    
    if (modified3) {
        for (unsigned i = 0; i < tmp_new.at(3).size(); i++)
        {
            oSS << "\t{\n";
            oSS << "\t\t\"id\": \"" << tmp_new.at(3).at(i) << "\",\n";
            oSS << "\t\t\"win_type\": \"";
            if (m.checkRadiantWin(tmp_new.at(3).at(i))) { oSS << "Radiant"; }
            else { oSS << "Dire"; }
            oSS << "\",\n";
            oSS << "\t\t\"first_blood_time\": " << m.getFirstBloodTime(tmp_new.at(3).at(i)) << ",\n";
            oSS << "\t\t\"region_id\": " << m.getCluster(tmp_new.at(3).at(i)) << ",\n";
            oSS << "\t\t\"region\": \"" << c.getClusterName(m.getCluster(tmp_new.at(3).at(i))) << "\",\n ";
            oSS << "\t\t\"proportion\": \""; 
            oSS << fixed << setprecision(1) << stoi(m.getLikes(tmp_new.at(3).at(i))) / stod(m.getDislikes(tmp_new.at(3).at(i))) * 100;
            oSS << "\",\n";
            oSS << "\t\t\"likes\": " << m.getLikes(tmp_new.at(3).at(i)) << ",\n";
            oSS << "\t\t\"dislikes\": " << m.getDislikes(tmp_new.at(3).at(i)) << "\n\t}";
            if (tmp_new.at(3).size()-1 != 0) { oSS << ",\n"; }
        }
    }
    oSS << "\n\t]\n}";
    ofstream outFS(out4);
    if (outFS.is_open()) { outFS << oSS.str(); }
    outFS.close();
}

void search_cluster_region(string limit, bool modified3, vector<vector<string>> tmp_new)
{
    Submanager_Match m = Submanager_Match();
    if (modified3) { m.readFile(m.fileName2); } // read from temporary_data csv
    else { m.readFile(m.fileName); } // read from original csv
    int display;
    if (modified3) { display = stoi(limit) - tmp_new.at(3).size(); }
    else { display = stoi(limit); } 
    vector<string> tmp = m.getMatchIDs(display);
    Submanager_Cluster c = Submanager_Cluster();
    c.readFile("../../Data/cluster_regions.csv");
    ostringstream oSS;
    oSS << "{\n";
    oSS << "\t\"matches\": [\n";
    int outputLimit;
    if (modified3) { outputLimit = tmp.size()-tmp_new.at(3).size(); }
    else { outputLimit = tmp.size(); } 

    for (unsigned i = 0; i < outputLimit; i++) {
        oSS << "\t{\n";
        oSS << "\t\t\"id\": \"" << tmp.at(i) << "\",\n";
        oSS << "\t\t\"win_type\": \"";
        if (m.checkRadiantWin(tmp.at(i))) { oSS << "Radiant"; }
        else { oSS << "Dire"; }
        oSS << "\",\n";
        oSS << "\t\t\"first_blood_time\": " << m.getFirstBloodTime(tmp.at(i)) << ",\n";
        oSS << "\t\t\"region_id\": " << m.getCluster(tmp.at(i)) << ",\n";
        oSS << "\t\t\"region\": \"" << c.getClusterName(m.getCluster(tmp.at(i))) << "\",\n ";
        oSS << "\t\t\"likes\": " << m.getLikes(tmp.at(i)) << ",\n";
        oSS << "\t\t\"dislikes\": " << m.getDislikes(tmp.at(i)) << "\n\t}";
        if (modified3 && tmp_new.at(3).size() > 0 || !(i == outputLimit-1)) { oSS << ",\n"; }
    }
    if (modified3) {
        for (unsigned i = 0; i < tmp_new.at(3).size(); i++)
        {
            oSS << "\t{\n";
            oSS << "\t\t\"id\": \"" << tmp_new.at(3).at(i) << "\",\n";
            oSS << "\t\t\"win_type\": \"";
            if (m.checkRadiantWin(tmp_new.at(3).at(i))) { oSS << "Radiant"; }
            else { oSS << "Dire"; }
            oSS << "\",\n";
            oSS << "\t\t\"first_blood_time\": " << m.getFirstBloodTime(tmp_new.at(3).at(i)) << ",\n";
            oSS << "\t\t\"region_id\": " << m.getCluster(tmp_new.at(3).at(i)) << ",\n";
            oSS << "\t\t\"region\": \"" << c.getClusterName(m.getCluster(tmp_new.at(3).at(i))) << "\",\n ";
            oSS << "\t\t\"proportion\": \""; 
            oSS << fixed << setprecision(1) << stoi(m.getLikes(tmp_new.at(3).at(i))) / stod(m.getDislikes(tmp_new.at(3).at(i))) * 100;
            oSS << "\",\n";
            oSS << "\t\t\"likes\": " << m.getLikes(tmp_new.at(3).at(i)) << ",\n";
            oSS << "\t\t\"dislikes\": " << m.getDislikes(tmp_new.at(3).at(i)) << "\n\t}";
            if (tmp_new.at(3).size()-1 != 0) { oSS << ",\n"; }
        }
    }
    oSS << "\n\t]\n}";
    ofstream outfile(out5);
    if (outfile.is_open()) { outfile << oSS.str(); }
    outfile.close();
}

void search_most_liked_match(string limit, bool modified3, vector<vector<string>> tmp_new)
{
    Submanager_Match m = Submanager_Match();
    if (modified3) { m.readFile(m.fileName2); } // read from temporary_data csv
    else { m.readFile(m.fileName); } // read from original csv
    Submanager_Cluster c = Submanager_Cluster();
    c.readFile("../../Data/cluster_regions.csv");
    priority_queue<pair<double, string>> likesSorted;
    unsigned numMatches = m.getMatchCount();
    vector<int> likes;
    vector<int> dislikes;
    unsigned i = 0;

    for (unsigned i = 0; i < numMatches; ++i) 
    {
        double pos_votes = atol(m.getLikesByRow(i).c_str());
        double tot_votes = pos_votes + atol(m.getDisLikesByRow(i).c_str());
        likesSorted.push(make_pair(pos_votes / tot_votes, m.getMatchIDByRow(i)));
        likes.push_back(atol(m.getLikesByRow(i).c_str()));
        dislikes.push_back(atol(m.getDisLikesByRow(i).c_str()));
    }

    ostringstream oSS;
    oSS << "{\n\t\"matches\": [\n";
    int tmp = stoi(limit);
    if (modified3) { tmp -= tmp_new.at(3).size(); }
    while (tmp != 0) {
        oSS << "\t{\n";
        oSS << "\t\t\"id\": \"" << likesSorted.top().second << "\",\n";
        oSS << "\t\t\"win_type\": \"";
        if (m.checkRadiantWin(likesSorted.top().second)) { oSS << "Radiant"; }
        else { oSS << "Dire"; }
        oSS << "\",\n";
        oSS << "\t\t\"first_blood_time\": " << m.getFirstBloodTime(likesSorted.top().second) << ",\n";
        oSS << "\t\t\"region_id\": " << m.getCluster(likesSorted.top().second) << ",\n";
        oSS << "\t\t\"region\": \"" << c.getClusterName(m.getCluster(likesSorted.top().second)) << "\",\n ";
        oSS << "\t\t\"proportion\": \""; 
        oSS << fixed << setprecision(1) << likesSorted.top().first * 100;
        oSS << "\",\n";
        likesSorted.pop();
        oSS << "\t\t\"likes\": " << likes.at(i) << ",\n";
        oSS << "\t\t\"dislikes\": " << dislikes.at(i++) << "\n\t}";
        tmp--;
        if ((modified3 && tmp_new.at(3).size() > 0 || tmp != 0)) {  oSS << ",\n";  }
    }

    if (modified3) {
        for (unsigned i = 0; i < tmp_new.at(3).size(); i++)
        {
            oSS << "\t{\n";
            oSS << "\t\t\"id\": \"" << tmp_new.at(3).at(i) << "\",\n";
            oSS << "\t\t\"win_type\": \"";
            if (m.checkRadiantWin(tmp_new.at(3).at(i))) { oSS << "Radiant"; }
            else { oSS << "Dire"; }
            oSS << "\",\n";
            oSS << "\t\t\"first_blood_time\": " << m.getFirstBloodTime(tmp_new.at(3).at(i)) << ",\n";
            oSS << "\t\t\"region_id\": " << m.getCluster(tmp_new.at(3).at(i)) << ",\n";
            oSS << "\t\t\"region\": \"" << c.getClusterName(m.getCluster(tmp_new.at(3).at(i))) << "\",\n ";
            oSS << "\t\t\"proportion\": \""; 
            oSS << fixed << setprecision(1) << stoi(m.getLikes(tmp_new.at(3).at(i))) / stod(m.getDislikes(tmp_new.at(3).at(i))) * 100;
            oSS << "\",\n";
            oSS << "\t\t\"likes\": " << m.getLikes(tmp_new.at(3).at(i)) << ",\n";
            oSS << "\t\t\"dislikes\": " << m.getDislikes(tmp_new.at(3).at(i)) << "\n\t}";
            if (tmp_new.at(3).size()-1 != 0) { oSS << ",\n"; }
        }
    }
    oSS << "\n\t]\n}";
    ofstream outFS(out6);
    if (outFS.is_open()) { outFS << oSS.str(); }
    outFS.close(); 
}

void insert_ability(string parameter0, string parameter1, string parameter13, bool modified0)
{
    int ab_id = 1;
    string ab_name = parameter0;
    string upgrade_cnt = parameter1;
    ability ab = ability();
    if (modified0) { ab.readFile(ab.fileName2); } // read from temporary_data csv
    else { ab.readFile(ab.fileName); } // read from original csv

    // add ability to respective CSV file
    while (ab.get_ability_name(ab_id) != "NOT FOUND") { ab_id++; } // find an available id
    ab.insert_ability(ab_id,ab_name); 
    bool flag = stoi(parameter13);
    ab.writeFile(flag, 'o');

    // add upgrade count to stat txt file
    ifstream inFS;
    string tempLine;
    vector<string> temp_ids, temp_upgrades;
    bool inserted = false;
    unsigned size = 0; //change this back to 0 if it causes problems
    if (modified0) { inFS.open("../../Temporary_Data/ability_upgrade_popularity.txt", ifstream::in); }
    else { inFS.open("./stat_dump/ability_upgrade_popularity.txt", ifstream::in); }
    if (!inFS.is_open()) { inFS.close(); cout << "Something went wrong.  The file was corrupt." << endl; }
    while (getline(inFS, tempLine))
    {
        stringstream ss(tempLine);
        string value;
        for (unsigned i = 0; i < 2; i++)
        {
            if (getline(ss, value, ','))
            {
                if (i == 0) { temp_ids.push_back(value); }
                else if (i == 1) { temp_upgrades.push_back(value); }
            }
        }

        // sort inserted ability
        if (!inserted && stoi(temp_upgrades.at(size)) < stoi(upgrade_cnt))
        {
            vector<string> temp(2);
            temp.at(0) = temp_ids.at(size);
            temp.at(1) = temp_upgrades.at(size);
            temp_ids.at(size) = to_string(ab_id);
            temp_upgrades.at(size) = upgrade_cnt;
            temp_ids.push_back(temp.at(0));
            temp_upgrades.push_back(temp.at(1));
            inserted = true;
        }
        size++;
    }

    inFS.close(); inFS.clear();
    ofstream outfile(out7);
    if (outfile.is_open()) { outfile << "{ \"results\": [ { \"result\": \"" << true << "\"" << "} ] }"; }
    outfile.close(); outfile.clear();
    ofstream outFS("../../Temporary_Data/ability_upgrade_popularity.txt");
    if (!outFS.is_open()) { outFS.close(); cout << "Something went wrong.  The file was corrupt." << endl; }
    for (unsigned i = 0; i < size - 1; i++) { outFS << temp_ids.at(i) << "," << temp_upgrades.at(i) << endl; }
    outFS.close(); outFS.clear();
}

void analytics_winrates_over_time(string numHeroes, bool modifier) 
{
    time_t start, end;
    time(&start);
    cout << "Begin analytics_winrates_over_time...";
    vector<bool> winloss;
    vector<double> duration;
    //First part that is done for reusuablity
    //costly calculation if you only search for one hero, gets better if you do multiple hero searches
    //someone has to implement the stats file dump :)
    Submanager_Match m = Submanager_Match();
    if (modifier) {
        m.readFile(m.fileName2);
    } else {
        m.readFile(m.fileName);
    }
    winloss = m.findWinLoss();
    duration = m.findDuration();
    vector<tuple<string,string,string>> info;
    Submanager_Players p = Submanager_Players();
    if (modifier) {
        p.readFile(p.csvFile2);
    } else {
        p.readFile(p.csvFile);
    }

    info = p.getMatchHero();

    vector<tuple<string,string,bool,bool,bool,bool,bool,bool,bool>> matches;
    bool win, less20, more20, more30, more40, more50, more60;
    string matchid;
    string heroid;
    double temptime;
    for(unsigned i = 0; i < info.size(); i++)
    {
        matchid = get<0>(info.at(i));
        heroid = get<1>(info.at(i));
        if(stoi(get<2>(info.at(i))) < 100 && winloss.at(stoi(matchid)) == true)
        {
            win = true;
        }
        else if(stoi(get<2>(info.at(i))) > 100 && winloss.at(stoi(matchid)) == false)
        {
            win = true;
        }
        else
        {
            win = false;
        }
        temptime = duration.at(stoi(matchid))/60;
        (temptime < 20) ? less20 = true : less20 = false;
        (temptime >= 20) ? more20 = true : more20 = false;
        (temptime >= 30) ? more30 = true : more30 = false;
        (temptime >= 40) ? more40 = true : more40 = false;
        (temptime >= 50) ? more50 = true : more50 = false;
        (temptime >= 60) ? more60 = true : more60 = false;
        matches.push_back(make_tuple(matchid,heroid,win,less20,more20,more30,more40,more50,more60));
    }
    // cout << "Done with slow part";
    //THIS IS WHERE YOU'D STORE THE INFO INTO A STATS FILE IF YOU WANTED TO NOT REDO THIS COMPUTATION
    //Alternatively you could store the winrate of a specific hero into a stats file and only update that


    ostringstream oSS;
    oSS << "{\n";
    oSS << "\t\"winrates\": [\n";
    for (unsigned hero = 1; hero <= stoi(numHeroes); hero++)
    {
        int wless20 = 0, tless20 = 0;
        int wmore20 = 0, tmore20 = 0;
        int wmore30 = 0, tmore30 = 0;
        int wmore40 = 0, tmore40 = 0;
        int wmore50 = 0, tmore50 = 0;
        int wmore60 = 0, tmore60 = 0;

        vector<double> result;
        for (unsigned i = 0; i < matches.size(); i++)
        {
            if (get<1>(matches.at(i)) == to_string(hero - 1)) // hero id comparison, offset to match dataset
            {
                if (get<3>(matches.at(i)))
                {
                    tless20++;
                    if(get<2>(matches.at(i))) wless20++; 
                    continue;
                }
                if(get<8>(matches.at(i)))
                {
                    tmore20++; tmore30++; tmore40++; tmore50++; tmore60++;
                    if(get<2>(matches.at(i))) { wmore20++; wmore30++; wmore40++; wmore50++; wmore60++; }
                    continue;
                }
                if(get<7>(matches.at(i)))
                {
                    tmore20++; tmore30++; tmore40++; tmore50++;
                    if(get<2>(matches.at(i))) { wmore20++; wmore30++; wmore40++; wmore50++; }
                    continue;
                }
                if(get<6>(matches.at(i)))
                {
                    tmore20++; tmore30++; tmore40++;
                    if(get<2>(matches.at(i))) { wmore20++; wmore30++; wmore40++; }
                    continue;
                }
                if(get<5>(matches.at(i)))
                {
                    tmore20++; tmore30++;
                    if(get<2>(matches.at(i))) { wmore20++; wmore30++; }
                    continue;
                }
                if(get<4>(matches.at(i)))
                {
                    tmore20++;
                    if(get<2>(matches.at(i))) { wmore20++; }
                    continue;
                }
            }
        }
        if (tless20 > 0) { result.push_back(wless20/(double)tless20 * 100); } //winrates of matches ending in < 20 minutes
        else { result.push_back(0); }
        
        if (tmore20 > 0) { result.push_back(wmore20/(double)tmore20 * 100); } //winrates of matches ending in > 20 minutes
        else { result.push_back(0); }
        
        if (tmore30 > 0) { result.push_back(wmore30/(double)tmore30 * 100); } //winrates of matches ending in > 30 minutes
        else { result.push_back(0); }
        
        if (tmore40 > 0) { result.push_back(wmore40/(double)tmore40 * 100); } //winrates of matches ending in > 40 minutes
        else { result.push_back(0); }
        
        if (tmore50 > 0) { result.push_back(wmore50/(double)tmore50 * 100); } //winrates of matches ending in > 50 minutes
        else { result.push_back(0); }
        
        if (tmore60 > 0) { result.push_back(wmore60/(double)tmore60 * 100); } //winrates of matches ending in > 60 minutes
        else { result.push_back(0); }

        //result vector contains winrates for a specific hero based on timebrackets of 10 minutes starting at 20 minutes.

        oSS << "\t{\n";
        oSS << "\t\t\"id\": \"";
        if (hero >= 24) { oSS << hero + 1; } // offset match id for frontend (no hero has id 24)
        else { oSS << hero; }
        oSS << "\",\n";
        oSS << "\t\t\"wless20\": \"" << fixed << setprecision(2) << result.at(0) << "\",\n";
        oSS << "\t\t\"wmore20\": \"" << fixed << setprecision(2) << result.at(1) << "\",\n";
        oSS << "\t\t\"wmore30\": \"" << fixed << setprecision(2) << result.at(2) << "\",\n ";
        oSS << "\t\t\"wmore40\": \"" << fixed << setprecision(2) << result.at(3) << "\",\n";
        oSS << "\t\t\"wmore50\": \"" << fixed << setprecision(2) << result.at(4) << "\",\n";
        oSS << "\t\t\"wmore60\": \"" << fixed << setprecision(2) << result.at(5) << "\"\n\t}";
        if (hero != stoi(numHeroes)) { oSS << ",\n"; }
    }
    oSS << "\n\t]\n}";
    ofstream outFS(out9);
    if (outFS.is_open()) {
        outFS << oSS.str();
    }
    outFS.close();

    time(&end);
    double time_taken = double(end - start);
    cout << "\nTime taken by analytics_winrates_over_time feature is: " << fixed <<  time_taken << setprecision(5);
    cout << " sec" << endl;
}

void analytics_first_blood_winrate(string numMatches, bool modifier) 
{
    time_t start, end;
    time(&start);
    cout << "Begin analytics_first_blood_winrate...";
    vector<pair<string,string>> fb;
    Submanager_Objective a;
    if (modifier) {
        a.readFile(a.csvfile2);
    } else {
        a.readFile(a.csvfile);
    }
    fb = a.getFirstBlood(50000);

    vector<bool> winloss;
    Submanager_Match b;
    if (modifier) {
        b.readFile(b.fileName2);
    } else {
        b.readFile(b.fileName);
    }
    winloss = b.findWinLoss();

    vector<string> r;
    double result, radiant, dire;
    for (unsigned i = 0; i < fb.size(); i++)
    {
        if (stoi(fb.at(i).second) <= 4 && winloss.at(i) == true)
        {
            r.push_back(fb.at(i).first);
            result++;
            radiant++;
        }
        else if (stoi(fb.at(i).second) >= 4 && winloss.at(i) == false)
        {
            r.push_back(fb.at(i).first);
            result++;
            dire++;
        }
    }
    radiant /= result; // get winrate of radiant wins out of first blood
    dire /= result; // get winrate of dire wins out of first blood
    result /= winloss.size();
    
    Submanager_Match m = Submanager_Match();
    m.readFile(m.fileName);    
    ostringstream oSS;
    oSS << "{\n";
    oSS << "\t\"winrates\": [\n";
    for (unsigned i = 0; i < stoi(numMatches); i++)
    {
        oSS << "\t{\n";
        oSS << "\t\t\"id\": \"" << r.at(i) << "\",\n";
        oSS << "\t\t\"general\": \"" << fixed << setprecision(2) << result * 100 << "\",\n";
        oSS << "\t\t\"specific\": \"";
        if (m.checkRadiantWin(r.at(i))) { oSS << fixed << setprecision(2) << radiant * 100; }
        else { oSS << fixed << setprecision(2) << dire * 100; }
        oSS << "\"\n\t}";
        if (i != (stoi(numMatches)-1)) { oSS << ",\n"; }
    }
    oSS << "\n\t]\n}";
    ofstream outFS(out11);
    if (outFS.is_open()) {
        outFS << oSS.str();
    }
    outFS.close();

    time(&end);
    double time_taken = double(end - start);
    cout << "\nTime taken by analytics_winrates_over_time feature is: " << fixed <<  time_taken << setprecision(5);
    cout << " sec" << endl;
}

//helper function: to refactor analytics_search_vs_winrate
void search_vs_winrate_helper(int chosenHero, string numMatches, string startHeroId, string endHeroId, bool modifier, vector<bool> winarr, vector<tuple<string,string,string>> matcharrange,
    vector<string> players_in_match, vector<string> slot, vector<pair<double, double>> & matches_vs, int startmatch) {
        for (unsigned i = startmatch; i < stoi(numMatches); i++)
        {
            for (unsigned j = 0; j <  matcharrange.size(); j++)
            {
                if (get<0>(matcharrange.at(j)) == to_string(i))
                {
                    players_in_match.push_back(get<1>(matcharrange.at(j)));
                    slot.push_back(get<2>(matcharrange.at(j)));
                }
            }
            for (unsigned j = 0; j < players_in_match.size(); j++){
                // offset parameter to match dataset
                if (players_in_match.at(j) == to_string(chosenHero-1))
                {
                    if (stoi(slot.at(j)) < 100)
                    {
                        for (unsigned k = 0; k < players_in_match.size(); k++)
                        {
                            if (stoi(slot.at(k)) > 100)
                            { 
                                matches_vs.at(stoi(players_in_match.at(k))).second++;
                                if (winarr.at(i) == true)
                                {
                                    matches_vs.at(stoi(players_in_match.at(k))).first++;
                                }
                            }
                        }
                    }
                    else if (stoi(slot.at(j)) > 100)
                    {
                        for (unsigned k = 0; k < players_in_match.size(); k++)
                        {
                            if (stoi(slot.at(k)) < 100)
                            { 
                                matches_vs.at(stoi(players_in_match.at(k))).second++;
                                if (winarr.at(i) == true)
                                {
                                    matches_vs.at(stoi(players_in_match.at(k))).first++;
                                }
                            }
                        }
                    }
                }
            }
            players_in_match.clear();
            slot.clear();
            cout << "match entry complete : " << i << endl;
        }
    }

void analytics_search_vs_winrate(string numMatches, string startHeroId, string endHeroId, bool modifier)
{
    time_t start, end;
    time(&start);
    cout << "Begin analytics_search_vs_winrate...";
    Submanager_Hero_Names hn = Submanager_Hero_Names();
    if (modifier) {
        hn.readFile(hn.fileName2);
    } else {
        hn.readFile(hn.fileName);
    }
    vector<string> names = hn.getHnames(NUM_HEROES);
    Submanager_Match m = Submanager_Match();
    if (modifier) {
        m.readFile(m.fileName2);
    } else {
        m.readFile(m.fileName);
    }
    Submanager_Players p = Submanager_Players();
    if (modifier) {
        p.readFile(p.csvFile2);
    } else {
        p.readFile(p.csvFile);
    }
    
    string filename = "stat_dump/winrates_diagnostic.txt";
    ifstream inFS(filename);
    string limit;
    //first line is search_vs_winrate old limit
    if (inFS.is_open()) {
        getline(inFS, limit);
        inFS.close(); inFS.clear();
    }
    //super costly calculation.
    //essentially, it goes through the data output by the players.csv file
    //in this case, the matchid, heroid, and slot
    //this goes match by match up to the limit specified by parameter[1], checking for our desired hero existing in the match
    //if it finds the heroid specified by parameter[0]
    //it checks the opponents, tallying the total matches vs that heroid
    //if the parameter[0] hero won the match, it also tallies a win.
    //replaced parameter[1] with 2000 now until we know more about what we want to do for this
    //2000 is an estimated minimum to get varied enough winrates
    ostringstream oSS;
    oSS << "{\n";
    oSS << "\t\"winrates_vs_heroes\": [\n";
    for (unsigned chosenHero = stoi(startHeroId); chosenHero <= stoi(endHeroId); chosenHero++) {
        vector <bool> winarr;
        winarr = m.findWinLoss();
        vector<tuple<string,string,string>> matcharrange;
        matcharrange = p.getMatchHero();
        vector<string> players_in_match;
        vector<string> slot;
        //helper vector to hold info during calculation.
        vector<pair<double,double>> matches_vs(NUM_HEROES);
        search_vs_winrate_helper(chosenHero, numMatches, startHeroId, endHeroId, modifier, winarr, matcharrange, players_in_match, slot, matches_vs, 0);
        //TODO: Refactor as separate helper function with multiple parameters
        if (modifier) {
            search_vs_winrate_helper(chosenHero, to_string(m.getSize()), startHeroId, endHeroId, modifier, winarr, matcharrange, players_in_match, slot, matches_vs, stoi(limit));
        }
        //dump to stats file here to save us a headache of rerunning.
        cout << "finished computation for hero " << chosenHero << endl;
        vector<pair<string,double>> result;
        //result is a vector with a pair containing a string and double.
        //the string is the hero id that is vs our requested hero id
        //the double is the winrate vs the hero id specified by the string.
        double temp = 0;
        for (unsigned i = 0; i < matches_vs.size(); i++)
        {
            if (matches_vs.at(i).second != 0)
            {
                temp = matches_vs.at(i).first/matches_vs.at(i).second;
                result.push_back(make_pair(to_string(i),temp)); // offset to match displayed hero
            }
        }
        oSS << "\t{\n";
        oSS << "\t\t\"id\": \"";
        if (chosenHero >= 24) { oSS << chosenHero + 1; } // offset match id for frontend (no hero has id 24)
        else { oSS << chosenHero; }
        oSS << "\",\n";
        oSS << "\t\t\"vs\": {\n";
        oSS << "\t\t\t\"heroes\": [";
        if (result.size() == 0) { oSS << "\"N/A\""; }
        for (unsigned i = 0; i < result.size(); i++)
        {
            int temp_id = stoi(result.at(i).first);
            if (temp_id >= 25) { temp_id--; }
            oSS << "\"" << fixed << setprecision(2) << names.at(temp_id) << "\"";
            if (i != (result.size()-1)) { oSS << ","; }
        }
        oSS << "],\n";
        oSS << "\t\t\t\"winrates\": [";
        if (result.size() == 0) { oSS << "\"N/A\""; }
        for (unsigned i = 0; i < result.size(); i++)
        {
            oSS << "\"" << fixed << setprecision(2) << result.at(i).second << "\"";
            if (i != (result.size()-1)) { oSS << ","; }
        }
        oSS << "]\n\t\t}\n";
        oSS << "\t\t}";
        if (chosenHero != stoi(endHeroId)) { oSS << ",\n"; }
        cout << "finished output for hero " << chosenHero << endl;
    }
    oSS << "\n\t]\n}";
    ofstream outFS(out10);
    if (outFS.is_open()) {
        outFS << oSS.str();
    }
    outFS.close();
    time(&end);
    double time_taken = double(end - start);
    cout << "\nTime taken by analytics_search_vs_winrate feature is: " << fixed <<  time_taken << setprecision(5);
    cout << " sec" << endl;
}

void analytics_predict_winrate(string ratingType, string ratingVal) 
{
    cout << "Test 3" << endl;
    time_t start, end;
    cout << "Test 4" << endl;
    time(&start);
    cout << "Test 5" << endl;
    cout << "Begin analytics_predict_winrate...";
    vector<double> winRates;

    ifstream inFS("stat_dump/predict_winrate.txt");
    if (!inFS.is_open()) { inFS.close(); cout << "Something went wrong. The file was corrupt." << endl; }
    if (inFS.peek() == ifstream::traits_type::eof()) // if stat dump file is empty, build regression model
    {
        Submanager_Player_Ratings pr = Submanager_Player_Ratings();
        pr.readFile(pr.fileName);
        winRates = pr.getWinrates();
        
        int n;
        double sumRatings = 0;
        double sumWinRates = 0;
        double sumRatWr = 0;
        double sumRatSq = 0;
    
        vector<double> vMu;
        vMu = pr.getMu();
        n = vMu.size();
        for (int i = 0; i < n; i++) 
        {
            sumRatings += vMu[i];
            sumWinRates += winRates[i];
            sumRatWr += vMu[i] * winRates[i];
            sumRatSq += pow(vMu[i], 2);
        }

        ofstream outFS("stat_dump/predict_winrate.txt");
        if (!outFS.is_open()) { outFS.close(); cout << "Something went wrong. The file was corrupt." << endl; }
        outFS << to_string(sumWinRates) << "\n";
        outFS << to_string(n) << ", " << to_string(sumRatings) << ", " << to_string(sumRatWr) << ", " << to_string(sumRatSq) << "\n";

        sumRatings = 0;
        sumRatWr = 0;
        sumRatSq = 0;
        vector<double> vSigma;
        vSigma = pr.getSigma();
        n = vSigma.size();
        for (int i = 0; i < n; i++) 
        {
            sumRatings += vSigma[i];
            sumRatWr += vSigma[i] * winRates[i];
            sumRatSq += pow(vSigma[i], 2);
        }
        
        outFS << to_string(n) << ", " << to_string(sumRatings) << ", " << to_string(sumRatWr) << ", " << to_string(sumRatSq);
        outFS.close();
    }
    inFS.close(); inFS.clear();

    ifstream inFS2("stat_dump/predict_winrate.txt");
    if (!inFS2.is_open()) { inFS2.close(); cout << "Something went wrong. The file was corrupt." << endl; }

    int numEntries;
    double sumX;
    double sumY;
    double sumXY;
    double sumXSq;

    inFS2 >> sumY; 
    string temp;
    string s;
    cout << "Test 6" << endl;

    if (ratingType == "mu") 
    {
        getline(inFS2, temp);
        getline(inFS2, temp);
        stringstream ss(temp);
        getline(ss, s, ',');
        //cout << "S: " << s << endl;
        numEntries = stoi(s);
        getline(ss, s, ',');
        sumX = stod(s);
        getline(ss, s, ',');
        sumXY = stod(s);
        getline(ss, s, ',');
        sumXSq = stod(s);
    }
    else if (ratingType == "sigma")
    {
        getline(inFS2, temp);
        getline(inFS2, temp);
        getline(inFS2, temp);
        stringstream ss(temp);
        getline(ss, s, ',');
        numEntries = stoi(s);
        getline(ss, s, ',');
        sumX = stod(s);
        getline(ss, s, ',');
        sumXY = stod(s);
        getline(ss, s, ',');
        sumXSq = stod(s);
    }
    else
    {
        cout << "Error: invalid rating type" << endl;
        return;
    }
    inFS2.close(); inFS2.clear();

    double slope = ((numEntries * sumXY) - (sumX * sumY)) / ((numEntries * sumXSq) - pow(sumX, 2));
    cout << "Num Entries: " << numEntries << endl;
    cout << "sumX: " << sumX << endl;
    cout << "sumY: " << sumY << endl;
    cout << "sumXY: " << sumXY << endl;
    cout << "sumXSq: " << sumXSq << endl;
    cout << "Slope: " << slope << endl;
    double y_int = (sumY - (slope * sumX)) / numEntries;
    cout << "y_int: " << y_int << endl;
    double predWinRate = y_int + (stod(ratingVal) * slope);
    cout << "predWinRate: " << predWinRate << endl;
    string pwr = to_string(predWinRate);

    ostringstream oSS;
    oSS << "{\n";
    oSS << "\t\"player_rating\": [\n";
    oSS << "\t{\n";
    oSS << "\t\t\"trueskill_";
    if (ratingType == "mu") { oSS << "mu\": "; }
    if (ratingType == "sigma") { oSS << "sigma\": "; }
    oSS << "\"" << ratingVal << "\",\n";
    oSS << "\t\t\"predicted_winrate\": " << pwr << "\n";
    oSS << "\t}\n";
    oSS << "\t]\n";
    oSS << "}";
    ofstream outFS(out12);
    if (outFS.is_open()) { outFS << oSS.str(); }
    outFS.close(); outFS.clear();

    time(&end);
    double time_taken = double (end-start);
    cout << "\nTime taken by analytics_predict_winrate feature is: " << fixed << time_taken << setprecision(5);
    cout << " sec" << endl;
}   

void insert_hero(const std::string parameter[], bool &modifier, int &mod_cnt, bool &update, std::vector<std::vector<std::string>> &tmp_new)
{
    int id = 1;
    string name = (parameter[0]);
    string win = (parameter[1]);
    string pick = (parameter[2]);
    ifstream inFS;
    string tempLine;
    vector<string> temp_ids;
    unsigned size = 0;
    if (modifier) { inFS.open("../../Temporary_Data/hero_stats.txt", ifstream::in); }
    else { inFS.open("./stat_dump/hero_stats.txt", ifstream::in); }
    if (!inFS.is_open()) { inFS.close(); cout << "Something went wrong.  The file was corrupt." << endl; }
    while (getline(inFS, tempLine))
    {
        stringstream ss(tempLine);
        string value;
        for (unsigned i = 0; i < 4; i++)
        {
            if (getline(ss, value, ',')) { if (i == 0) { temp_ids.push_back(value); } }
        }
        size++;
    }
    inFS.close(); inFS.clear();
    id += stoi(temp_ids.at(temp_ids.size()-1));
    ofstream outfile(out7);
    if (outfile.is_open()) { outfile << "{ " << "\"results\": [ { \"result\" : \"" << true << "\" } ] }"; }
    outfile.close(); outfile.clear();

    if (!modifier) // import original data into temporary data if no modifications made
    {
        ifstream src("./stat_dump/hero_stats.txt", ios::in | ios::binary);
        ofstream dst("../../Temporary_Data/hero_stats.txt", ios::out | ios::binary);
        dst << src.rdbuf();
        src.close(); src.clear();
        dst.close(); dst.clear();
    }

    ofstream outFS("../../Temporary_Data/hero_stats.txt", ios_base::app);
    if (!outFS.is_open()) { outFS.close(); cout << "Something went wrong.  The file was corrupt." << endl; }
    string new_hero = to_string(id) + "," + name + "," + to_string(stod(win)/100) + "," + pick;
    tmp_new.at(1).push_back(new_hero);
    outFS << new_hero << endl;
    outFS.close(); outFS.clear();
    modifier = true; 
    mod_cnt++;
    //TODO: check if insertions go above certain amount then set updateSearch = true
    update = true;
}

void insert_player(const std::string parameter[], bool &modifier, bool &update, std::vector<std::vector<std::string>> &tmp_new)
{
    Submanager_Player_Ratings pr = Submanager_Player_Ratings();
    int player_id = stoi(parameter[0]);
    string win_cnt = parameter[1];
    string match_cnt = parameter[2];
    string ts_mu = parameter[3];
    string ts_sigma = parameter[4];
    if (!modifier)
    {
        ifstream src(pr.fileName, ios::in | ios::binary);
        ofstream dst(pr.fileName2, ios::out | ios::binary);
        dst << src.rdbuf();
        src.close(); src.clear();
        dst.close(); dst.clear();
    }
    ofstream outFS(pr.fileName2, ios_base::app);
    if (!outFS.is_open()) { outFS.close(); cout << "Something went wrong.  The file was corrupt." << endl; }
    string new_pr = to_string(player_id) + "," + win_cnt + "," + match_cnt + "," + ts_mu + "," + ts_sigma;
    tmp_new.at(2).push_back(new_pr);
    outFS << new_pr << endl;
    outFS.close(); outFS.clear();
    {
        pr.incremental_analytics_insert(win_cnt, match_cnt, ts_mu, ts_sigma); // incremental analytics
        cout << "check3" << endl;
    }
    modifier = true; 
    //TODO: check if insertions go above certain amount then set updateSearch = true
    update = true;
}

void insert_match(const std::string parameter[], bool &modifier, int &mod_cnt, bool &updateFB, bool &updateCR, bool &updateML, std::vector<std::vector<std::string>> &tmp_new)
{
    string match_id = (parameter[0]);
    string start_time = (parameter[1]); // not used
    string duration = (parameter[2]); // not used
    string tower_status_radiant = (parameter[3]); // not used
    string tower_status_dire = (parameter[4]); // not used
    string barracks_status_dire = (parameter[5]); // not used
    string barracks_status_radiant = (parameter[6]); // not used
    string first_blood_time = (parameter[7]);
    string game_mode = "22"; // not used
    string radiant_win = (parameter[9]); 
    string negative_votes = (parameter[10]);
    string positive_votes = (parameter[11]);
    string cluster = (parameter[12]);
    {
        Submanager_Match subMat = Submanager_Match();
        subMat.readFile(subMat.fileName);
        if (!subMat.addMatch(match_id, start_time, duration, tower_status_radiant, tower_status_dire, 
            barracks_status_dire, barracks_status_radiant, first_blood_time, game_mode, radiant_win, negative_votes, positive_votes, cluster)) 
        {
            cout << "Match information not valid!\n";
            ofstream outfile(out7);
            if (outfile.is_open()) { outfile << "{ \"results\": [ { \"result\": \"" << true << "\"" << "} ] }"; }
            outfile.close(); outfile.clear();
        }
        else
        {
            // temporary file to store new matches (for demo purposes)
            tmp_new.at(3).push_back(match_id);
            subMat.writeFile(stoi(parameter[13]), 'o');
            ofstream outfile(out7);
            if (outfile.is_open()) { outfile << "{ \"results\": [ { \"result\": \"" << true << "\"" << "} ] }"; }
            outfile.close(); outfile.clear();
            modifier = true; 
            mod_cnt++;
            //TODO: check if insertions go above certain amount then set updateSearch = true
            updateFB = true; updateCR = true; updateML = true;
        }
    }
}
//FEATURE: update_ability_popularity
void update_ability(const std::string parameter[], bool &modifier, bool &update)
{
    string ab_id = parameter[0];
    string upgrade_cnt = parameter[1];
    ifstream inFS;
    string tempLine;
    vector<string> temp_ids, temp_upgrades;
    bool updated = false;
    unsigned size = 0;
    if (modifier) { inFS.open("../../Temporary_Data/ability_upgrade_popularity.txt", ifstream::in); }
    else { inFS.open("./stat_dump/ability_upgrade_popularity.txt", ifstream::in); }
    if (!inFS.is_open()) { inFS.close(); cout << "Something went wrong.  The file was corrupt." << endl; }
    while (getline(inFS, tempLine))
    {
        stringstream ss(tempLine);
        string value;
        for (unsigned i = 0; i < 2; i++)
        {
            if (getline(ss, value, ','))
            {
                if (i == 0) 
                { 
                    if (value == ab_id) { updated = true; }
                    temp_ids.push_back(value); 
                }
                else if (i == 1) { 
                    if (updated) { temp_upgrades.push_back(upgrade_cnt); }
                    else { temp_upgrades.push_back(value); }
                }
            }
        }
        updated = false;
        size++;
    }
    inFS.close(); inFS.clear();

    if (!modifier) // import original data into temporary data if no modifications made
    {
        ifstream src("../../Data/ability_ids.csv", ios::in | ios::binary);
        ofstream dst("../../Temporary_Data/ability_ids.csv", ios::out | ios::binary);
        dst << src.rdbuf();
        src.close(); src.clear();
        dst.close(); dst.clear();
    }
    ofstream outfile(out7);
    if (outfile.is_open()) { outfile << "{ " << "\"results\": [ { \"result\" : \"" << true << "\" } ] }"; }
    outfile.close(); outfile.clear();
    ofstream outFS("../../Temporary_Data/ability_upgrade_popularity.txt");
    if (!outFS.is_open()) { outFS.close(); cout << "Something went wrong.  The file was corrupt." << endl; }

    // resort ability upgrades
    vector<pair<int,int>> resorted;
    for (int i = 0; i < size - 1; i++) { resorted.push_back(make_pair(stoi(temp_ids.at(i)),stoi(temp_upgrades.at(i)))); }
    sort(resorted.begin(), resorted.end(), sortbysecdesc);
    for (int i = 0; i < size - 1; i++) { outFS << resorted.at(i).first << "," << resorted.at(i).second << endl; } 
    outFS.close(); outFS.clear();
    modifier = true; update = true;
}

void update_hero(const std::string parameter[], bool &modifier, bool &update, std::vector<std::vector<std::string>> &tmp_new)
{
    string hero_id = (parameter[0]);
    string win = (parameter[1]);
    string pick = (parameter[2]);
    ifstream inFS;
    string tempLine;
    vector<string> temp_ids, temp_names, temp_win, temp_pick;
    string original = ""; // to store original statistics
    string newStats = hero_id + ",";
    bool updated = false;
    bool result = false; // check if there was even a hero_id to update
    unsigned size = 0;
    if (modifier) { inFS.open("../../Temporary_Data/hero_stats.txt", ifstream::in); }
    else { inFS.open("./stat_dump/hero_stats.txt", ifstream::in); }
    if (!inFS.is_open()) { inFS.close(); cout << "Something went wrong.  The file was corrupt." << endl; }
    while (getline(inFS, tempLine))
    {
        stringstream ss(tempLine);
        string value;
        for (unsigned i = 0; i < 4; i++)
        {
            if (getline(ss, value, ','))
            {
                if (i == 0)
                {
                    if (value == hero_id) { original = tempLine; updated = true; result = true; }
                    temp_ids.push_back(value);
                }
                else if (i == 1) 
                { 
                    if (updated) { newStats += value + ","; }
                    temp_names.push_back(value); 
                }
                else if (i == 2)
                {
                    if (updated) { newStats += to_string(stod(win)/100) + ","; temp_win.push_back(to_string(stod(win)/100)); }
                    else { temp_win.push_back(value); }
                }
                else if (i == 3)
                {
                    if (updated) { newStats += pick; temp_pick.push_back(pick); }
                    else { temp_pick.push_back(value); }
                }
            }
        }
        updated = false;
        size++;
    }
    inFS.close(); inFS.clear();
    // also update values in temporary display
    if (modifier) { for (unsigned i = 0; i < tmp_new.at(1).size(); i++) { if (tmp_new.at(1).at(i) == original) { tmp_new.at(1).at(i) = newStats; } } }
    if(!result) 
    {
        ofstream outfile(out7);
        if (outfile.is_open()) { outfile << "{ " << "\"results\": [ { \"result\" : \"" << false << "\" } ] }"; }
        outfile.close(); outfile.clear();
    }
    else
    {
        ofstream outfile(out7);
        if (outfile.is_open()) { outfile << "{ " << "\"results\": [ { \"result\" : \"" << true << "\" } ] }"; }
        outfile.close(); outfile.clear();
        ofstream outFS("../../Temporary_Data/hero_stats.txt");
        if (!outFS.is_open()) { outFS.close(); cout << "Something went wrong.  The file was corrupt." << endl; }
        for (int i = 0; i < size - 1; i++)
        {
            outFS << temp_ids.at(i) << "," 
                    << temp_names.at(i) << "," 
                    << temp_win.at(i) << "," 
                    << temp_pick.at(i) << endl; 
        } 
        outFS.close(); outFS.clear();
        modifier = true; update = true;
    }
}

void update_player(const std::string parameter[], bool &modifier, bool &update, std::vector<std::vector<std::string>> &tmp_new)
{
    Submanager_Player_Ratings pr = Submanager_Player_Ratings();
    int player_id = stoi(parameter[0]);
    string win_cnt = parameter[1];
    string match_cnt = parameter[2];
    string ts_mu = parameter[3];
    string ts_sigma = parameter[4];

    if (!modifier)
    {
        ifstream src(pr.fileName, ios::in | ios::binary);
        ofstream dst(pr.fileName2, ios::out | ios::binary);
        dst << src.rdbuf();
        src.close(); src.clear();
        dst.close(); dst.clear();
    }

    ifstream inFS;
    string tempLine;
    vector<string> temp_ids, temp_wins, temp_matches, temp_mu, temp_sigma;
    string original = ""; // to store original statistics
    vector<string> oldStats;
    string newStats = to_string(player_id) + ",";
    bool updated = false;
    unsigned size = 0;
    if (modifier) { inFS.open(pr.fileName2, ifstream::in); }
    else { inFS.open(pr.fileName, ifstream::in); }
    if (!inFS.is_open()) { inFS.close(); cout << "Something went wrong.  The file was corrupt." << endl; }
    while (getline(inFS, tempLine))
    {
        stringstream ss(tempLine);
        string value;
        for (unsigned i = 0; i < 5; i++)
        {
            if (getline(ss, value, ','))
            {
                if (i == 0)
                {
                    if (value == to_string(player_id)) { original = tempLine; updated = true; }
                    temp_ids.push_back(value);
                }
                else if (i == 1) 
                { 
                    if (updated) { oldStats.push_back(value); newStats += win_cnt + ","; temp_wins.push_back(win_cnt); }
                    else { temp_wins.push_back(value); }
                }
                else if (i == 2)
                {
                    if (updated) { oldStats.push_back(value); newStats += match_cnt + ","; temp_matches.push_back(match_cnt); }
                    else { temp_matches.push_back(value); }
                }
                else if (i == 3)
                {
                    if (updated) { oldStats.push_back(value); newStats += ts_mu + ","; temp_mu.push_back(ts_mu); }
                    else { temp_mu.push_back(value); }
                }
                else if (i == 4)
                {
                    if (updated) { oldStats.push_back(value); newStats += ts_sigma; temp_sigma.push_back(ts_sigma); }
                    else { temp_sigma.push_back(value); }
                }
            }
        }
        updated = false;
        size++;
    }
    inFS.close(); inFS.clear();
    // also update values in temporary display
    if (modifier) { for (unsigned i = 0; i < tmp_new.at(2).size(); i++) { if (tmp_new.at(2).at(i) == original) { tmp_new.at(2).at(i) = newStats; } } }
    ofstream outfile(out7);
    if (outfile.is_open()) { outfile << "{ " << "\"results\": [ { \"result\" : \"" << true << "\" } ] }"; }
    outfile.close(); outfile.clear();
    ofstream outFS(pr.fileName2);
    if (!outFS.is_open()) { outFS.close(); cout << "Something went wrong.  The file was corrupt." << endl; }
    for (int i = 0; i < size - 1; i++)
    {
        outFS << temp_ids.at(i) << "," 
                << temp_wins.at(i) << "," 
                << temp_matches.at(i) << "," 
                << temp_mu.at(i) << ","
                << temp_sigma.at(i) << endl; 
    } 
    outFS.close(); outFS.clear();
    {
        pr.incremental_analytics_insert(win_cnt, match_cnt, ts_mu, ts_sigma); // incremental analytics
        pr.incremental_analytics_delete(oldStats[0], oldStats[1], oldStats[2], oldStats[3]); // incremental analytics
    }
    modifier = true; update = true;
}

void update_match(const std::string parameter[], bool &modifier, bool &updateFB, bool &updateCR, bool &updateML)
{
    string match_id = (parameter[0]);
    string start_time = (parameter[1]); // not used
    string duration = (parameter[2]); // not used
    string tower_status_radiant = (parameter[3]); // not used
    string tower_status_dire = (parameter[4]); // not used
    string barracks_status_dire = (parameter[5]); // not used
    string barracks_status_radiant = (parameter[6]); // not used
    string first_blood_time = (parameter[7]);
    string game_mode = "22"; // not used
    string radiant_win = (parameter[9]);
    string negative_votes = (parameter[10]);
    string positive_votes = (parameter[11]);
    string cluster = (parameter[12]);
    {
        Submanager_Match subMat = Submanager_Match();
        if (modifier) { subMat.readFile(subMat.fileName2); }
        else { subMat.readFile(subMat.fileName); }
        if (!subMat.updateMatch(match_id, start_time, duration, tower_status_radiant, tower_status_dire, 
            barracks_status_dire, barracks_status_radiant, first_blood_time, game_mode, radiant_win, negative_votes, positive_votes, cluster)) 
        {
            cout << "Match information not valid!\n";
            ofstream outfile(out7);
            if (outfile.is_open()) { outfile << "{ \"results\": [ { \"result\": \"" << false << "\"" << "} ] }"; }
            outfile.close(); outfile.clear();
        }
        else
        {
            ofstream outfile(out7);
            if (outfile.is_open()) { outfile << "{ \"results\": [ { \"result\": \"" << true << "\"" << "} ] }"; }
            outfile.close(); outfile.clear();
            subMat.writeFile(stoi(parameter[13]), 'o');
            modifier = true; 
            updateFB = true; updateCR = true; updateML = true;
        }
    }
}

void delete_ability(std::string id, bool &modifier, bool flag, bool &update, std::vector<std::vector<std::string>> &tmp_new)
{
    ability ab = ability();
    if (modifier) { ab.readFile(ab.fileName2); } // read from temporary_data csv
    else { ab.readFile(ab.fileName); } // read from original csv
    bool result = ab.delete_ability(stoi(id)); 
    ifstream inFS;
    string tempLine;
    vector<string> temp_ids, temp_upgrades;
    bool deleted = false;
    unsigned size = 0;

    if (modifier) { inFS.open("../../Temporary_Data/ability_upgrade_popularity.txt", ifstream::in); }
    else { inFS.open("./stat_dump/ability_upgrade_popularity.txt", ifstream::in); }
    if (!inFS.is_open()) { inFS.close(); cout << "Something went wrong.  The file was corrupt." << endl; }
    while (getline(inFS, tempLine))
    {
        stringstream ss(tempLine);
        string value;
        for (unsigned i = 0; i < 2; i++)
        {
            if (getline(ss, value, ','))
            {
                if (i == 0) 
                { 
                    if (value == id) { deleted = true; }
                    else { temp_ids.push_back(value); }
                }
                if (!deleted && i == 1) { 
                    temp_upgrades.push_back(value); 
                }
            }
        }
        deleted = false;
        size++;
    }
    inFS.close(); inFS.clear();
    if(!result) 
    {
        ofstream outfile(out7);
        if (outfile.is_open()) { outfile << "{ " << "\"results\": [ { \"result\" : \"" << false << "\" } ] }"; }
        outfile.close(); outfile.clear();
    }
    else
    {
        ofstream outFS("../../Temporary_Data/ability_upgrade_popularity.txt");
        if (!outFS.is_open()) { outFS.close(); cout << "Something went wrong.  The file was corrupt." << endl; }
        for (unsigned i = 0; i < size - 1; i++) { outFS << temp_ids.at(i) << "," << temp_upgrades.at(i) << endl; }
        outFS.close(); outFS.clear();
        ofstream outfile(out7);
        if (outfile.is_open()) { outfile << "{ " << "\"results\": [ { \"result\" : \"" << true << "\" } ] }"; }
        outfile.close(); outfile.clear();
        ab.writeFile(flag, 'o');
        modifier = true; update = true;
    }
}

void delete_hero(std::string id, bool &modifier, bool &update, std::vector<std::vector<std::string>> &tmp_new)
{
    ifstream inFS;
    string tempLine;
    vector<string> temp_ids, temp_names, temp_win, temp_pick;
    bool deleted = false;
    bool result = false; // check if there was even a hero_id to delete
    unsigned size = 0;
    if (modifier) { inFS.open("../../Temporary_Data/hero_stats.txt", ifstream::in); }
    else { inFS.open("./stat_dump/hero_stats.txt", ifstream::in); }
    if (!inFS.is_open()) { inFS.close(); cout << "Something went wrong.  The file was corrupt." << endl; }
    while (getline(inFS, tempLine))
    {
        stringstream ss(tempLine);
        string value;
        for (unsigned i = 0; i < 4; i++)
        {
            if (getline(ss, value, ','))
            {
                if (i == 0)
                {
                    if (value == id) { deleted = true; result = true; }
                    else { temp_ids.push_back(value); }
                }
                if (!deleted)
                {
                    if (i == 1) { temp_names.push_back(value); }
                    else if (i == 2) { temp_win.push_back(value); }
                    else if (i == 3) { temp_pick.push_back(value); }
                }
            }
        }
        deleted = false;
        size++;
    }
    inFS.close(); inFS.clear();
    if(!result) 
    {
        ofstream outfile(out7);
        if (outfile.is_open()) { outfile << "{ " << "\"results\": [ { \"result\" : \"" << false << "\" } ] }"; }
        outfile.close(); outfile.clear();
    }
    else
    {
        ofstream outfile(out7);
        if (outfile.is_open()) { outfile << "{ " << "\"results\": [ { \"result\" : \"" << true << "\" } ] }"; }
        outfile.close(); outfile.clear();
        ofstream outFS("../../Temporary_Data/hero_stats.txt");
        if (!outFS.is_open()) { outFS.close(); cout << "Something went wrong.  The file was corrupt." << endl; }
        for (int i = 0; i < size - 1; i++)
        {
            outFS << temp_ids.at(i) << "," 
                    << temp_names.at(i) << "," 
                    << temp_win.at(i) << "," 
                    << temp_pick.at(i) << endl; 
        } 
        outFS.close(); outFS.clear();
        modifier = true; update = true;
    }

    // also delete from temporary display
    if (tmp_new.at(1).size() > 0)
    {
        int del_index = 0;
        bool found = false;
        for (unsigned i = 0; i < tmp_new.at(1).size(); i++)
        {
            stringstream newSS(tmp_new.at(1).at(i));
            string val;
            for (unsigned k = 0; k < 4; k++) 
            { 
                if (getline(newSS,val,',') && k == 0 && val == id) 
                { 
                    found = true;
                    del_index = i;
                    break;
                }
            }
        }
        if (found) { tmp_new.at(1).erase(tmp_new.at(1).begin() + del_index); }
    }
}

void delete_player(std::string id, bool &modifier, bool &update, std::vector<std::vector<std::string>> &tmp_new)
{
    Submanager_Player_Ratings pr = Submanager_Player_Ratings();                
    ifstream inFS;
    string tempLine;
    vector<string> temp_ids, temp_wins, temp_matches, temp_mu, temp_sigma;
    vector<string> oldStats;
    bool deleted = false;
    unsigned size = 0;
    if (modifier) { inFS.open(pr.fileName2, ifstream::in); }
    else { inFS.open(pr.fileName, ifstream::in); }
    if (!inFS.is_open()) { inFS.close(); cout << "Something went wrong.  The file was corrupt." << endl; }
    while (getline(inFS, tempLine))
    {
        stringstream ss(tempLine);
        string value;
        for (unsigned i = 0; i < 5; i++)
        {
            if (getline(ss, value, ','))
            {
                if (i == 0) 
                { 
                    if (value == id) { deleted = true; }
                    else { temp_ids.push_back(value); }
                }
                if (!deleted) 
                { 
                    if (i == 1) { temp_wins.push_back(value); }
                    else if (i == 2) { temp_matches.push_back(value); }
                    else if (i == 3) { temp_mu.push_back(value); }
                    else if (i == 4) { temp_sigma.push_back(value); }
                }
                else { oldStats.push_back(value); }
            }
        }
        deleted = false;
        size++;
    }
    inFS.close(); inFS.clear();
    ofstream outFS(pr.fileName2);
    if (!outFS.is_open()) { outFS.close(); cout << "Something went wrong.  The file was corrupt." << endl; }
    for (unsigned i = 0; i < size - 1; i++) { outFS << temp_ids.at(i) << "," << temp_wins.at(i) << "," << temp_matches.at(i) << "," << temp_mu.at(i) << "," << temp_sigma.at(i) << endl; }
    outFS.close(); outFS.clear();
    ofstream outfile(out7);
    if (outfile.is_open()) { outfile << "{ " << "\"results\": [ { \"result\" : \"" << true << "\" } ] }"; }
    outfile.close(); outfile.clear();
    {
        pr.incremental_analytics_delete(oldStats[0], oldStats[1], oldStats[2], oldStats[3]); // incremental analytics
    }
    modifier = true; update = true;
    // also delete from temporary display
    if (tmp_new.at(2).size() > 0)
    {
        int del_index = 0;
        bool found = false;
        for (unsigned i = 0; i < tmp_new.at(2).size(); i++)
        {
            stringstream newSS(tmp_new.at(2).at(i));
            string val;
            for (unsigned k = 0; k < 5; k++) 
            { 
                if (getline(newSS,val,',') && k == 0 && val == id) 
                { 
                    found = true;
                    del_index = i;
                    break;
                }
            }
        }
        if (found) { tmp_new.at(2).erase(tmp_new.at(2).begin() + del_index); }
    }
}

void delete_match(std::string id, bool &modifier, bool flag, bool &updateFB, bool &updateCR, bool &updateML, std::vector<std::vector<std::string>> &tmp_new)
{
    // string ab_name = parameter[1];
    Submanager_Match m = Submanager_Match();
    if (modifier) { m.readFile(m.fileName2); } // read from temporary_data csv
    else { m.readFile(m.fileName); } // read from original csv

    bool result = m.deleteMatch(id); 
    if(!result) 
    {
    ofstream outfile(out7);
    if (outfile.is_open()) { outfile << "{ " << "\"results\": [ { \"result\" : \"" << false << "\" } ] }"; }
    outfile.close(); outfile.clear();
    }
    else
    {
    ofstream outfile(out7);
    if (outfile.is_open()) { outfile << "{ " << "\"results\": [ { \"result\" : \"" << true << "\" } ] }"; }
    outfile.close(); outfile.clear();
    m.writeFile(flag, 'o');
    modifier = true; 
    updateFB = true; updateCR = true; updateML = true;
    }

    // also delete from temporary display
    if (tmp_new.at(3).size() > 0)
    {
    int del_index = 0;
    for (unsigned i = 0; i < tmp_new.at(3).size(); i++)
    {
        if (tmp_new.at(3).at(i) == id) { del_index = i; }
    }
    tmp_new.at(3).erase(tmp_new.at(3).begin() + del_index);
    }
}

void backup_ability(bool modified, bool &backup)
{
    ability ob = ability();
    if(!modified) 
    {
        // copy original file into saved directory (backup)
        ifstream src(ob.fileName, ios::in | ios::binary);
        ofstream dst(ob.fileName3, ios::out | ios::binary);
        dst << src.rdbuf();
        src.close(); src.clear();
        dst.close(); dst.clear();
        
        src.open("./stat_dump/ability_upgrade_popularity.txt", ios::in | ios::binary);
        dst.open("../../Backup_Data/ability_upgrade_popularity.txt", ios::out | ios::binary);
        dst << src.rdbuf();
        src.close(); src.clear();
        dst.close(); dst.clear();
    }
    else
    {
        // copy modified file into saved directory (backup)
        ifstream src(ob.fileName2, ios::in | ios::binary);
        ofstream dst(ob.fileName3, ios::out | ios::binary);
        dst << src.rdbuf();
        src.close(); src.clear();
        dst.close(); dst.clear();
        
        src.open("../../Temporary_Data/ability_upgrade_popularity.txt", ios::in | ios::binary);
        dst.open("../../Backup_Data/ability_upgrade_popularity.txt", ios::out | ios::binary);
        dst << src.rdbuf();
        src.close(); src.clear();
        dst.close(); dst.clear();
    }
    // backup successful
    backup = true;
    ofstream outfile(out8);
    outfile << "{ \"results\": [ { \"result\": \"" << true << "\"" << "} ] }";
    outfile.close(); outfile.clear();
}

void backup_hero(bool modified, bool &backup)
{
    if(!modified) 
    {
        // copy original file into saved directory (backup)
        ifstream src("./stat_dump/hero_stats.txt", ios::in | ios::binary);
        ofstream dst("../../Backup_Data/hero_stats.txt", ios::out | ios::binary);
        dst << src.rdbuf();
        src.close(); src.clear();
        dst.close(); dst.clear();
    }
    else
    {
        // copy modified file into saved directory (backup)
        ifstream src("../../Temporary_Data/hero_stats.txt", ios::in | ios::binary);
        ofstream dst("../../Backup_Data/hero_stats.txt", ios::out | ios::binary);
        dst << src.rdbuf();
        src.close(); src.clear();
        dst.close(); dst.clear();
    }
    // backup successful
    backup = true;
    ofstream outfile(out8);
    outfile << "{ \"results\": [ { \"result\": \"" << true << "\"" << "} ] }";
    outfile.close(); outfile.clear();
}

void backup_player(bool modified, bool &backup)
{
    Submanager_Player_Ratings pr = Submanager_Player_Ratings();
    if(!modified) 
    {
        // copy original file into saved directory (backup)
        ifstream src(pr.fileName, ios::in | ios::binary);
        ofstream dst(pr.fileName3, ios::out | ios::binary);
        dst << src.rdbuf();
        src.close(); src.clear();
        dst.close(); dst.clear();
    }
    else
    {
        // copy modified file into saved directory (backup)
        ifstream src(pr.fileName2, ios::in | ios::binary);
        ofstream dst(pr.fileName3, ios::out | ios::binary);
        dst << src.rdbuf();
        src.close(); src.clear();
        dst.close(); dst.clear();
    }
    // backup successful
    backup = true;
    ofstream outfile(out8);
    outfile << "{ \"results\": [ { \"result\": \"" << true << "\"" << "} ] }";
    outfile.close(); outfile.clear();
}

void backup_match(bool modified, bool &backup)
{
    Submanager_Match m = Submanager_Match();
    if(!modified) 
    {
        // copy original file into saved directory (backup)
        ifstream src(m.fileName, ios::in | ios::binary);
        ofstream dst(m.fileName3, ios::out | ios::binary);
        dst << src.rdbuf();
        src.close(); src.clear();
        dst.close(); dst.clear();
    }
    else
    {
        // copy modified file into saved directory (backup)
        ifstream src(m.fileName2, ios::in | ios::binary);
        ofstream dst(m.fileName3, ios::out | ios::binary);
        dst << src.rdbuf();
        src.close(); src.clear();
        dst.close(); dst.clear();
    }
    // backup successful
    backup = true;
    ofstream outfile(out8);
    outfile << "{ \"results\": [ { \"result\": \"" << true << "\"" << "} ] }";
    outfile.close(); outfile.clear();
}

void import_ability(bool backup, bool &update)
{
    ability ob = ability();
    if (backup)
    {
        // copy file from saved directory into temp (import)
        ifstream src(ob.fileName3, ios::in | ios::binary);
        ofstream dst(ob.fileName2, ios::out | ios::binary);
        dst << src.rdbuf();
        src.close(); src.clear();
        dst.close(); dst.clear();
        
        src.open("../../Backup_Data/ability_upgrade_popularity.txt", ios::in | ios::binary);
        dst.open("../../Temporary_Data/ability_upgrade_popularity.txt", ios::out | ios::binary);
        dst << src.rdbuf();
        src.close(); src.clear();
        dst.close(); dst.clear();
        update = true;
    }
}

void import_hero(bool backup, bool &update) 
{
    if (backup)
    {
        // copy file from saved directory into temp (import)
        ifstream src("../../Backup_Data/hero_stats.txt", ios::in | ios::binary);
        ofstream dst("../../Temporary_Data/hero_stats.txt", ios::out | ios::binary);
        dst << src.rdbuf();
        src.close(); src.clear();
        dst.close(); dst.clear();
        update = true;
    }
}

void import_player(bool backup, bool &update) 
{
    Submanager_Player_Ratings pr = Submanager_Player_Ratings();
    if (backup)
    {
        // copy file from saved directory into temp (import)
        ifstream src(pr.fileName3, ios::in | ios::binary);
        ofstream dst(pr.fileName2, ios::out | ios::binary);
        dst << src.rdbuf();
        src.close(); src.clear();
        dst.close(); dst.clear();
        update = true;
    }
}

void import_match(bool backup, bool &updateFB, bool &updateCR, bool &updateML) 
{
    Submanager_Match m = Submanager_Match();
    if (backup)
    {
        // copy file from saved directory into temp (import)
        ifstream src(m.fileName3, ios::in | ios::binary);
        ofstream dst(m.fileName2, ios::out | ios::binary);
        dst << src.rdbuf();
        src.close(); src.clear();
        dst.close(); dst.clear();
        updateFB = true; updateCR = true; updateML = true;
    }
}

void add_ability_upgrades(const std::string parameter[]){
    string ability_id = (parameter[0]);
    string check = "";
    {
        ability ob1 = ability();
        ob1.readFile(ob1.fileName);
        check = ob1.get_ability_name(stoi(ability_id));
        //cout << check << endl;
    }
    if(check == "NOT FOUND") 
    { 
        cout << "Ability ID not valid!" << endl; 
        ofstream outfile(out7);
        if (outfile.is_open()) 
        { 
            outfile << "{\n";
            outfile << "    \"results\": [\n";
            outfile << "    \"result\": {" << "\"" << false << "\"" << "}\n               ]\n}"<< endl;
        }
        outfile.close();
    }
    else
    {
        //cout << "1" << endl; //debug
        string level = parameter[1];
        string time = parameter[2];
        string player_slot = parameter[3];
        string match_id = parameter[4];
        ability_upgrades ob2 = ability_upgrades();
        ob2.readFile(ob2.fileName);
        bool result = ob2.addUpgrade(ability_id, level, time, player_slot, match_id);
        fstream thing;
        thing.open("stat_dump/ability_upgrade_popularity.txt", ios::in | ios::out);
        string line = "";
        bool flag = false;
        cout << "Loaded" << endl;
        if(!thing.is_open()) { exit(1); thing.close(); }
        else
        {
            cout << "File opened" << endl;
            while(getline(thing, line))
            {
                cout << "Running" << endl;
                stringstream ss(line);
                string data1,data2;
                //keep track of size of line
                int prevLineSize = line.length();
                //cout << "prev line size = " << prevLineSize << endl;
                //get current file position
                streampos oldPos = thing.tellg(); 
                if(getline(ss, data1, ',') && getline(ss, data2))
                {
                    cout << "data1 = " << data1 << " & data2 = " << data2 << endl; //debug prints every single line before the AB_ID is seen
                    if(data1 == ability_id)
                    {
                        cout << "data1 = " << data1 << " & data2 = " << data2 << endl; 
                        int temp = stoi(data2);
                        temp++;
                        data2 = to_string(temp);
                        cout << "data1 = " << data1 << " & data2 = " << data2 << endl; 
                        if(oldPos > thing.beg || oldPos < thing.beg) { oldPos -= prevLineSize + 1; }
                        thing.seekp(oldPos);
                        stringstream tempSS;
                        tempSS << data1.c_str() << ',' << data2.c_str();
                        // thing << data1.c_str() << ',' << data2.c_str();
                        thing.write(tempSS.str().c_str(), tempSS.str().size());
                        flag = true;
                    }
                }
                if(flag)break;
            }
            if(!flag) { cout << "Abilitiy ID not found.  Cannot make changes to stat file." << endl; }
        }
        thing.close(); thing.clear();
        ofstream outfile(out7);
        if (outfile.is_open()) 
        { 
            outfile << "{\n";
            outfile << "    \"results\": [\n";
            outfile << "    \"result\": {" << "\"" << result << "\"" << "}\n               ]\n}"<< endl;
        }
        outfile.close();
        //cout << "2" << endl; //debug
        }
}