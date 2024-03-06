#include "submanager_playerRatings.h"

using namespace std;

void Submanager_Player_Ratings::readFile(string text){
    ifstream infile;
    infile.open(text);
    string fileTemp;
    string account_id, total_wins, total_matches;
    if (infile.is_open()) {
        getline(infile,fileTemp);
        while (getline(infile, fileTemp)) {
            stringstream ss(fileTemp);
            string value;
            for (unsigned i = 0; i < PRCOLS; i++) {
                if (getline(ss,value,',')) {
                    pratings_data[i].push_back(value);
                }
            }
        }
    }
    infile.close();
}

void Submanager_Player_Ratings::writeFile(string text) {
    int colCount = 0;
    ofstream outfile(text);
    stringstream output;
    if (outfile.is_open()) {
        for (unsigned row = 0; row < pratings_data[0].size(); row++) {
            for (unsigned col = 0; col < PRCOLS - 1; col++) {
                output << pratings_data[col].at(row) << ',';
            }
            output << pratings_data[PRCOLS - 1].at(row) << "\n";
        }
    }
    outfile << output.rdbuf();
    output.str("");
    outfile.close(); outfile.clear();
}

int Submanager_Player_Ratings::getTotalWins(int account_id) {
    vector<string> temp = pratings_data[0];
    for (unsigned i = 0; i < temp.size(); i++) {
        int tmp = stoi(pratings_data[0].at(i));
        if (tmp == account_id) {
            return stoi(pratings_data[1].at(i));
        }
    }
    return 0;
}

int Submanager_Player_Ratings::getTotalMatches(int account_id) {
    vector<string> temp = pratings_data[0];
    for (unsigned i = 0; i < temp.size(); i++) {
        int tmp = stoi(pratings_data[0].at(i));
        if (tmp == account_id) {
            return stoi(pratings_data[2].at(i));
        }
    }
    return 0;
}

vector<string> Submanager_Player_Ratings::getAccountIDs(int cnt) {
    vector<string> temp = pratings_data[0];
    vector<string> result;
    for (unsigned i = 0; i < temp.size(); i++) {
        int tmp = stoi(pratings_data[0].at(i));
        int match_cnt = stoi(pratings_data[2].at(i));
        if (tmp >= 0 && match_cnt > 0) {
            result.push_back(pratings_data[0].at(i));
        }
        if (result.size() == cnt) { break; }
    }
    return result;
}

vector<double> Submanager_Player_Ratings::getWinrates() {
    vector<double> result;
    int len = pratings_data[0].size();
    for (unsigned i = 0; i < len; i++) {
        result.push_back(stod(pratings_data[1].at(i)) / stod(pratings_data[2].at(i)));
    }
    return result;
}

vector<double> Submanager_Player_Ratings::getMu() {
    vector<double> result;
    int len = pratings_data[0].size();
    for (unsigned i = 0; i < len; i++) {
        result.push_back(stod(pratings_data[3].at(i)));
    }
    return result;
}

vector<double> Submanager_Player_Ratings:: getSigma() {
    vector<double> result;
    int len = pratings_data[0].size();
    for (unsigned i = 0; i < len; i++) {
        result.push_back(stod(pratings_data[4].at(i)));
    }
    return result;
}

void Submanager_Player_Ratings::incremental_analytics_insert(string total_wins, string total_matches, string ts_mu, string ts_sigma) {
    ifstream inFS("stat_dump/predict_winrate.txt");
    if (!inFS.is_open()) { inFS.close(); cout << "Something went wrong. The file was corrupt." << endl; return; }

    ostringstream oSS;
    double sumWR;
    string temp;
    inFS >> sumWR;
    sumWR += (stod(total_wins) / stod(total_matches));
    oSS << to_string(sumWR) << "\n";

    getline(inFS, temp);
    getline(inFS, temp);

    stringstream ss(temp);
    string s;

    getline(ss, s, ',');
    temp = to_string(stoi(s) + 1);
    oSS << temp << ", ";
    getline(ss, s, ',');
    temp = to_string(stod(s) + stod(ts_mu));
    oSS << temp << ", ";
    getline(ss, s, ',');
    temp = to_string(stod(s) + (stod(ts_mu) * (stod(total_wins) / stod(total_matches))));
    oSS << temp << ", ";
    getline(ss, s, '\n');
    temp = to_string(stod(s) + pow(stod(ts_mu), 2));
    oSS << temp << "\n";

    getline(inFS, temp);

    stringstream ss2(temp);

    getline(ss2, s, ',');
    temp = to_string(stoi(s) + 1);
    oSS << temp << ", ";
    getline(ss2, s, ',');
    temp = to_string(stod(s) + stod(ts_sigma));
    oSS << temp << ", ";
    getline(ss2, s, ',');
    temp = to_string(stod(s) + (stod(ts_sigma) * (stod(total_wins) / stod(total_matches))));
    oSS << temp << ", ";
    getline(ss2, s, '\n');
    temp = to_string(stod(s) + pow(stod(ts_sigma), 2));
    oSS << temp;
    inFS.close(); inFS.clear();

    ofstream outFS("stat_dump/predict_winrate.txt");
    if (!outFS.is_open()) { inFS.close(); cout << "Something went wrong. The file was corrupt." << endl; return; }
    outFS << oSS.str();
    outFS.close(); outFS.clear();
}

void Submanager_Player_Ratings::incremental_analytics_delete(string total_wins, string total_matches, string ts_mu, string ts_sigma) {
    ifstream inFS("stat_dump/predict_winrate.txt");
    if (!inFS.is_open()) { inFS.close(); cout << "Something went wrong. The file was corrupt." << endl; return; }

    ostringstream oSS;
    double sumWR;
    string temp;
    inFS >> sumWR;
    sumWR -= (stod(total_wins) / stod(total_matches));
    oSS << to_string(sumWR) << "\n";

    getline(inFS, temp);
    getline(inFS, temp);

    stringstream ss(temp);
    string s;
    
    getline(ss, s, ',');
    temp = to_string(stoi(s) - 1);
    oSS << temp << ", ";
    getline(ss, s, ',');
    temp = to_string(stod(s) - stod(ts_mu));
    oSS << temp << ", ";
    getline(ss, s, ',');
    temp = to_string(stod(s) - (stod(ts_mu) * (stod(total_wins) / stod(total_matches))));
    oSS << temp << ", ";
    getline(ss, s, '\n');
    temp = to_string(stod(s) - pow(stod(ts_mu), 2));
    oSS << temp << "\n";

    getline(inFS, temp);

    stringstream ss2(temp);

    getline(ss2, s, ',');
    temp = to_string(stoi(s) - 1);
    oSS << temp << ", ";
    getline(ss2, s, ',');
    temp = to_string(stod(s) - stod(ts_sigma));
    oSS << temp << ", ";
    getline(ss2, s, ',');
    temp = to_string(stod(s) - (stod(ts_sigma) * (stod(total_wins) / stod(total_matches))));
    oSS << temp << ", ";
    getline(ss2, s, '\n');
    temp = to_string(stod(s) - pow(stod(ts_sigma), 2));
    oSS << temp;
    inFS.close(); inFS.clear();

    ofstream outFS("stat_dump/predict_winrate.txt");
    if (!outFS.is_open()) { inFS.close(); cout << "Something went wrong. The file was corrupt." << endl; return; }
    outFS << oSS.str();
    outFS.close(); outFS.clear();
}