#ifndef SUBM_CLUSTER
#define SUBM_CLUSTER

#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>
#include <stdexcept>
#include <ctype.h>

#define CLUSTER_COLS 2

class Submanager_Cluster {
private:
    std::vector<std::string> cluster_data[CLUSTER_COLS];
    bool requiresWrite = false;
public:
    const std::string fileName = "../../Data/cluster_regions.csv";
    const std::string fileName2 = "../../Temporary_Data/cluster_regions.csv";
    Submanager_Cluster() {};
    void readFile(std::string text);
    void writeFile(bool, char);
    std::string getClusterName(int cluster);
    bool addCluster(std::string cluster, std::string region);
    bool updateCluster(std::string, std::string);
    bool deleteCluster(std::string);
    bool checkInput(std::string, std::string);
};

#endif