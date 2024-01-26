#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h> 
#include <time.h>
#include <vector>
#include <string>
#include <sstream>
#include <cstring>

using std::string;
using std::vector;

#ifndef CPP_SQL_H
#define CPP_SQL_H

class DbConnect 
{
    public:
        struct PlayerData
        {
            string playerName;
            int playerAge;
            string alignment;
            vector<string> enemies;

            int con;                            //  Constitution
            int dex;                            //  Dexterity
            int str;                            //  Strength
            int intel;                          //  Intelligence
            int wis;                            //  Wisdom
            int cha;                            //  Charisma

            string playerRace;
            string playerClass;
            vector<string> langs;               //  Known languages

            int level;
            int castingLevel;
            double hp;
            double xp;
            int spellsPerDay;                   //  The number of spells a player can cast between each daily reset
            int numOfSpells;                    //  Number of spells known by the player
            int attacks_per_round;              //  Number of attacks per round of combat

            int gold;
            vector<string> inventory;
            double encumberance;
            string spells;
            vector<string> allowed_weapons;
            vector<string> allowed_armor;

            vector<string> base_mods;           //  Passive modifiers to base ability scores
            vector<string> racial_abilities;
            vector<string> class_abilities;
            vector<string> profficiencies;
            vector<string> magicSchools;
            string sphere;

            string mesh;
        };
        PlayerData player;

        void writeCharacter(PlayerData& player);
        PlayerData readCharacter(int id);
    private:
        sqlite3 *db;                                                                        //  SQL database handle
        sqlite3_stmt *stmt;                                                                 //  Used to step through and edit the SQL statement
        char *zErrMsg;                                                                      //  SQL error message
        char *sql;                                                                          //  The prepared SQL statement
        int rc;                                                                             //  Return code: returned values from calls to the SQL API
        int index;                                                                          //  An incrementing number used to represent the index position of database columns

        char *getDateTime();
        time_t seaconds;                                                                    //  The current number of seconds passed since January 1st 1970
        char *currentTime;                                                                  //  The return value of the getDateTime() function

        int generateUuid();
        int id;                                                                             //  An ID (PK) for CHARACTERS database entries

        const char *vecToChar(vector<string> vec);
        vector<string> charToVec(const unsigned char *input);
};

#endif