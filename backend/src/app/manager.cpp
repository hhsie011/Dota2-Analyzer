#include "manager.h"

using namespace std;

int main(void)
{
    //0: ability upgrades, 1:hero, 2: match 3: players
    int modification_counter[4] = {0, 0, 0, 0};
    string choice = "", past_choice = "";
    string pastTempString = "";
    string parameter[PARAMETERS];
    time_t oldModTime = 0, newModTime;
    bool backendNoRequestMessage = false, requestFileDNEMessage = false;
    bool is_processing = false, is_asleep = false, error = false;
    
    // modify check array
    // 0: ability, 1: hero, 2: player, 3: match
    // set to true if data has been modified by insert/update/delete
    // decides whether to read from original csv or temporary_data csv
    bool modified[] = {false, false, false, false};

    // backup check array
    // 0: ability, 1: hero, 2: player, 3: match
    // set to true if data has been backed up
    // determines if there's data to import from saved_data csv
    bool backup[] = {false, false, false, false};

    // temporary data vector (to store new inserts and display in search)
    // 0: ability, 1: hero, 2: player, 3: match
    // used to store new data entries (made by insert)
    // to display at the end of the output in search features
    vector<vector<string>> tmp_new(4);

    // store previous search limits
    // 0: ability, 1: hero, 2: player, 3: first blood, 4: clsuter region, 5: most liked
    // mainly to stop the frontend from refreshing if no new data is sent
    // if previous limit is not equal to new limit, process the search feature
    vector<string> limit(6);

    // store update check
    // 0: ability, 1: hero, 2: player, 3: first blood, 4: clsuter region, 5: most liked
    // mainly to stop the frontend from refreshing if no new data is sent
    // if a new update has been made, process the search feature
    // set to false after search feature starts
    bool updateSearch[] = {true, true, true, true, true, true};

    // first time running for analytics purposes
    bool init = true;

    time_t start, end;
    while (true)
    {
        /** ANALYTICS FUNCTION CALLS **/
        if (init) // only call the first time backend is run
        {
            is_processing = true;
            ifstream test;
            test.open(out1);
            if(test.is_open())
            {
                if(test.peek() != ifstream::traits_type::eof() || test.peek() != '\n'){}
                else { search_top_abilities("500",0); }
            }
            test.close(); test.clear();
            test.open(out9);
            if(test.is_open())
            {
                if(test.peek() != ifstream::traits_type::eof() || test.peek() != '\n'){}
                else { analytics_winrates_over_time("10", false); }
            }
            test.close(); test.clear();
            test.open(out10);
            if(test.is_open())
            {
                if(test.peek() != ifstream::traits_type::eof() || test.peek() != '\n'){}
                else { analytics_search_vs_winrate("1","1", "10", false); }
            }
            test.close(); test.clear();
            test.open(out11);
            if(test.is_open())
            {
                if(test.peek() != ifstream::traits_type::eof() || test.peek() != '\n'){}
                else { analytics_first_blood_winrate("25", false); }
            }
            test.close(); test.clear();
            test.open(out13);
            if(test.is_open())
            {
                if(test.peek() != ifstream::traits_type::eof() || test.peek() != '\n'){}
                else { analytics_compare_xp_and_gold_heroes("200", "112", false, false); }
            }
            test.close(); test.clear();
            init = false;
            is_processing = false;
        }

        //  TODO: check num of modifications here and assign choice appropiately here (if condition)
        //  Example: 
        if(modification_counter[2] >= 100) 
        { 
            is_processing = true;
            cout << "Start analytics..." << endl;
            analytics_winrates_over_time("10", true);
            analytics_first_blood_winrate("25", true);
            analytics_search_vs_winrate("3000","1", "10", true);
            cout << "Complete analytics..." << endl;
            modification_counter[2] = 0;
            is_processing = false;
        }
        
        if (modification_counter[3] >= 20) {
            is_processing = true;
            cout << "Start analytics..." << endl;
            if (modification_counter[1] > 0) {
                analytics_compare_xp_and_gold_heroes("200", "112", true, true);
            } else {
                analytics_compare_xp_and_gold_heroes("200", "112", true, false);
            }
            cout << "Complete analytics..." << endl;
            modification_counter[3] = 0;
            is_processing = false;
        }

        is_asleep = false;
        start = 0, end = 0;
        ifstream testRequestsTxt(filename_input);
        // case to handle file DNE
        if (!testRequestsTxt)
        {
            testRequestsTxt.clear();
            sleep(TIME_SLEEP);
            continue;
        }
        //file is empty
        else if (testRequestsTxt.peek() == std::ifstream::traits_type::eof())
        {
            testRequestsTxt.close(); 
            testRequestsTxt.clear();
            is_asleep = true;
            sleep(TIME_SLEEP);
            continue;
        }
        //file is not-empty
        else
        {
            if (is_processing)
            {
                sleep(TIME_SLEEP);
                continue;
            }
            //reinitialize inputs
            choice = "";
            for(int i = 0; i < PARAMETERS; i++)
            {
                //SET PARAMETER ARRAY TO -1
                if (i != 13) { parameter[i] = "-1"; }
                //LAST ONE SHOULD BE 0 (MODIFY TEMPORARY DATA) FRONT END WILL EXPLICITLY SET THIS BIT TO 1 (PERMANENT MODIFICATION)
                else { parameter[i] = "0"; }
            }
            {
                //STRUCT TO ACCESS TXT MODIFIED TIME
                struct stat fileInfoTxt; 
                int err = stat(filename_input, &fileInfoTxt);
                if (err != 0) 
                { 
                    //IF TO HAVE THE ERROR MESSAGE ONLY ONCE IF REQUEST FILE DNE (REDUNDANT - BUT IT HELPS TO CATCH ALL CASES)
                    if(!requestFileDNEMessage) { perror("request file does not exist"); }
                    requestFileDNEMessage = true; 
                    continue;
                }
                if (requestFileDNEMessage) { requestFileDNEMessage = false; }
                newModTime = fileInfoTxt.st_mtime;
            }
            if (backendNoRequestMessage) { backendNoRequestMessage = false; }
            if (oldModTime < newModTime) 
            {
                ifstream in(filename_input);
                string tempString = "";
                getline(in, tempString);
                in.close(); in.clear();
                stringstream ss(tempString);
                string value;
                if(pastTempString == tempString)
                {
                    is_asleep = true;
                    sleep(TIME_SLEEP);
                    continue; 
                }
                else
                {
                    // PARAMETER + 1 BC WE NEED THE FEATURE NAME TOO WHICH MEANS THAT PARAMETERS WILL BE OFFSET BY 1: <FEATURE> -1 -1 -1 -1 -1 -1 ...
                    for (unsigned i = 0; i < PARAMETERS + 1; i++) 
                    {
                        if (getline(ss,value,' ')) 
                        {
                            if (i == 0) { cout << value << endl; choice = value; }
                            //CAPTURES CASE WHERE FEATURE IS NOT IN REQUEST FILE
                            if (choice == "")
                            {
                                //EARLY ESCAPE TO REDUCE PROCESSING TIME
                                is_asleep = true;
                                break;
                            }
                            if(i == 1) { parameter[i - 1] = (value); }
                            if(i == 2) { parameter[i - 1] = (value); }
                            if(i == 3) { parameter[i - 1] = (value); }
                            if(i == 4) { parameter[i - 1] = (value); }
                            if(i == 5) { parameter[i - 1] = (value); }
                            if(i == 6) { parameter[i - 1] = (value); }
                            if(i == 7) { parameter[i - 1] = (value); }
                            if(i == 8) { parameter[i - 1] = (value); }
                            if(i == 9) { parameter[i - 1] = (value); }
                            if(i == 10) { parameter[i - 1] = (value); }
                            if(i == 11) { parameter[i - 1] = (value); }
                            if(i == 12) { parameter[i - 1] = (value); }
                            if(i == 13) { parameter[i - 1] = (value); }
                            if(i == 14) { parameter[i - 1] = (value); }   
                        }
                    }
                    if (is_asleep)
                    {
                        sleep(TIME_SLEEP);
                        continue; 
                    }

                    //NON FUNCTIONAL, PLEASE TROUBLESHOOT
                    ifstream test;
                    test.open(out12);
                    if(test.is_open())
                    {
                        is_processing = true;
                        cout << "Test" << endl;
                        if (test.peek() != ifstream::traits_type::eof() || test.peek() != '\n'){}
                        else 
                        {
                            cout << "Test 2" << endl; 
                            analytics_predict_winrate("mu", "25.72"); 
                        } // first parameter indicates "mu" or "sigma", second parameter is user-defined trueskill value
                        is_processing = false;
                    }
                    test.close(); test.clear();
                    
                    //for(int i = 0; i < PARAMETERS; i++) { cout << "parameter[" << i << "] = " << parameter[i] << endl; } //debug TO CHECK PARAMETERS
                    //THIS IS WHERE THE REQUESTS ARE CLEARED 
                    ofstream outfile(filename_input, ios::out | ios::trunc); 
                    outfile.close(); outfile.clear();
                    //BECAUSE WE JUST CLEARED REQUESTS WE NEED TO UPDATE MODIFIED TIME AGAIN
                    {
                        struct stat fileInfoTxt; //struct to access txt modified time
                        int err = stat(filename_input, &fileInfoTxt);
                        if (err != 0) 
                        { 
                            //So the error only prints out once
                            if (!requestFileDNEMessage) { perror("request file does not exist"); }
                            requestFileDNEMessage = true; 
                            //continue; //Uncomment this continue once we uncomment while(true)
                            choice = "";
                            continue;
                        }
                        newModTime = fileInfoTxt.st_mtime;
                    }   
                    oldModTime = newModTime;
                    if (choice != "") 
                    {
                        if (tempString != pastTempString) 
                        {
                            //IF REQUEST IS NOT THE SAME AS THE PREVIOUS REQUEST JUST SUBMITTED 
                            past_choice = choice; 
                            cout << "New request detected." << endl;
                            cout << "Servicing Request: " << choice << endl;
                        }
                        else 
                        { 
                            //IF REQUEST IS THE SAME
                            choice = "";
                            is_asleep = true;
                            sleep(TIME_SLEEP);
                            continue; 
                        }
                    }
                    else 
                    {
                        //REDUNDANT CHECK IF CHOICE IS EMPTY - BUT HELPS TO HAVE
                        choice = "";
                        is_asleep = true;
                        sleep(TIME_SLEEP);
                        continue;
                    }

                    /***** START OF ACCEPTING REQUESTS *****/
                    /*BEGINNING OF SEARCHES*/
                    /***** SEARCH REQUESTS *****/
                    // FEATURE: SEARCH ability (top abilities)
                    if (choice == "search_top_abilities")
                    {    
                        //cout << "working" << endl; // Sanity Check
                        if (limit.at(0) == parameter[0] && !updateSearch[0])
                        { 
                            cout << "Skip " << choice << ". Nothing new to display." << endl;
                            continue; 
                        }
                        limit.at(0) = parameter[0]; updateSearch[0] = false;
                        time(&start);
                        is_processing = true;    
                        if(modification_counter[0] >= 0) { 
                            search_top_abilities(parameter[0],modified[0]); 
                            modification_counter[0] = 0; /* reset the modified counter since the analytics has already been displayed */
                        }
                        time(&end);
                        double time_taken = double(end - start);
                        cout << "Time taken by " << choice << " feature is: " << fixed << time_taken << setprecision(5);
                        cout << " sec" << endl;
                    } 
                    
                    // FEATURE: SEARCH heroes
                    else if (choice == "search_heroes")
                    {
                        if (limit.at(1) == parameter[0] && !updateSearch[1])
                        { 
                            cout << "Skip " << choice << ". Nothing new to display." << endl;
                            continue; 
                        }
                        limit.at(1) = parameter[0]; updateSearch[1] = false;
                        time(&start);
                        is_processing = true; 
                        search_heroes(parameter[0], modified[1], tmp_new, modified[3]); 
                        time(&end);
                        double time_taken = double(end - start);
                        cout << "Time taken by " << choice << " feature is: " << fixed <<  time_taken << setprecision(5);
                        cout << " sec" << endl;
                    }
                    // FEATURE: SEARCH players
                    else if (choice == "search_players")
                    {
                        if (limit.at(2) == parameter[0] && !updateSearch[2])
                        { 
                            cout << "Skip " << choice << ". Nothing new to display." << endl;
                            continue; 
                        }
                        limit.at(2) = parameter[0]; updateSearch[2] = false;
                        time(&start);
                        is_processing = true;  
                        search_players(parameter[0], modified[2], tmp_new);
                        time(&end);
                        double time_taken = double(end - start);
                        cout << "Time taken by " << choice << " feature is: " << fixed <<  time_taken << setprecision(5);
                        cout << " sec" << endl;
                    }
                    // FEATURE: SEARCH winning matches by first blood time
                    else if (choice == "search_first_blood_time")
                    {
                        if (limit.at(3) == parameter[0] && !updateSearch[3])
                        { 
                            cout << "Skip " << choice << ". Nothing new to display." << endl;
                            continue; 
                        }
                        limit.at(3) = parameter[0]; updateSearch[3] = false;
                        time(&start);
                        is_processing = true;  
                        search_first_blood_time(parameter[0], modified[3], tmp_new);
                        time(&end);
                        double time_taken = double(end - start);
                        cout << "Time taken by " << choice << " feature is: " << fixed <<  time_taken << setprecision(5);
                        cout << " sec" << endl;
                    }

                    // FEATURE: SEARCH matches by cluster region
                    else if (choice == "search_cluster_region") 
                    {
                        if (limit.at(4) == parameter[0] && !updateSearch[4])
                        { 
                            cout << "Skip " << choice << ". Nothing new to display." << endl;
                            continue; 
                        }
                        limit.at(4) = parameter[0]; updateSearch[4] = false;
                        time(&start);
                        is_processing = true;  
                        search_cluster_region(parameter[0],modified[3],tmp_new);
                        time(&end);
                        double time_taken = double(end - start);
                        cout << "Time taken by " << choice << " feature is: " << fixed <<  time_taken << setprecision(5);
                        cout << " sec" << endl;
                    }
                    // FEATURE: SEARCH matches by most likes
                    else if (choice == "search_most_liked_match") 
                    {
                        if (limit.at(5) == parameter[0] && !updateSearch[5])
                        { 
                            cout << "Skip " << choice << ". Nothing new to display." << endl;
                            continue; 
                        }
                        limit.at(5) = parameter[0]; updateSearch[5] = false;
                        time(&start);
                        is_processing = true;  
                        search_most_liked_match(parameter[0], modified[3], tmp_new);
                        time(&end);
                        double time_taken = double(end - start);
                        cout << "Time taken by " << choice << " feature is: " << fixed <<  time_taken << setprecision(5);
                        cout << " sec" << endl;
                    }
                    /*****BEGINNING OF INSERT REQUESTS *****/
                    // FEATURE: INSERT ability
                    else if (choice == "insert_ability")
                    {
                        time(&start);
                        is_processing = true;
                        insert_ability(parameter[0],parameter[1], parameter[13], modified[0]);
                        modified[0] = true; 
                        modification_counter[0]++; //increment ability upgrade modifications
                        if (modification_counter[0] == 10) { updateSearch[0] = true; }
                        time(&end);
                        double time_taken = double(end - start);
                        cout << "\nTime taken by " << choice << " feature is: " << fixed <<  time_taken << setprecision(5);
                        cout << " sec" << endl;
                    }
                    
                    // FEATURE: INSERT hero
                    else if (choice == "insert_hero") 
                    {
                        time(&start);
                        insert_hero(parameter, modified[1], modification_counter[1], updateSearch[1], tmp_new);
                        time(&end);
                        double time_taken = double (end - start);
                        cout << "\nTime taken by " << choice << " feature is: " << fixed << time_taken << setprecision(5);
                        cout << " sec" << endl; 
                    }
                    
                    // FEATURE: INSERT player
                    else if (choice == "insert_player")
                    {
                        time(&start);
                        is_processing = true;
                        modification_counter[3]++;
                        insert_player(parameter, modified[2], updateSearch[2], tmp_new);
                        time(&end);
                        double time_taken = double(end - start);
                        cout << "\nTime taken by " << choice << " feature is: " << fixed <<  time_taken << setprecision(5);
                        cout << " sec" << endl;
                    }

                    // FEATURE: INSERT match
                    else if (choice == "insert_match") 
                    {
                        time(&start);
                        is_processing = true;
                        insert_match(parameter, modified[3], modification_counter[2], updateSearch[3], updateSearch[4], updateSearch[5], tmp_new);
                        time(&end);
                        double time_taken = double(end - start);
                        cout << "\nTime taken by " << choice << " feature is: " << fixed << time_taken << setprecision(5);
                        cout << " sec" << endl; 
                        modification_counter[2]++;
                    }

                    //FEATURE: add_ability_upgrades
                    else if (choice == "add_ability_upgrades"){
                        time(&start);
                        is_processing = true;
                        add_ability_upgrades(parameter);
                        modified[0] = true; 
                        modification_counter[0]++;
                        if (modification_counter[0] == 10) { updateSearch[0] = true; }
                        time(&end);
                        double time_taken = double(end - start);
                        cout << "\nTime taken by " << choice << " feature is: " << fixed << time_taken << setprecision(5);
                        cout << " sec" << endl; 
                    }

                    /***** UPDATE REQUESTS *****/
                    // FEATURE: UPDATE ability
                    else if (choice == "update_ability")
                    {
                        time(&start);
                        is_processing = true;
                        update_ability(parameter, modified[0], updateSearch[0]);
                        time(&end);
                        double time_taken = double(end - start);
                        cout << "\nTime taken by " << choice << " feature is: " << fixed <<  time_taken << setprecision(5);
                        cout << " sec" << endl;
                    }

                    // FEATURE: UPDATE hero
                    else if (choice == "update_hero") 
                    {
                        time(&start);
                        is_processing = true;
                        update_hero(parameter, modified[1], updateSearch[1], tmp_new);
                        time(&end);
                        double time_taken = double(end - start);
                        cout << "\nTime taken by " << choice << " feature is: " << fixed <<  time_taken << setprecision(5);
                        cout << " sec" << endl;
                    }

                    // FEATURE: UPDATE player
                    else if (choice == "update_player")
                    {
                        time(&start);
                        is_processing = true;
                        
                        update_player(parameter, modified[2], updateSearch[2], tmp_new);
                        time(&end);
                        double time_taken = double(end - start);
                        cout << "\nTime taken by " << choice << " feature is: " << fixed << time_taken << setprecision(5);
                        cout << " sec" << endl; 
                    }

                    // FEATURE: UPDATE match
                    else if (choice == "update_match") 
                    {
                        time(&start);
                        is_processing = true;
                        update_match(parameter, modified[3], updateSearch[3], updateSearch[4], updateSearch[5]);
                        time(&end);
                        double time_taken = double(end - start);
                        cout << "\nTime taken by " << choice << " feature is: " << fixed << time_taken << setprecision(5);
                        cout << " sec" << endl; 
                    }

                    /***** DELETE REQUESTS *****/
                    // FEATURE: DELETE ability
                    else if(choice == "delete_ability")
                    {
                        time(&start);
                        is_processing = true;
                        delete_ability(parameter[0], modified[0], stoi(parameter[13]), updateSearch[0], tmp_new);
                        time(&end);
                        double time_taken = double(end - start);
                        cout << "\nTime taken by " << choice << " feature is: " << fixed <<  time_taken << setprecision(5);
                        cout << " sec" << endl;    
                    }

                    // FEATURE: DELETE hero
                    else if (choice == "delete_hero")
                    {
                        time(&start);
                        is_processing = true;
                        delete_hero(parameter[0], modified[1], updateSearch[1], tmp_new);
                        time(&end);
                        double time_taken = double(end - start);
                        cout << "\nTime taken by " << choice << " feature is: " << fixed <<  time_taken << setprecision(5);
                        cout << " sec" << endl;
                    }
                    
                    // FEATURE: DELETE player
                    else if (choice == "delete_player") 
                    {
                        time(&start);
                        is_processing = true;
                        delete_player(parameter[0], modified[2], updateSearch[2], tmp_new);
                        time(&end);
                        double time_taken = double(end - start);
                        cout << "\nTime taken by " << choice << " feature is: " << fixed <<  time_taken << setprecision(5);
                        cout << " sec" << endl;
                    }

                    // FEATURE: DELETE match
                    else if (choice == "delete_match") 
                    {
                        time(&start);
                        is_processing = true;
                        delete_match(parameter[0], modified[3], stoi(parameter[13]), updateSearch[3], updateSearch[4], updateSearch[5], tmp_new);
                        time(&end);
                        double time_taken = double(end - start);
                        cout << "\nTime taken by " << choice << " feature is: " << fixed <<  time_taken << setprecision(5);
                        cout << " sec" << endl;
                    }


                    /***** BACKUP REQUESTS *****/
                    // FEATURE: BACKUP ability
                    else if (choice == "backup_ability")
                    {
                        time(&start);
                        is_processing = true;
                        backup_ability(modified[0], backup[0]);
                        time(&end);
                        double time_taken = double(end - start);
                        cout << "\nTime taken by " << choice << " feature is: " << fixed <<  time_taken << setprecision(5);
                        cout << " sec" << endl;
                    }
                    
                    // FEATURE: BACKUP hero
                    else if (choice == "backup_hero") 
                    {
                        time(&start);
                        is_processing = true;
                        backup_hero(modified[1], backup[1]);
                        time(&end);
                        double time_taken = double(end - start);
                        cout << "\nTime taken by " << choice << " feature is: " << fixed <<  time_taken << setprecision(5);
                        cout << " sec" << endl;
                    }
                    
                    // FEATURE: BACKUP player
                    else if (choice == "backup_player") 
                    {
                        time(&start);
                        is_processing = true;
                        backup_player(modified[2], backup[2]);
                        time(&end);
                        double time_taken = double(end - start);
                        cout << "\nTime taken by " << choice << " feature is: " << fixed <<  time_taken << setprecision(5);
                        cout << " sec" << endl;
                    }

                    // FEATURE: BACKUP match
                    else if (choice == "backup_match")
                    {
                        time(&start);
                        is_processing = true;
                        backup_match(modified[3], backup[3]);
                        time(&end);
                        double time_taken = double(end - start);
                        cout << "\nTime taken by " << choice << " feature is: " << fixed <<  time_taken << setprecision(5);
                        cout << " sec" << endl;
                    }


                    /***** IMPORT REQUESTS *****/
                    // FEATURE: IMPORT ability
                    else if (choice == "import_ability")
                    {
                        time(&start);
                        is_processing = true;
                        import_ability(backup[0], updateSearch[0]);
                        time(&end);
                        double time_taken = double(end - start);
                        cout << "\nTime taken by " << choice << " feature is: " << fixed <<  time_taken << setprecision(5);
                        cout << " sec" << endl;
                    }

                    // FEATURE: IMPORT hero
                    else if (choice == "import_hero") 
                    {
                        time(&start);
                        is_processing = true;
                        import_hero(backup[1], updateSearch[1]);
                        time(&end);
                        double time_taken = double(end - start);
                        cout << "\nTime taken by " << choice << " feature is: " << fixed <<  time_taken << setprecision(5);
                        cout << " sec" << endl;
                    }

                    // Feature: Import player
                    else if (choice == "import_player") 
                    {
                        time(&start);
                        is_processing = true;
                        import_player(backup[2], updateSearch[2]);
                        time(&end);
                        double time_taken = double(end - start);
                        cout << "\nTime taken by " << choice << " feature is: " << fixed <<  time_taken << setprecision(5);
                        cout << " sec" << endl;
                    }

                    // FEATURE: IMPORT match
                    else if (choice == "import_match") 
                    {
                        time(&start);
                        is_processing = true;
                        import_match(backup[3], updateSearch[3], updateSearch[4], updateSearch[5]);
                        time(&end);
                        double time_taken = double(end - start);
                        cout << "\nTime taken by " << choice << " feature is: " << fixed <<  time_taken << setprecision(5);
                        cout << " sec" << endl;
                    }

                    /***** ANALYTICS REQUESTS *****/ //we might not even have any here anymore
                    else
                    {
                        // cout << "Invalid Request: " << choice << endl;
                        choice = "";
                        continue;
                    }

                    if (choice != "") 
                    {
                        cout << "Done Servicing Request: " << choice << endl;
                        cout << "oldModTime = " << oldModTime << endl; //debug
                        cout << "Cleared requests queue" << endl;
                        past_choice = choice;
                        pastTempString = tempString;
                        choice = "";
                        is_processing = false;
                    }
                }
            }
            else 
            { 
                if (!backendNoRequestMessage) { backendNoRequestMessage = true; } // limit the cout to one time only
                continue; /*Uncomment this continue once we uncomment while(true)*/ 
                if (backendNoRequestMessage) { cout << "No new Requests" << endl; }
            }
        }
    }
}