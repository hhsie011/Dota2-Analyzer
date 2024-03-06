#pragma once
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include <utility>

#define HNCOLS 3

class Submanager_Hero_Names {
    private:
        std::vector<std::string> hname_data[HNCOLS];
        bool requiresWrite = false;
    public:
        const std::string fileName = "../../Data/hero_names.csv";
        const std::string fileName2 = "../../Temporary_Data/hero_names.csv";
        const std::string fileName3 = "../../Backup_Data/hero_names.csv";
        void readFile(std::string text);
        void writeFile(bool, char);
        std::string idByHeroName(std::string heroName);
        std::vector<std::string> getHnames(int); //pulls hero names
        std::vector<std::string> getHids(int); //pulls hero ids
        bool checkInput(std::string, std::string, std::string);
        bool insertHero(std::string, std::string, std::string);
        bool updateHero(std::string, std::string, std::string);
        bool deleteHero(std::string);
        int getIDSize();
};