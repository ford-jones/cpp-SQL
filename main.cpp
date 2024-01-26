#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h> 

static int callback(void *data, int argc, char **argv, char **azColName) 
{
   int i;
   fprintf(stderr, "%s: ", (const char*)data);
   
   for(i = 0; i<argc; i++) 
   {
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
   }

   printf("\n");
   return 0;
}

int main(int argc, char* argv[]) 
{
   sqlite3 *db;                                                                                          //  SQL database handle
   char *zErrMsg = 0;                                                                                    //  SQL error message
   int rc;                                                                                               //  Return code: returned values from calls to the SQL API
   char *sql;                                                                                            //  The prepared SQL statement
   const char* data = "Callback function called";

   rc = sqlite3_open("characters.db", &db);                                                              // Open the SQLite database
   
   if(rc)                                                                                                // If opening the database returns / exits the thread at this point in execution
   {
      fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));                                  // Throw an error
      return(0);                                                                                         // Return from the function, exit the thread
   } 
   else 
   {
      fprintf(stderr, "Opened database successfully\n");                                                 // Otherwise print something to the console
   }
                                                                                                         // Prepare the SQL statement
   sql = "CREATE TABLE CHARACTERS("\
      "ID                   INT PRIMARY KEY   NOT NULL,"\
      "PLAYER_NAME          TEXT              NOT NULL,"\
      "PLAYER_AGE           INT               NOT NULL,"\
      "DATE                 DATETIME          NOT NULL,"\
      "ALIGNMENT            TEXT              NOT NULL,"\
      "ENEMIES              TEXT              NOT NULL,"\
      "CON                  INT               NOT NULL,"\
      "DEX                  INT               NOT NULL,"\
      "INTEL                INT               NOT NULL,"\
      "WIS                  INT               NOT NULL,"\
      "CHA                  INT               NOT NULL,"\
      "PLAYER_RACE          TEXT              NOT NULL,"\
      "PLAYER_CLASS         TEXT              NOT NULL,"\
      "LANGS                TEXT              NOT NULL,"\
      "LEVEL                INT               NOT NULL,"\
      "CASTING_LEVEL        INT               NOT NULL,"\
      "HP                   INT               NOT NULL,"\
      "XP                   INT               NOT NULL,"\
      "SPELLS_PER_DAY       INT               NOT NULL,"\
      "NUM_OF_SPELLS        INT               NOT NULL,"\
      "ATTACKS_PER_ROUND    INT               NOT NULL,"\
      "GOLD                 INT               NOT NULL,"\
      "INVENTORY            TEXT              NOT NULL,"\
      "ENCUMBERANCE         INT               NOT NULL,"\
      "SPELLS               TEXT              NOT NULL,"\
      "ALLOWED_WEAPONS      TEXT              NOT NULL,"\
      "ALLOWED_ARMOR        TEXT              NOT NULL,"\
      "BASE_MODS            TEXT              NOT NULL,"\
      "RACIAL_ABILITIES     TEXT              NOT NULL,"\
      "CLASS_ABILITIES      TEXT              NOT NULL,"\
      "PROFFICIENCIES       TEXT              NOT NULL,"\
      "MAGIC_SCHOOLS        TEXT              NOT NULL,"\
      "SPHERE               TEXT              NOT NULL,"\
      "MESH                 TEXT              NOT NULL);";

   rc = sqlite3_exec(db, sql, callback, (void*)data, &zErrMsg);                                          // Execute the SQL statement pass the callback handler
   
   if( rc != SQLITE_OK )                                                                                 // If the return code is anything other than OK
   {
      fprintf(stderr, "SQL error: %s\n", zErrMsg);                                                       // Throw an error
      sqlite3_free(zErrMsg);                                                                             // Free the error 
   } 
   else                                                                    
   {
      fprintf(stdout, "Operation done successfully\n");                                                  // Otherwise print to console
   }

   sqlite3_close(db);                                                                                    // Close the database
   return 0;                                                                                             // Exit the thread
}