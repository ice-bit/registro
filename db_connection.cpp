#include "db_connection.h"

int connection::init_db() {
	std::string adPath = getenv("APPDATA");
	std::string rootDir = adPath + "/Registro/registro.db";


	rc = sqlite3_open(rootDir.c_str(), &db);
	if (rc != SQLITE_OK) {
		std::cerr << "Can't open the database" << std::endl;  // Error Messages handled by other class
		sqlite3_close(db);
		return 1;
	}
	const char * sqlInit = "DROP TABLE IF EXISTS REGISTRO; CREATE TABLE REGISTRO( " \
		"ID INTEGER PRIMARY KEY AUTOINCREMENT, " \
		"VOTE INT NOT NULL, " \
		"SUBJECT TEXT NOT NULL, " \
		"DATE TEXT NOT NULL, " \
		"DESCRIPTION TEXT); ";
	rc = sqlite3_exec(db, sqlInit, 0, 0, &ErrMsg);
	if (rc != SQLITE_OK) {
		std::cerr << "Can't open the database" << std::endl; //Error Messages handled by other class
		sqlite3_free(ErrMsg);
		sqlite3_close(db);
		return 1;
	}
	sqlite3_close(db);
	return 0;
}