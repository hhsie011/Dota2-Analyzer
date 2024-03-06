#ifndef SUBM_OBJECTIVE_H
#define SUBM_OBJECTIVE_H

#include <string>
#include <vector>

#define OBJ_COLS 9

class Submanager_Objective {
private:
    std::vector<std::string> obj_data[OBJ_COLS];
    bool requiresWrite = false;
public:
    std::string csvfile = "../../Data/objectives.csv";
    std::string csvfile2 = "../../Temporary_Data/objectives.csv";
    void readFile(std::string text);
    void writeFile(std::string text);
    std::vector<std::pair<std::string,std::string>> getFirstBlood(int limit);
    void addFirstBlood(std::string match_id, std::string player);
    void updateFirstBlood(std::string match_id, std::string player);
    void deleteFirstBlood(std::string match_id);
    bool checkInput(std::string, std::string);
};

#endif