#include <map>
#include "submanager_ability.h"
#include "submanager_ability_upgrades.h"
#include "submanager_heroNames.h"
#include "submanager_objectives.h"
#include "submanager_players.h"
#include "submanager_playerRatings.h"
#include "submanager_match.h"
#include "submanager_cluster.h"
#include <iostream>
#include <iomanip>
#include <sys/stat.h>
#include <ctime>
#include <iterator>
#include <locale>
#include <queue>
#include <bits/stdc++.h>
#include <unistd.h>
#include "defines.h"
#include <map>

// helper function to sort in top abilties
bool sortbysecdesc(const std::pair<int,int> &a, const std::pair<int,int> &b);


/** ANALYTICS FUNCTIONS **/
// FEATURE: SEARCH WINRATES BY TIME, SPECIFIC HERO (analytics)
// param 1 - limit of heroes to do analytics for
// param 2 - boolean used to describe if data is modified (as in, insert/delete/update)
void analytics_winrates_over_time(std::string numHeroes, bool modifier);

// FEATURE: Display first blood winrate (analytics)
// param 1 - limit of matches to print results for
// param 2 - boolean used to describe if data is modified (as in, insert/delete/update)
void analytics_first_blood_winrate(std::string numMatches, bool modifier);

// FEATURE: Search for winrates VS other heroes (analytics)
// param 1 -  number of matches to process (must be large to get any useful info)
// param 2 - starting hero ID to process
// param 3 - last hero ID to process
// param 4 - boolean used to describe if data is modified (as in, insert/delete/update)
void analytics_search_vs_winrate(std::string numMatches, std::string startHeroId, std::string endHeroId, bool modifier);

// FEATURE: Predict winrates by user-defined player ratings
// param 1 - The type of rating to consider (mu, sigma, etc)
// param 2 - The value of the specified rating type
void analytics_predict_winrate(std::string ratingType, std::string ratingVal);

// FEATURE: Finds average xp and gold per minute for each hero based on a certain number of matches
// param 1 - limit on the number of matches taken into consideration
// param 2 - number of Heroes to perform comparison on
// param 3 - boolean used to describe if data is modified (as in, insert/delete/update)
void analytics_compare_xp_and_gold_heroes(std::string limit, std::string numHeroes, bool modified1, bool modified2);

/** SEARCH FUNCTIONS **/
// FEATURE: SEARCH top abilities upgraded
// param 1 - limit of abilities to retrieve data for
// param 2 - boolean used to determine if files were modified
void search_top_abilities(std::string limit, bool modifier);

// FEATURE: SEARCH top abilities upgraded
// param 1 - limit of heroes to retrieve data for
// param 2 - boolean used to determine if files were modified
// param 3 - temporary vector to hold newly inserted heroes
// param 4 - boolean to check if matches were modified
void search_heroes(std::string limit, bool modifier, std::vector<std::vector<std::string>> tmp_new, bool);

// FEATURE: SEARCH top abilities upgraded
// param 1 - limit of players to retrieve data for
// param 2 - boolean used to determine if files were modified
// param 3 - temporary vector to hold newly inserted players
void search_players(std::string limit, bool modified2, std::vector<std::vector<std::string>> tmp_new);

// FEATURE: SEARCH top abilities upgraded
// param 1 - limit of matches to retrieve data for
// param 2 - boolean used to determine if files were modified
// param 3 - temporary vector to hold newly inserted matches
void search_first_blood_time(std::string limit, bool modified3, std::vector<std::vector<std::string>> tmp_new);

// FEATURE: SEARCH top abilities upgraded
// param 1 - limit of matches to retrieve data for
// param 2 - boolean used to determine if files were modified
// param 3 - temporary vector to hold newly inserted matches
void search_cluster_region(std::string limit, bool modified3, std::vector<std::vector<std::string>> tmp_new);

// FEATURE: SEARCH top abilities upgraded
// param 1 - limit of matches to retrieve data for
// param 2 - boolean used to determine if files were modified
// param 3 - temporary vector to hold newly inserted matches
void search_most_liked_match(std::string limit, bool modified3, std::vector<std::vector<std::string>> tmp_new);

/** INSERT FUNCTIONS **/
// param 1 - ability name
// param 2 - upgrade count
// param 3 - flag to write file
// param 4 - check if file was modified, set to true after a successful insert
void insert_ability(std::string parameter0, std::string parameter1, std::string parameter13, bool modified0);

// FEATURE: INSERT hero
// param 1 - parameters passed in (from request txt file)
// param 2 - check if file was modified, set to true after a successful update
// param 3 - modification counter (for analytics)
// param 4 - toggle to update search heroes
// param 5 - temporary data vector (for newly inserted data)
void insert_hero(const std::string parameter[], bool &modifier, int &mod_cnt, bool &update, std::vector<std::vector<std::string>> &tmp_new);

// FEATURE: INSERT player
// param 1 - parameters passed in (from request txt file)
// param 2 - check if file was modified, set to true after a successful update
// param 3 - toggle to update search players
// param 4 - temporary data vector (for newly inserted data)
void insert_player(const std::string parameter[], bool &modifier, bool &update, std::vector<std::vector<std::string>> &tmp_new);

// FEATURE: INSERT match
// param 1 - parameters passed in (from request txt file)
// param 2 - check if file was modified, set to true after a successful update
// param 3 - modification counter (for analytics)
// param 4 - toggle to update search matches by first blood
// param 5 - toggle to update search matches by cluster region
// param 6 - toggle to update search matches by most liked
// param 7 - temporary data vector (for newly inserted data)
void insert_match(const std::string parameter[], bool &modifier, int &mod_cnt, bool &updateFB, bool &updateCR, bool &updateML, std::vector<std::vector<std::string>> &tmp_new);

//FEATURE: ADD ability upgrades
//param 1 - ability id
//param 2 - level
//param 3 - time
//param 4 - playerslot
//param 5 - matchid
void add_ability_upgrades(const std::string parameter[]);


/** UPDATE FUNCTIONS **/
// FEATURE: UPDATE ability
// param 1 - parameters passed in (from request txt file)
// param 2 - check if file was modified, set to true after a successful update
// param 3 - toggle to update search abilites
void update_ability(const std::string parameter[], bool &modifier, bool &update);

// FEATURE: UPDATE hero
// param 1 - parameters passed in (from request txt file)
// param 2 - check if file was modified, set to true after a successful update
// param 3 - toggle to update search heroes
// param 4 - temporary data vector (for newly inserted data)
void update_hero(const std::string parameter[], bool &modifier, bool &update, std::vector<std::vector<std::string>> &tmp_new);

// FEATURE: UPDATE player
// param 1 - parameters passed in (from request txt file)
// param 2 - check if file was modified, set to true after a successful update
// param 3 - toggle to update search players
// param 4 - temporary data vector (for newly inserted data)
void update_player(const std::string parameter[], bool &modifier, bool &update, std::vector<std::vector<std::string>> &tmp_new);

// FEATURE: UPDATE match
// param 1 - parameters passed in (from request txt file)
// param 2 - check if file was modified, set to true after a successful update
// param 3 - toggle to update search matches by first blood
// param 4 - toggle to update search matches by cluster region
// param 5 - toggle to update search matches by most liked
void update_match(const std::string parameter[], bool &modifier, bool &updateFB, bool &updateCR, bool &updateML);

/** DELETE FUNCTIONS **/
// FEATURE: DELETE ability
// param 1 - id to delete by
// param 2 - check if file was modified, set to true after a successful delete
// param 3 - flag to write to file
// param 4 - toggle to update search abilities
// param 5 - temporary data vector (for newly inserted data)
void delete_ability(std::string id, bool &modifier, bool flag, bool &update, std::vector<std::vector<std::string>> &tmp_new);

// FEATURE: DELETE hero
// param 1 - id to delete by
// param 2 - check if file was modified, set to true after a successful delete
// param 3 - toggle to update search heroes
// param 4 - temporary data vector (for newly inserted data)
void delete_hero(std::string id, bool &modifier,  bool &update, std::vector<std::vector<std::string>> &tmp_new);

// FEATURE: DELETE player
// param 1 - id to delete by
// param 2 - check if file was modified, set to true after a successful delete
// param 3 - toggle to update search players
// param 4 - temporary data vector (for newly inserted data)
void delete_player(std::string id, bool &modifier, bool &update, std::vector<std::vector<std::string>> &tmp_new);

// FEATURE: DELETE match
// param 1 - id to delete by
// param 2 - check if file was modified, set to true after a successful delete
// param 3 - flag to write to file
// param 4 - toggle to update search matches by first blood
// param 5 - toggle to update search matches by cluster region
// param 6 - toggle to update search matches by most liked
// param 7 - temporary data vector (for newly inserted data)
void delete_match(std::string id, bool &modifier, bool flag, bool &updateFB, bool &updateCR, bool &updateML, std::vector<std::vector<std::string>> &tmp_new);

/** BACKUP FUNCTIONS **/
// FEATURE: BACKUP ability
// param 1 - if respective file was modified
// param 2 - set backup true to indicate success
void backup_ability(bool modified, bool &backup);

// FEATURE: BACKUP hero
// param 1 - if respective file was modified
// param 2 - set backup true to indicate success
void backup_hero(bool modified, bool &backup);

// FEATURE: BACKUP player
// param 1 - if respective file was modified
// param 2 - set backup true to indicate success
void backup_player(bool modified, bool &backup);

// FEATURE: BACKUP match
// param 1 - if respective file was modified
// param 2 - set backup true to indicate success
void backup_match(bool modified, bool &backup);

/** IMPORT FUNCTIONS **/
// FEATURE: IMPORT abilities
// param 1 - backup is valid
// param 2 - toggle to update search abilities
void import_ability(bool backup, bool &update);

// FEATURE: IMPORT heroes
// param 1 - backup is valid
// param 2 - toggle to update search heroes
void import_hero(bool backup, bool &update);

// FEATURE: IMPORT player
// param 1 - backup is valid
// param 2 - toggle to update search players
void import_player(bool backup, bool &update);

// FEATURE: IMPORT match
// param 1 - backup is valid
// param 2 - toggle to update search matches by first blood
// param 3 - toggle to update search matches by cluster region
// param 4 - toggle to update search matches by most liked
void import_match(bool backup, bool &updateFB, bool &updateCR, bool &updateML);