  
#include "submanager_match.h"
#include <algorithm>
#include <tuple>

using namespace std;

void Submanager_Match::readFile(string text) {
    ifstream infile;
    infile.open(text);
    string fileTemp;
    vector<tuple<int, string, string, string, string, string, string, string, string, string, string, string, string>> matchesToSort;
    if (infile.is_open()) {
        getline(infile,fileTemp);
        while (getline(infile, fileTemp)) {
            stringstream ss(fileTemp);
            string value;
            vector<string> tempHolder;
            for (unsigned i = 0; i < MATCH_COLS; i++) {
                if (getline(ss,value,',')) {
                    tempHolder.push_back(value);
                }
            }
            matchesToSort.push_back(make_tuple(stoi(tempHolder.at(0)), tempHolder.at(1), tempHolder.at(2), tempHolder.at(3), tempHolder.at(4), tempHolder.at(5), tempHolder.at(6),
                tempHolder.at(7), tempHolder.at(8), tempHolder.at(9), tempHolder.at(10), tempHolder.at(11), tempHolder.at(12)));
        }
        sort(matchesToSort.begin(),matchesToSort.end());
        for (unsigned j = 0; j < matchesToSort.size(); j++) {
            match_data[0].push_back(to_string(get<0>(matchesToSort.at(j))));
            match_data[1].push_back(get<1>(matchesToSort.at(j)));
            match_data[2].push_back(get<2>(matchesToSort.at(j)));
            match_data[3].push_back(get<3>(matchesToSort.at(j)));
            match_data[4].push_back(get<4>(matchesToSort.at(j)));
            match_data[5].push_back(get<5>(matchesToSort.at(j)));
            match_data[6].push_back(get<6>(matchesToSort.at(j)));
            match_data[7].push_back(get<7>(matchesToSort.at(j)));
            match_data[8].push_back(get<8>(matchesToSort.at(j)));
            match_data[9].push_back(get<9>(matchesToSort.at(j)));
            match_data[10].push_back(get<10>(matchesToSort.at(j)));
            match_data[11].push_back(get<11>(matchesToSort.at(j)));
            match_data[12].push_back(get<12>(matchesToSort.at(j)));
        }
    }
    infile.close();
}

void Submanager_Match::writeFile(bool flag, char mode) {
    int colCount = 0;
    ofstream outfile;
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
            output << "match_id,start_time,duration,tower_status_radiant,tower_status_dire,barracks_status_dire,barracks_status_radiant,first_blood_time,game_mode,radiant_win" << endl;
            for (unsigned row = 0; row < match_data[0].size(); row++) 
            {
                if (!(match_data[0].at(row) == "DELETED")) {
                    for (unsigned col = 0; col < MATCH_COLS; col++) {
                        output << match_data[col].at(row) ;
                        if (col != MATCH_COLS - 1) { output << ','; }
                    }
                    output << "\n";
                }
            }
        }
        else if (mode == 'a')
        {
            int row = match_data[0].size()-1;
            if (!(match_data[0].at(row) == "DELETED")) {
                for (unsigned col = 0; col < MATCH_COLS; col++) 
                {
                    output << match_data[col].at(row) ;
                    if (col != MATCH_COLS - 1) { output << ','; }
                }
                output << "\n";
            }
        } 
        else 
        {
            cout << "ERROR! SELECTED MODE IS INVALID!!" << endl;
            return;
        }
        outfile << output.rdbuf();
        output.str("");
    }
    outfile.close(); outfile.clear();

    outfile.open("stat_dump/winrates_diagnostic.txt");
    if (outfile.is_open()) {
        outfile << match_data[0].size();
    }
    outfile.close(); outfile.clear();
}

int Submanager_Match::getSize() {
    return match_data[0].size();
}

//gets all match_ids that exist between the min and max duration provided
vector<string> Submanager_Match::findByDurationRange(int mindur, int maxdur) {
    vector<string> temp;
    for (unsigned row = 0; row < match_data[2].size(); row++) {
        if (std::stoi(match_data[2].at(row)) >= mindur && std::stoi(match_data[2].at(row)) <= maxdur) {
            temp.push_back(match_data[0].at(row));
        }
    }
    return temp;
}

vector<bool> Submanager_Match::findWinLoss(){
    vector<bool> temp;
    for(unsigned i = 0; i < match_data[9].size(); i++){
        if(match_data[9].at(i) == "False"){
            temp.push_back(false);
        }else{
            temp.push_back(true);
        }
    }
    return temp;
}

vector<double> Submanager_Match::findDuration(){
    vector<double> temp;
    for(unsigned i = 0; i < match_data[2].size(); i++){
        temp.push_back(stod(match_data[2].at(i)));
    }
    return temp;
}

int Submanager_Match::getMatchCount(){
    return match_data[0].size();
}

vector<string> Submanager_Match::getMatchIDs(int cnt) {
    
    vector<string> result;
    for(unsigned i = 0; i < cnt; i++){
        result.push_back(match_data[0].at(i));
    }
    return result;
}

bool Submanager_Match::checkRadiantWin(string id) {
    bool found = false;
    int low = 0, mid, high = match_data[0].size()-1;
    while (low <= high) {
        mid = (low + high) / 2;
        if (stoi(id) < stoi(match_data[0].at(mid))) {
            high = mid - 1;
        } else if (stoi(id) > stoi(match_data[0].at(mid))) {
            low = mid + 1;
        } else {
            found = true;
            if (match_data[9].at(mid) == "True") return true;
            break;
        }
    }
    if (found) {
        return false;
    } else {
        cout << "No match exists with this id!" << endl;
        throw std::out_of_range("Match ID");
    }
}   

string Submanager_Match::getFirstBloodTime(string id) {
    for(unsigned i = 0; i < match_data[7].size(); i++){
        if (id == match_data[0].at(i)) {
            return match_data[7].at(i);
        }
    }
    return "";
}

string Submanager_Match::getLikes(string id) {
    for(unsigned i = 0; i < match_data[11].size(); i++){
        if (id == match_data[0].at(i)) {
            return match_data[11].at(i);
        }
    }
    return "";
}

string Submanager_Match::getDislikes(string id) {
    for(unsigned i = 0; i < match_data[10].size(); i++){
        if (id == match_data[0].at(i)) {
            return match_data[10].at(i);
        }
    }
    return "";
}

int Submanager_Match::getCluster(string id) {
    for(unsigned i = 0; i < match_data[12].size(); i++){
        if (id == match_data[0].at(i)) {
            return stoi(match_data[12].at(i));
        }
    }
    return 0;
}

string Submanager_Match::getMatchIDByRow(int row) {
    return match_data[0].at(row);
}

string Submanager_Match::getLikesByRow(int row) {
    return match_data[12].at(row);
}

string Submanager_Match::getDisLikesByRow(int row) {
    return match_data[12].at(row);
}

//helper
bool isNumber(string value) {
    if (value != "-1") { 
        for (unsigned i = 0; i < value.size(); i++) {
            if (!isdigit(value[i])) {
                cout << "ERROR! NOT A DIGIT!" << endl;
                return false;
            }
        }
    }
    return true;
}

// bool Submanager_Match::checkInput (string input[MATCH_COLS]) {
//     for (unsigned i = 0; i < input[0].size(); i++) {
//         if (input[0].size() == '-' && i == 0 && input[0].size() > 1) {
//             if (!isdigit(input[0][i])) {
//                 cout << "ERROR! NOT A DIGIT!" << endl;
//                 return false;
//             }
//         }
//     }
//     for (unsigned i = 0; i < MATCH_COLS; i++) {
//         if (!isNumber(input[i])) {
//             return false;
//         }
//     }
//     return true;
// }
 bool Submanager_Match::checkInput(string mid, string st, string dur, string tsr, string tsd, string bsd, string bsr, string fbt, string gm, 
     string rw, string nv, string pv, string cl) {
         for (unsigned i = 0; i < mid.size(); i++) {
            if (mid[0] == '-' && i == 0) {
                if (mid.size() < 2) {
                    return false;
                }
                i++;
            }
            if (!isdigit(mid[i])) {
                cout << "ERROR! NOT A DIGIT!" << endl;
                return false;
            }
        }
        if (st != "-1") { 
            for (unsigned i = 0; i < st.size(); i++) {
                if (!isdigit(st[i])) {
                    cout << "ERROR! NOT A DIGIT!" << endl;
                    return false;
                }
            }
        }
        if (dur != "-1") {
            for (unsigned i = 0; i < dur.size(); i++) {
                if (!isdigit(dur[i])) {
                    cout << "ERROR! NOT A DIGIT!" << endl;
                    return false;
                }
            }
        }
        if (tsr != "-1") {
            for (unsigned i = 0; i < tsr.size(); i++) {
                if (!isdigit(tsr[i])) {
                    cout << "ERROR! NOT A DIGIT!" << endl;
                    return false;
                }
            }
        }
        if (tsd != "-1") {
            for (unsigned i = 0; i < tsd.size(); i++) {
                if (!isdigit(tsd[i])) {
                    cout << "ERROR! NOT A DIGIT!" << endl;
                    return false;
                }
            }
        }
        if (bsd != "-1") {
            for (unsigned i = 0; i < bsd.size(); i++) {
                if (!isdigit(bsd[i])) {
                    cout << "ERROR! NOT A DIGIT!" << endl;
                    return false;
                }
            }
        }
        if (bsr != "-1") {
            for (unsigned i = 0; i < bsr.size(); i++) {
                if (!isdigit(bsr[i])) {
                    cout << "ERROR! NOT A DIGIT!" << endl;
                    return false;
                }
            }
        }
        if (fbt != "-1") {
            for (unsigned i = 0; i < fbt.size(); i++) {
                if (!isdigit(fbt[i])) {
                    cout << "ERROR! NOT A DIGIT!" << endl;
                    return false;
                }
            }
        }
        if (gm != "-1") {
            for (unsigned i = 0; i < gm.size(); i++) {
                if (!isdigit(gm[i])) {
                    cout << "ERROR! NOT A DIGIT!" << endl;
                    return false;
                }
            }
        }
        if (rw != "False" && rw != "True" && rw != "-1") {
            cout << "ERROR! NOT FORMATTED CORRECTLY!" << endl;
            return false;
        }
        if (nv != "-1") {
            for (unsigned i = 0; i < nv.size(); i++) {
                if (!isdigit(nv[i])) {
                    cout << "ERROR! NOT A DIGIT!" << endl;
                    return false;
                }
            }
        }
        if (pv != "-1") {
            for (unsigned i = 0; i < pv.size(); i++) {
                if (!isdigit(pv[i])) {
                    cout << "ERROR! NOT A DIGIT!" << endl;
                    return false;
                }
            }
        }
        if (cl != "-1") {
            for (unsigned i = 0; i < cl.size(); i++) {
                if (!isdigit(cl[i])) {
                    cout << "ERROR! NOT A DIGIT!" << endl;
                    return false;
                }
            }
        }
        return true;
}

bool Submanager_Match::addMatch(string mid, string st, string dur, string tsr, string tsd, string bsd, string bsr, string fbt, string gm, string rw, string nv, string pv, string cl) {
    string input[MATCH_COLS] = {mid, st, dur, tsr, tsd, bsd, bsr, fbt, gm, rw, nv, pv, cl };
    /*if (!checkInput(input)) {
        return false;
    }*/
    if (!checkInput(mid, st, dur, tsr, tsd, bsd, bsr, fbt, gm, rw, nv, pv, cl)) {
         return false;
    }
    requiresWrite = true;
    for (unsigned i = 0; i < MATCH_COLS; i++) {
        match_data[i].push_back("0");
    }
    long int numMatches = match_data[0].size() - 1;

    if (mid == "-1") { mid = to_string(numMatches); }
    match_data[0].at(match_data[0].size()-1) = mid;

    if (st == "-1") { st = "N/A"; }
    match_data[1].at(match_data[0].size()-1) = st;

    if (dur == "-1") { dur = "N/A"; }
    match_data[2].at(match_data[0].size()-1) = dur; 

    if (tsr == "-1") { tsr = "N/A"; }
    match_data[3].at(match_data[0].size()-1) = tsr; 

    if (tsd == "-1") { tsd = "N/A"; }
    match_data[4].at(match_data[0].size()-1) = tsd; 

    if (bsd == "-1") { bsd = "N/A"; }
    match_data[5].at(match_data[0].size()-1) = bsd; 

    if (bsr == "-1") { bsr = "N/A"; }
    match_data[6].at(match_data[0].size()-1) = bsr; 

    if (fbt == "-1") { fbt = "N/A"; }
    match_data[7].at(match_data[0].size()-1) = fbt; 

    if (gm == "-1") { gm = "N/A"; }
    match_data[8].at(match_data[0].size()-1) = gm; 

    if (rw == "-1") { rw = "N/A"; }
    match_data[9].at(match_data[0].size()-1) = rw;

    if (nv == "-1") { nv = "N/A"; }
    match_data[10].at(match_data[0].size()-1) = nv;

    if (pv == "-1") { pv = "N/A"; }
    match_data[11].at(match_data[0].size()-1) = pv;

    if (cl == "-1") {cl = "N/A"; }
    match_data[12].at(match_data[0].size()-1) = cl;  

    return true;    
}

void modspecline_helper(string filename, string match_id, string st, string dur, string tsr, string tsd, string bsd, string bsr, string fbt, string gm, string rw, string nv, string pv, string cl) {
    fstream input;
    input.open(filename, ios::in | ios::out | ios::binary);
    string line = "";
    bool flag = false;
    if(!input.is_open()) { exit(1); input.close(); }
    else
    {
        while(getline(input, line))
        {
            cout << line << endl;
            stringstream ss(line);
            string data1,data2, data3, data4, data5, data6, data7, data8, data9, data10, data11, data12, data13;
            string dump;
            //keep track of size of line
            int prevLineSize = line.length();
            //get current file position
            streampos oldPos = input.tellg(); 
            if(getline(ss, data1, ',') && getline(ss, data2, ',') && getline(ss, data3, ',') && getline(ss, data4, ',') && getline(ss, data5, ',')
                && getline(ss,data6,',') && getline(ss,data7,',') && getline(ss,data8,',') && getline(ss, data9, ',') && getline(ss,data10,',') 
                && getline(ss, data11, ',') && getline(ss,data12,',') && getline(ss,data13,',') && getline(ss,dump))
            {
                cout << data1 << endl;
                if(stoi(data1) == stoi(match_id))
                {
                    flag = true;
                    data2 = st;
                    data3 = dur;
                    data4 = tsr;
                    data5 = tsd;
                    data6 = bsd;
                    data7 = bsr;
                    data8 = fbt;
                    data9 = gm;
                    data10 = rw;
                    data11 = nv;
                    data12 = pv;
                    data13 = cl;
                    //Did this way because it's the only way != works on Windows
                    if(oldPos > input.beg || oldPos < input.beg) { oldPos -= prevLineSize + 1; }
                    input.seekp(oldPos);
                    stringstream tempSS;
                    tempSS << data1.c_str() << ',' << data2.c_str() << ',' << data3.c_str() << ',' << data4.c_str() << ',' << data5.c_str() << ',' << data6.c_str()
                     << ',' << data7.c_str() << ',' << data8.c_str() << ',' << data9.c_str() << ',' << data10.c_str() << ',' << data11.c_str() << ',' << data12.c_str()
                      << ',' << data13.c_str() << ',' << dump.c_str();
                    input.write(tempSS.str().c_str(), tempSS.str().size());
                }
            }
            //cout << flag << endl;
            if(flag) {break;}
        }
        //cout << flag << endl;
        //if(!flag) { cout << "Match ID not found.  Cannot make changes to stat file." << endl; }
    }
    input.close(); input.clear();
}

bool Submanager_Match::updateMatch(string mid, string st, string dur, string tsr, string tsd, string bsd, string bsr, string fbt, string gm, string rw, string nv, string pv, string cl) {
    string input[MATCH_COLS] = {mid, st, dur, tsr, tsd, bsd, bsr, fbt, gm, rw, nv, pv, cl };
    /*if (!checkInput(input)) {
        return false;
    }*/
     if (!checkInput(mid, st, dur, tsr, tsd, bsd, bsr, 
     fbt, gm, rw, nv, pv, cl)) {
          return false;
     }
    for (unsigned i = 0; i < match_data[0].size(); i++) {
        if (match_data[0].at(i) == mid) {
            match_data[1].at(i) = st;
            match_data[2].at(i) = dur;
            match_data[3].at(i) = tsr;
            match_data[4].at(i) = tsd;
            match_data[5].at(i) = bsd;
            match_data[6].at(i) = bsr;
            match_data[7].at(i) = fbt;
            match_data[8].at(i) = gm;
            match_data[9].at(i) = rw;
            match_data[10].at(i) = nv;
            match_data[11].at(i) = pv;
            match_data[12].at(i) = cl;
            //modspecline_helper(fileName2, mid, st, dur, tsr, tsd, bsd, bsr, fbt, gm, rw, nv, pv, cl);
            return true;
        }
    }
    return false;
}

bool Submanager_Match::deleteMatch(string match_id) {
        for (unsigned i = 0; i < match_id.size(); i++) {
            if (!isdigit(match_id[i])) {
                cout << "ERROR! NOT A DIGIT!" << endl;
                return false;
            }
        }
    for (unsigned i = 0; i < match_data[0].size(); i++) {
        if (match_data[0].at(i) == match_id) {
            match_data[0].at(i) = "DELETED";
            requiresWrite = true;
            return true;
        }
    }
    cout << "ERROR! NOT FOUND!" << endl;
    return false;
}

