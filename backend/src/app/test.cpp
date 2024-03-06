#include "submanager_players.h"
#include "submanager_match.h"
#include <ctime>
#include <iostream>

using namespace std;

#define NUM_HEROES 113;

// void analytics_winrates_over_time() {
//     time_t start, end;
//     time(&start);
//     cout << "Begin analytics_winrates_over_time..." << endl;
//     vector<bool> winloss;
//     vector<double> duration;
//     //First part that is done for reusuablity
//     //costly calculation if you only search for one hero, gets better if you do multiple hero searches
//     //someone has to implement the stats file dump :)
//     Submanager_Match m = Submanager_Match();
//     m.readFile(m.fileName);
//     winloss = m.findWinLoss();
//     duration = m.findDuration();
//     vector<tuple<string,string,string>> info;
//     Submanager_Players p = Submanager_Players();
//     p.readFile(p.csvFile);
//     info = p.getMatchHero();

//     vector<tuple<string,string,bool,bool,bool,bool,bool,bool,bool>> matches;
//     bool win = false, less20 = false, more20 = false, more30 = false, more40 = false, more50 = false, more60 = false;
//     string matchid;
//     string heroid;
//     double temptime;
//     for(unsigned i = 0; i < info.size(); i++)
//     {
//         // cout << i << endl;
//         matchid = get<0>(info.at(i));
//         heroid = get<1>(info.at(i));
//         if(stoi(get<2>(info.at(i))) < 100 && winloss.at(stoi(matchid)) == true)
//         {
//             win = true;
//         }
//         else if(stoi(get<2>(info.at(i))) > 100 && winloss.at(stoi(matchid)) == false)
//         {
//             win = true;
//         }
//         else
//         {
//             win = false;
//         }
//         temptime = duration.at(stoi(matchid))/60;
//         if (temptime < 20) {
//            less20 = true;
//         }
//         else if (temptime >= 20) {
//            more20 = true;
//             if (temptime >= 30) {
//                more30 = true;
//                if (temptime >= 40) {
//                   more40 = true;
//                   if (temptime >= 50) {
//                      more50 = true;
//                      if (temptime >= 60) {
//                         more60 = true;
//                      }
//                   }
//                }
//             }
//         }
//         matches.push_back(make_tuple(matchid,heroid,win,less20,more20,more30,more40,more50,more60));
//     }
//     cout << "Done with slow part" << endl;
//     //THIS IS WHERE YOU'D STORE THE INFO INTO A STATS FILE IF YOU WANTED TO NOT REDO THIS COMPUTATION
//     //Alternatively you could store the winrate of a specific hero into a stats file and only update that
//     //no idea how that would work tho.


//     ostringstream oSS;
//     oSS << "{\n";
//     oSS << "\t\"winrates\": [\n";
//     int heronum = NUM_HEROES;
//     for (int hero = 0; hero < heronum; hero++)
//     {
//         int wless20 = 0, tless20 = 0;
//         int wmore20 = 0, tmore20 = 0;
//         int wmore30 = 0, tmore30 = 0;
//         int wmore40 = 0, tmore40 = 0;
//         int wmore50 = 0, tmore50 = 0;
//         int wmore60 = 0, tmore60 = 0;

//         vector<double> result;
//         for (unsigned i = 0; i < matches.size(); i++)
//         {
//             if (get<1>(matches.at(i)) == to_string(hero - 1)) // hero id comparison, offset to match dataset
//             {
//                 if (get<3>(matches.at(i)))
//                 {
//                     tless20++;
//                     if(get<2>(matches.at(i))) wless20++; 
//                     continue;
//                 }
//                 if(get<8>(matches.at(i)))
//                 {
//                     tmore20++; tmore30++; tmore40++; tmore50++; tmore60++;
//                     if(get<2>(matches.at(i))) { wmore20++; wmore30++; wmore40++; wmore50++; wmore60++; }
//                     continue;
//                 }
//                 if(get<7>(matches.at(i)))
//                 {
//                     tmore20++; tmore30++; tmore40++; tmore50++;
//                     if(get<2>(matches.at(i))) { wmore20++; wmore30++; wmore40++; wmore50++; }
//                     continue;
//                 }
//                 if(get<6>(matches.at(i)))
//                 {
//                     tmore20++; tmore30++; tmore40++;
//                     if(get<2>(matches.at(i))) { wmore20++; wmore30++; wmore40++; }
//                     continue;
//                 }
//                 if(get<5>(matches.at(i)))
//                 {
//                     tmore20++; tmore30++;
//                     if(get<2>(matches.at(i))) { wmore20++; wmore30++; }
//                     continue;
//                 }
//                 if(get<4>(matches.at(i)))
//                 {
//                     tmore20++;
//                     if(get<2>(matches.at(i))) { wmore20++; }
//                     continue;
//                 }
//             }
//         }
//         if (tless20 > 0) { result.push_back(wless20/(double)tless20 * 100); } //winrates of matches ending in < 20 minutes
//         else { result.push_back(0); }
//         // cout << "WR of < 20 " << result.at(result.size()-1) << endl; // debug
        
//         if (tmore20 > 0) { result.push_back(wmore20/(double)tmore20 * 100); } //winrates of matches ending in > 20 minutes
//         else { result.push_back(0); }
//         // cout << "WR of > 20 " << result.at(result.size()-1) << endl; // debug
        
//         if (tmore30 > 0) { result.push_back(wmore30/(double)tmore30 * 100); } //winrates of matches ending in > 30 minutes
//         else { result.push_back(0); }
//         // cout << "WR of > 30 " << result.at(result.size()-1) << endl; // debug
        
//         if (tmore40 > 0) { result.push_back(wmore40/(double)tmore40 * 100); } //winrates of matches ending in > 40 minutes
//         else { result.push_back(0); }
//         // cout << "WR of > 40 " << result.at(result.size()-1) << endl; // debug
        
//         if (tmore50 > 0) { result.push_back(wmore50/(double)tmore50 * 100); } //winrates of matches ending in > 50 minutes
//         else { result.push_back(0); }
//         // cout << "WR of > 50 " << result.at(result.size()-1) << endl; // debug
        
//         if (tmore60 > 0) { result.push_back(wmore60/(double)tmore60 * 100); } //winrates of matches ending in > 60 minutes
//         else { result.push_back(0); }
//         // cout << "WR of > 60 " << result.at(result.size()-1) << endl; // debug

//         //result vector contains winrates for a specific hero based on timebrackets of 10 minutes starting at 20 minutes.

//         oSS << "\t{\n";
//         oSS << "\t\t\"id\": \"" << hero << "\",\n";
//         oSS << "\t\t\"wless20\": \"" << fixed << setprecision(2) << result.at(0) << "\",\n";
//         oSS << "\t\t\"wmore20\": \"" << fixed << setprecision(2) << result.at(1) << "\",\n";
//         oSS << "\t\t\"wmore30\": \"" << fixed << setprecision(2) << result.at(2) << "\",\n ";
//         oSS << "\t\t\"wmore40\": \"" << fixed << setprecision(2) << result.at(3) << "\",\n";
//         oSS << "\t\t\"wmore50\": \"" << fixed << setprecision(2) << result.at(4) << "\",\n";
//         oSS << "\t\t\"wmore60\": \"" << fixed << setprecision(2) << result.at(5) << "\"\n\t}";
//         if (hero != 113) { oSS << ",\n"; }
//     }
//     oSS << "\n\t]\n}";
//     ofstream outFS("test.txt");
//     if (outFS.is_open()) {
//         outFS << oSS.str();
//     }
//     outFS.close();

//     time(&end);
//     double time_taken = double(end - start);
//     cout << "\nTime taken by analytics_winrates_over_time feature is : " << fixed <<  time_taken << setprecision(5);
//     cout << " sec " << endl;
// }

int main(){
    //    analytics_winrates_over_time();
    Submanager_Match mhs = Submanager_Match();
    mhs.readFile(mhs.fileName);
   return 0;
}