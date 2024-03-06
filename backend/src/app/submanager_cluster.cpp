#include "submanager_cluster.h"

using namespace std;

void Submanager_Cluster::readFile(string text) {
    ifstream infile;
    infile.open(text);
    string fileTemp;
    if (infile.is_open()) {
        getline(infile,fileTemp);
        while (getline(infile, fileTemp)) {
            stringstream ss(fileTemp);
            string value;
            for (unsigned i = 0; i < CLUSTER_COLS; i++) {
                if (getline(ss,value,',')) {
                cluster_data[i].push_back(value);
                }
            }
        }
    }
    infile.close();
}

void Submanager_Cluster::writeFile(bool flag, char mode) {
    int colCount = 0;
    ofstream outfile;
    
    //check if you need to write
    if (!flag && !requiresWrite) {
        return;
    }
    requiresWrite = false;

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
            output << "name,hero_id,localized_name" << endl;
            for (unsigned row = 0; row < cluster_data[1].size(); row++) 
            {
                if (!(cluster_data[1].at(row) == "DELETED")) {
                    for (unsigned col = 0; col < CLUSTER_COLS; col++) {
                        output << cluster_data[col].at(row) ;
                        if (col != CLUSTER_COLS - 1) { output << ','; }
                    }
                    output << "\n";
                }
            }
        }
        else if (mode == 'a')
        {
            int row = cluster_data[0].size()-1;
            if (!(cluster_data[0].at(row) == "DELETED")) {
                for (unsigned col = 0; col < CLUSTER_COLS; col++) 
                {
                    output << cluster_data[col].at(row) ;
                    if (col != CLUSTER_COLS - 1) { output << ','; }
                }
                output << "\n";
            }
        } 
        else 
        {
            cout << "ERROR! SELECTED MODE IS INVALID!!" << endl;
            return;
        }
    }
    outfile << output.rdbuf();
    output.str("");
    outfile.close(); outfile.clear();
}

string Submanager_Cluster::getClusterName(int cluster) {
    for (unsigned row = 0; row < cluster_data[0].size(); row++) {
        if (stoi(cluster_data[0].at(row)) == cluster) {
            return cluster_data[1].at(row);
        }
    }
    return "";
}

bool Submanager_Cluster::checkInput(string cluster, string region = "") {
    for (unsigned i = 0; i < cluster.size(); i++) {
        if (!isdigit(cluster[i])) {
            return false;
        }
    }
    for (unsigned i = 0; i < region.size(); i++) {
        if (!isalpha(region[i]) && !isspace(region[i])) {
            return false;
        }
    }
    return true;
}

bool Submanager_Cluster::addCluster(string cluster, string region) {
    if (!checkInput(cluster, region)) {
        cout << "Error: Invalid input!" << endl;
        return false;
    }
    for (unsigned i = 0; i < cluster_data[0].size(); i++) {
        if (cluster_data[0].at(i) == cluster) {
            cout << "Error! Already Exists!" << endl;
            return false;
        }
    }
    requiresWrite = true;
    for (unsigned i = 0; i < CLUSTER_COLS; i++) {
        cluster_data[i].push_back("");
    }
    cluster_data[0].at(cluster_data[0].size()-1) = cluster;
    cluster_data[1].at(cluster_data[1].size()-1) = region;
    return true;
}

void modspecline_helper_cluster(string filename, string cluster, string region) {
    fstream input;
    input.open(filename, ios::in | ios::out | ios::binary);
    string line = "";
    bool flag = false;
    if(!input.is_open()) { exit(1); input.close(); }
    else
    {
        while(getline(input, line))
        {
            stringstream ss(line);
            string data1,data2;
            //keep track of size of line
            int prevLineSize = line.length();
            //get current file position
            streampos oldPos = input.tellg(); 
            if(getline(ss, data1, ',') && getline(ss, data2))
            {
                if(data2 == cluster)
                {
                    cout << "hero_name = " << data1 << " , region = " << data2 << endl; 
                    data1 = cluster;
                    data2 = region;
                    if(oldPos > input.beg || oldPos < input.beg) { oldPos -= prevLineSize + 1; }
                    input.seekp(oldPos);
                    stringstream tempSS;
                    tempSS << data1.c_str() << ',' << data2.c_str();
                    input.write(tempSS.str().c_str(), tempSS.str().size());
                    flag = true;
                }
            }
            if(flag)break;
        }
        if(!flag) { cout << "Cluster ID not found. Cannot make changes to stat file." << endl; }
    }
    input.close(); input.clear();
}

bool Submanager_Cluster::updateCluster(string cluster, string region) {
    if (!checkInput(cluster, region)) {
        cout << "Error: Invalid input!" << endl;
        return false;
    }
    for (unsigned i = 0; i < cluster_data[0].size(); i++) {
        if (cluster_data[0].at(i) == cluster) {
            cluster_data[1].at(i) = region;
            return true;
        }
    }
    cout << "Cluster not found!" << endl;
    //modspecline_helper_cluster(fileName2, cluster, region);
    return false;
}

bool Submanager_Cluster::deleteCluster(string cluster) {
    if (!checkInput(cluster)) {
        cout << "Error: Invalid input!" << endl;
        return false;
    }
    for (unsigned i = 0; i < cluster_data[0].size(); i++) {
        if (cluster_data[0].at(i) == cluster) {
            cluster_data[0].at(i) = "DELETED";
            requiresWrite = true;
            return true;
        }
    }
    cout << "Cluster not found!" << endl;
    return false;
}

