#pragma once
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <utility>
#include <math.h>

#define PRCOLS 5

class Submanager_Player_Ratings {
    private:
        std::vector<std::string> pratings_data[PRCOLS];
    public:
        std::string fileName = "../../Data/player_ratings.csv";
        std::string fileName2 = "../../Temporary_Data/player_ratings.csv";
        std::string fileName3 = "../../Backup_Data/player_ratings.csv";
        void readFile(std::string text);
        void writeFile(std::string text);
        int getTotalWins(int account_id);            // returns total wins by account id
        int getTotalMatches(int account_id);         // return total matches by account id
        std::vector<std::string> getAccountIDs(int cnt);
        std::vector<double> getWinrates();
        std::vector<double> getMu();
        std::vector<double> getSigma();
        void incremental_analytics_insert(std::string total_wins, std::string total_matches, std::string ts_mu, std::string ts_sigma);
        void incremental_analytics_delete(std::string total_wins, std::string total_matches, std::string ts_mu, std::string ts_sigma);
};