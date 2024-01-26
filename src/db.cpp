#include "../hdr/db.h"

using std::string;
using std::vector;
using std::stringstream;

#define GREEN_TEXT "\x1b[32m"
#define RESET_TEXT "\x1b[37m" 

int DbConnect::generateUuid() {                                                                                                  // Generates a uuid for the character db entry
   srand(time((0)));                                                                                                             // Seed a random number based on current time

   return 1 + (rand() % 2147483647);                                                                                             // Generate a random number between the current seed and INT_MAX
}

char *DbConnect::getDateTime() {                                                                                                 // Fetches the current time of upload to be stored with character data
   seaconds = time(&seaconds);                                                                                                   // Get the number of seconds passed since January 1st 1970

   return ctime(&seaconds);                                                                                                      // Convert seconds to UTC datetime
}

const char * DbConnect::vecToChar(vector<string> vec) {                                                                          // Converts vectors of strings to a char* seperating each index with a comma delimeter
   string stringBuffer;                                                                                                          // Declare a variable to temporarily store string before passing it to the player object
   string delimeter = ", ";                                                                                                      // Declare the delimeter as a comma followed by whitespace

   for(auto i: vec) {                                                                                                            // Loop through the input vector
      stringBuffer.append(i.append(delimeter));                                                                                  // Append the delimeter to the value located at each index before appending to the buffer
   };

   stringBuffer.erase(stringBuffer.size() - 2);                                                                                  // Erase the trailing delimeter
   char *cstr = new char[stringBuffer.length() + 1];  // This is a memory leak, call delete on this variable                     // Create a pointer to a memory address with size equal to the length of the buffer
   std::strcpy(cstr, stringBuffer.c_str());                                                                                      // Copy the buffer to the aforementioned memory location
   return cstr;                                                                                                                  // Return the pointer
}

vector<string> DbConnect::charToVec(const unsigned char *input) {                                                                // Converts a const unsigned char* to a vector<string>                                                                                       // 
   vector<string> vecBuffer;                                                                                                     // Declare a variable to temporarily store a vector before passing it to the player obj

   string tempString = string(reinterpret_cast<const char*>(input));                                                             // Convert the char* input to string
   stringstream ss(tempString);                                                                                                  // Load the aformentioned string into a stream

   char delimeter = ',';                                                                                                         // Declare the delimeter as a comma

   while( ss.good() ) {                                                                                                          // Loop through the stream while ss>streamstart && ss<streamend && ss != error
      string substr;                                                                                                             // Declare a variable to store substrings from within the string
      std::getline( ss, substr, delimeter);                                                                                      // Identify substring seperated by delimeters, store them in the aformentioned variable
      vecBuffer.push_back( substr );                                                                                             // Push the substring into the buffer
   }

   return vecBuffer;                                                                                                             // Return the buffer
}

void DbConnect::writeCharacter(PlayerData& player) {                                                                         // Writes a character to the database
   rc = sqlite3_open("characters.db", &db);                                                                                      // Open the CHARACTERS database, pass a memory location to store reference to the db
   id = generateUuid();                                                                                                          // Call the uuid generator function and assign return value to a variable
   currentTime = getDateTime();                                                                                                  // Call the datetime function and assign return value to a variable

   if( rc ) {                                                                                                                    // Admitedly I don't quite understand why this works it feels backwards
      fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
   } else {
      fprintf(stderr, "Opened database successfully\n");
   }
                                                                                                                                 // INSERT SQL statement defining a target table and its' respective columns
   sql = "INSERT INTO CHARACTERS (ID, PLAYER_NAME, PLAYER_AGE, DATE, ALIGNMENT, ENEMIES, CON, DEX, INTEL, WIS, CHA, " \
            "PLAYER_RACE, PLAYER_CLASS, LANGS, LEVEL, CASTING_LEVEL, HP, XP, SPELLS_PER_DAY, NUM_OF_SPELLS, " \
            "ATTACKS_PER_ROUND, GOLD, INVENTORY, ENCUMBERANCE, SPELLS, ALLOWED_WEAPONS, ALLOWED_ARMOR, BASE_MODS, " \
            "RACIAL_ABILITIES, CLASS_ABILITIES, PROFFICIENCIES, MAGIC_SCHOOLS, SPHERE, MESH) "  
         "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?); ";         // SQL statement defining column values as parameters

   rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);                                                                         // Prepare the SQL statement

   index = 1;                                                                                                                    // Seed a column-index (1 based) value

   const char* PLACEHOLDER = "none";                                                                                             // Declare a placeholder variable for non-functional player-class-members
                                                                                                                                 // Bind values to SQL statement parameters
   sqlite3_bind_int(stmt, index++, id);                                                                                          // ID
   sqlite3_bind_text(stmt, index++, player.playerName.c_str(), -1, SQLITE_STATIC);                                               // PLAYER_NAME
   sqlite3_bind_int(stmt, index++, player.playerAge);                                                                            // PLAYER_AGE
   sqlite3_bind_text(stmt, index++, currentTime, -1, SQLITE_STATIC);                                                             // DATE
   sqlite3_bind_text(stmt, index++, player.alignment.c_str(), -1, SQLITE_STATIC);                                                // ALIGNMENT
   sqlite3_bind_text(stmt, index++, vecToChar(player.enemies), -1, SQLITE_STATIC);                                               // ENEMIES
   sqlite3_bind_int(stmt, index++, player.con);                                                                                  // CON
   sqlite3_bind_int(stmt, index++, player.dex);                                                                                  // DEX
   sqlite3_bind_int(stmt, index++, player.intel);                                                                                // INTEL
   sqlite3_bind_int(stmt, index++, player.wis);                                                                                  // WIS
   sqlite3_bind_int(stmt, index++, player.cha);                                                                                  // CHA
   sqlite3_bind_text(stmt, index++, player.playerRace.c_str(), -1, SQLITE_STATIC);                                               // PLAYER_RACE
   sqlite3_bind_text(stmt, index++, player.playerClass.c_str(), -1, SQLITE_STATIC);                                              // PLAYER_CLASS
   sqlite3_bind_text(stmt, index++, vecToChar(player.langs), -1, SQLITE_STATIC);                                                 // LANGS
   sqlite3_bind_int(stmt, index++, player.level);                                                                                // LEVEL 
   sqlite3_bind_int(stmt, index++, player.castingLevel);                                                                         // CASTING_LEVEL
   sqlite3_bind_double(stmt, index++, player.hp);                                                                                // HP
   sqlite3_bind_double(stmt, index++, player.xp);                                                                                // XP
   sqlite3_bind_int(stmt, index++, player.spellsPerDay);                                                                         // SPELLS_PER_DAY
   sqlite3_bind_int(stmt, index++, player.numOfSpells);                                                                          // NUM_OF_SPELLS
   sqlite3_bind_int(stmt, index++, player.attacks_per_round);                                                                    // ATTACKS_PER_ROUND
   sqlite3_bind_int(stmt, index++, player.gold);                                                                                 // GOLD
   sqlite3_bind_text(stmt, index++, vecToChar(player.inventory), -1, SQLITE_STATIC);                                             // INVENTORY
   sqlite3_bind_double(stmt, index++, player.encumberance);                                                                      // ENCUMBERANCE
   sqlite3_bind_text(stmt, index++, player.spells.c_str(), -1, SQLITE_STATIC);                                                   // SPELLS
   sqlite3_bind_text(stmt, index++, vecToChar(player.allowed_weapons), -1, SQLITE_STATIC);                                       // ALLOWED_WEAPONS
   sqlite3_bind_text(stmt, index++, vecToChar(player.allowed_armor), -1, SQLITE_STATIC);                                         // ALLOWED_ARMOR
   sqlite3_bind_text(stmt, index++, vecToChar(player.base_mods), -1, SQLITE_STATIC);                                             // BASE_MODS
   sqlite3_bind_text(stmt, index++, vecToChar(player.racial_abilities), -1, SQLITE_STATIC);                                      // RACIAL_ABILITIES
   sqlite3_bind_text(stmt, index++, vecToChar(player.class_abilities), -1, SQLITE_STATIC);                                       // CLASS_ABILITIES
   sqlite3_bind_text(stmt, index++, vecToChar(player.profficiencies), -1, SQLITE_STATIC);                                        // PROFFICIENCIES
   sqlite3_bind_text(stmt, index++, vecToChar(player.magicSchools), -1, SQLITE_STATIC);                                          // MAGIC_SCHOOLS
   sqlite3_bind_text(stmt, index++, PLACEHOLDER, -1, SQLITE_STATIC);                                                             // SPHERE               *include ui option and characterCreation construction
   sqlite3_bind_text(stmt, index++, player.mesh.c_str(), -1, SQLITE_STATIC);                                                     // MESH

   rc = sqlite3_step(stmt);                                                                                                      // Step through and execute the SQL statement
   
   if( rc != SQLITE_DONE ){                                                                                                      // If, by this point stepping is not complete
      fprintf(stderr, "SQL error: %s\n", zErrMsg);                                                                               // Throw an error
      sqlite3_free(zErrMsg);                                                                                                     // Free the error from memory
   } else {                                                                                                                      // Otherwise
      std::cout << GREEN_TEXT << std::endl;                                                                                      
      std::cout << "Write success." << std::endl;                                                                                // Print to terminal
      std::cout << RESET_TEXT << std::endl;
   }
   sqlite3_finalize(stmt);                                                                                                       // Destroy the SQL statement and free from memory   
   sqlite3_close(db);                                                                                                            // Close the database
}

DbConnect::PlayerData DbConnect::readCharacter(int id) {                                                                                // maps CHARACTERS db SQL data to a player object
   rc = sqlite3_open("characters.db", &db);                                                                                      // Open the CHARACTERS database, pass a memory location to store reference to the db

   if( rc ) {                                                                                                                    // Admitedly I don't quite understand why this works it feels backwards
      fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
   } else {
      fprintf(stderr, "Opened database successfully\n");
   }

   sql = "SELECT * FROM CHARACTERS WHERE ID=?;";                                                                                 // SELECT SQL statement defining a target table with a character ID as a parameter

   rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);                                                                         // Prepare the SQL statement

   sqlite3_bind_int(stmt, 1, id);                                                                                                // Bind the input id to the SQL statements parameter

   rc = sqlite3_step(stmt);                                                                                                      // Step through the SQL statement

   if(rc == SQLITE_ROW) {                                                                                                        // If the current step has reached a row in the column, bind the value to the player obj
      player.playerName = string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));                                   // PLAYER_NAME
      player.playerAge = sqlite3_column_int(stmt, 2);                                                                            // PLAYER_AGE
      player.alignment = string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4)));                                    // ALIGNMENT
      player.enemies = charToVec(sqlite3_column_text(stmt, 5));                                                                  // ENEMIES
      player.con = sqlite3_column_int(stmt, 6);                                                                                  // CON
      player.dex = sqlite3_column_int(stmt, 7);                                                                                  // DEX
      player.intel = sqlite3_column_int(stmt, 8);                                                                                // INTEL
      player.wis = sqlite3_column_int(stmt, 9);                                                                                  // WIS
      player.cha = sqlite3_column_int(stmt, 10);                                                                                 // CHA
      player.playerRace = string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 11)));                                  // PLAYER_RACE
      player.playerClass = string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 12)));                                 // PLAYER_CLASS
      player.langs = charToVec(sqlite3_column_text(stmt, 13));                                                                   // LANGS
      player.level = sqlite3_column_int(stmt, 14);                                                                               // LEVEL
      player.castingLevel = sqlite3_column_int(stmt, 15);                                                                        // CASTING_LEVEL
      player.hp = sqlite3_column_double(stmt, 16);                                                                               // HP
      player.xp = sqlite3_column_double(stmt, 17);                                                                               // XP
      player.spellsPerDay = sqlite3_column_int(stmt, 18);                                                                        // SPELLS_PER_DAY
      player.numOfSpells = sqlite3_column_int(stmt, 19);                                                                         // NUM_OF_SPELLS
      player.attacks_per_round = sqlite3_column_int(stmt, 20);                                                                   // ATTACKS_PER_ROUND
      player.gold = sqlite3_column_int(stmt, 21);                                                                                // GOLD
      player.inventory = charToVec(sqlite3_column_text(stmt, 22));                                                               // INVENTORY
      player.encumberance = sqlite3_column_double(stmt, 23);                                                                     // ENCUMBERANCE
      player.spells = string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 24)));                                      // SPELLS
      player.allowed_weapons = charToVec(sqlite3_column_text(stmt, 25));                                                         // ALLOWED_WEAPONS
      player.allowed_armor = charToVec(sqlite3_column_text(stmt, 26));                                                           // ALLOWED_ARMOR
      player.base_mods = charToVec(sqlite3_column_text(stmt, 27));                                                               // BASE_MODS
      player.racial_abilities = charToVec(sqlite3_column_text(stmt, 28));                                                        // RACIAL_ABILITIES
      player.class_abilities = charToVec(sqlite3_column_text(stmt, 29));                                                         // CLASS_ABILITIES
      player.profficiencies = charToVec(sqlite3_column_text(stmt, 30));                                                          // PROFFICIENCIES
      player.magicSchools = charToVec(sqlite3_column_text(stmt, 31));                                                            // MAGIC_SCHOOLS
      player.sphere = string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 32)));                                      // SPHERE
      player.mesh = string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 33)));

      std::cout << GREEN_TEXT << std::endl;
      std::cout << "Read success." << std::endl;                                                                                 // Print to terminal
      std::cout << RESET_TEXT << std::endl;
   }
   else if( rc != SQLITE_DONE ){                                                                                                 // Otherwise, if the step has not either reached a row OR finished stepping
      fprintf(stderr, "SQL error: %s\n", zErrMsg);                                                                               // Throw an error
      sqlite3_free(zErrMsg);                                                                                                     // Free the error from memory
   } 
   
   sqlite3_finalize(stmt);                                                                                                       // Destroy the SQL statement and free allocated memory
   sqlite3_close(db);                                                                                                            // Close the database                                                                                                               // Return the player object
}