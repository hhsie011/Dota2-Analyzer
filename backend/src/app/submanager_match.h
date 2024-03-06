#ifndef SUBM_MATCH
#define SUBM_MATCH

#pragma once

#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>
#include <stdexcept>
#include <ctype.h>

#define MATCH_COLS 13

class Submanager_Match {
private:
    std::vector<std::string> match_data[MATCH_COLS];
    bool requiresWrite = false;
public:
    const std::string fileName = "../../Data/match.csv";
    const std::string fileName2 = "../../Temporary_Data/match.csv";
    const std::string fileName3 = "../../Backup_Data/match.csv";
    Submanager_Match() {};
    void readFile(std::string text);
    void writeFile(bool, char);
    //find size of match_data
    int getSize();
    std::vector<std::string> findByDurationRange(int mindur, int maxdur); //extract match id based on duration
    std::vector<bool> findWinLoss(); //extract win info about matches
    std::vector<double> findDuration();
    int getMatchCount();
    std::vector<std::string> getMatchIDs(int cnt);
    bool checkRadiantWin(std::string id);
    std::string getFirstBloodTime(std::string id);
    std::string getLikes(std::string id);
    std::string getDislikes(std::string id);
    int getCluster(std::string id);
    std::string getMatchIDByRow(int row);
    std::string getLikesByRow(int row);
    std::string getDisLikesByRow (int row);
    //keeping these dispensables for now because we might want to revert changes
     bool checkInput(std::string, std::string, std::string, std::string, std::string, std::string, std::string, std::string, std::string, std::string, std::string, std::string, std::string);
    //bool checkInput(std::string input[MATCH_COLS]);
    bool addMatch(std::string, std::string, std::string, std::string, std::string, std::string, std::string, std::string, 
        std::string, std::string, std::string, std::string, std::string);
    bool updateMatch(std::string, std::string, std::string, std::string, std::string, std::string, std::string, std::string, 
        std::string, std::string, std::string, std::string, std::string);
    bool deleteMatch(std::string);
};

#endif