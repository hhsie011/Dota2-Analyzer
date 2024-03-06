#pragma once
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include <algorithm>
#include <tuple>
#include <sys/types.h>
#include <sys/stat.h>
#include <ctime>
#include <iomanip>

#define PCOLS 73
#define MAXHERO 113

class Submanager_Players {
    private:
        std::vector<std::string> players_data[PCOLS];

        std::vector<std::tuple<std::string, std::string, std::string>> hero_xpgold;
    public:
        std::string csvFile = "../../Data/players.csv";
        std::string csvFile2 = "../../Temporary_Data/players.csv";
        std::string csvFile3 = "../../Backup_Data/players.csv";
        void readFile(std::string text);
        void writeFile(bool flag, char mode);
        std::vector<std::string> getHeroPickRate(int limit); //inserts hero ids into a multiset so you can use count() to check number of times picked
        std::multiset<std::string> getPlayerGameCount(); //inserts account ids into a multiset so you can use count() to check number of games played for each unique account
        std::vector<std::pair<std::string,std::string>> getPlayerHero(); //gets a pair that contains player id in first and hero id in second
        std::vector<std::tuple<std::string,std::string,std::string>> getPlayerMatch(); //gets a pair that contains player id in first and match id in second
        std::vector<std::tuple<std::string,std::string,std::string>> getHeroMatch(); //gets a tuple that contains hero id in first and match id in second and slot in third
        std::vector<std::tuple<std::string,std::string,std::string>> getMatchHero();
        std::vector<std::tuple<std::string,std::string,std::string,std::string,std::string>> getXPGoldGeneral(std::string limit);
        std::vector<std::tuple<std::string, std::string, std::string>> getXPGoldByHero(std::string limit, std::string numHeroes, std::string oldSize, std::string newSize, bool modifier);
        int getSize(); //returns size of players_data
        bool insertPlayer(std::vector<std::string>);
        //bool updatePlayer(int); no unique key, impossible(?) unless we just delete a specific line
        //bool deletePlayer(int); no unique key, impossible(?) unless we just delete a specific line
        bool delete_ability(int);
        bool checkInput(std::string);
};