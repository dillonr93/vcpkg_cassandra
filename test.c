#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <stdio.h>
#include "cassandra.h"

int runCassandra(){

    CassCluster* cluster = cass_cluster_new();

    CassSession* session = cass_session_new();

    cass_cluster_set_contact_points(cluster, "127.0.0.1");   

    CassFuture* future = cass_session_connect(session,cluster);

    CassError errorCode = cass_future_error_code(future);

    cass_future_free(future);

    if(errorCode != CASS_OK){
        printf("Connection Failed \n");
    }else{        

        for(int i = 0; i < 100; i++){
            CassStatement* statement = cass_statement_new("SELECT * FROM point_of_sale.transaction_log LIMIT 1", 0);
            CassFuture* result_future = cass_session_execute(session, statement);

            CassError statementError = cass_future_error_code(result_future);

            cass_future_free(result_future);
            cass_statement_free(statement);
        }

        printf("Connection Success \n");       

    }


    cass_session_close(session);
    cass_cluster_free(cluster);    
}

static int callback(void *NotUsed, int argc, char **argv, char **azColName) {
   int i;
   for(i = 0; i<argc; i++) {
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
   }
   printf("\n");
   return 0;
}

int runSqlite() {
   sqlite3 *db;
   char *zErrMsg = 0;
   int rc;
   char *sql;

   /* Open database */
   rc = sqlite3_open("test.db", &db);
   
   if( rc ) {
      fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
      return(0);
   } else {
      fprintf(stdout, "Opened database successfully\n");
   }


//    sqlite3_exec(db, "DROP TABLE IF EXISTS COMPANY;", 0 ,callback, &zErrMsg);
   sqlite3_exec(db, "PRAGMA journal_mode=WAL;", 0 ,callback, &zErrMsg);

   /* Create SQL statement */
   sql = "CREATE TABLE IF NOT EXISTS COMPANY("  \
      "ID INTEGER PRIMARY KEY AUTOINCREMENT  NOT NULL," \
      "NAME           TEXT    NOT NULL," \
      "AGE            INT     NOT NULL," \
      "ADDRESS        CHAR(50)," \
      "SALARY         REAL );";

   /* Execute SQL statement */
   rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
   
   if( rc != SQLITE_OK ){
      fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
   } else {
      fprintf(stdout, "Table created successfully\n");
   }

   time_t from;

   time(&from);

   rc = sqlite3_exec(db, "BEGIN TRANSACTION;", callback, 0, &zErrMsg);

   for(int i = 0; i < 2000; i++){
          /* Create SQL statement */
    sql = "INSERT INTO COMPANY (NAME,AGE,ADDRESS,SALARY) "  \
         "VALUES ('Paul', 32, 'California', 20000.00 ); " \
         "INSERT INTO COMPANY (NAME,AGE,ADDRESS,SALARY) "  \
         "VALUES ('Allen', 25, 'Texas', 15000.00 ); "     \
         "INSERT INTO COMPANY (NAME,AGE,ADDRESS,SALARY)" \
         "VALUES ('Teddy', 23, 'Norway', 20000.00 );" \
         "INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY)" \
         "VALUES ('Mark', 25, 'Rich-Mond ', 65000.00 );";

    /* Execute SQL statement */
    rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
   }

   rc = sqlite3_exec(db, "END TRANSACTION;", callback, 0, &zErrMsg);

   time_t to;
   time(&to);

   printf("Took %ld(seconds)\n", to-from);


   sqlite3_close(db);
   return 0;
}


//----------------------------------------------------------------------------
//Main

int main(int argc, char* argv[]){
    runSqlite();
}
