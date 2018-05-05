#include "db_connection.h"

int connection::appendSub(void *qCbnAppend, int argc, char **argv, char **azColName) {
	QComboBox* qCbn = (QComboBox*)qCbnAppend;
	for (int i = 0; i < argc; i++) {
		qCbn->addItem(argv[i]);
	}
	return 0;
}

std::string connection::define_path() {
	std::string path;
	if (OS == 1) {
        path = getenv("HOME");
        //path += "registro.db";
	}
	else if (OS == 2) {
        path = getenv("APPDATA");
		path += "/Registro";
	}
	else {
		std::string path = getenv(""); //Need error handling
	}
	return path;
}

int connection::init_db() {
	std::string adPath = define_path();
    std::string rootDir = adPath + "/registro.db";
	rc = sqlite3_open(rootDir.c_str(), &db);
	if (rc != SQLITE_OK) {
		std::cerr << "Can't open the database" << std::endl;  // Error Messages handled by other class
		sqlite3_close(db);
		return 1;
	}
	const char * sqlInit = "DROP TABLE IF EXISTS REGISTRO; CREATE TABLE REGISTRO( " \
		"ID INTEGER PRIMARY KEY AUTOINCREMENT, " \
		"MARK DOUBLE NOT NULL, " \
		"SUBJECT TEXT NOT NULL, " \
		"DAY DATE NOT NULL, " \
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
