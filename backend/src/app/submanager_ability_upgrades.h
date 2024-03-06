#pragma once
#include <string>
#include <vector>

#define abu_COLS 5

class ability_upgrades {
    private:
        std::vector<std::string> ability_data[abu_COLS];
        bool requiresWrite = false;
    public:
        void readFile(std::string text);
        //void readFile(std::string text, bool); nonfunctional currently
        void writeFile(bool);
        const std::string fileName = "../../Data/ability_upgrades.csv";
        const std::string fileName2 = "../../Temporary_Data/ability_upgrades.csv";
        const std::string fileName3 = "../../Backup_Data/ability_upgrades.csv";
        std::vector<std::pair<int,int>> get_upgrades(int, std::string, bool);
        bool addUpgrade(std::string, std::string, std::string, std::string, std::string);
        void updateUpgrade(std::string, std::string, std::string, std::string, std::string);
        void deleteUpgrade(std::string);
        bool checkInput(std::string,std::string,std::string,std::string,std::string);

        //for testing
        void printData();
};