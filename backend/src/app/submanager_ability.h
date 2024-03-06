#pragma once
#include <string>
#include <vector>

#define ABILITY_COLS 2

class ability {
    private:
        std::vector<std::string> ability_data[ABILITY_COLS];
        bool requiresWrite = false;
    public:
        void readFile(std::string text);
        //bool for save value: 1 - permanent
        //char for output mode: o overwrite, a append
        void writeFile(bool, char);
        const std::string fileName = "../../Data/ability_ids.csv";
        const std::string fileName2 = "../../Temporary_Data/ability_ids.csv";
        const std::string fileName3 = "../../Backup_Data/ability_ids.csv";
        int get_size(void);
        std::string get_ability_name(int);
        bool insert_ability(int, std::string);
        bool update_ability(int, std::string);
        bool delete_ability(int);
        bool checkInput(std::string);
};