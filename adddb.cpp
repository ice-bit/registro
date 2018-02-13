#include "adddb.h"

adddb::adddb(QWidget *parent) : QMainWindow(parent), ui(new Ui::adddb) {
	ui->setupUi(this);
	setFixedSize(422, 367);
	//Free all QObject and other stuff
	//Need to decrease memory when closing the window
	setAttribute(Qt::WA_DeleteOnClose);
}
void adddb::on_btnNewDB_clicked() {
	if (init_db() == 1)
		errMsg.critical(0, "Error", "Failed to initialize/reset your database");
	else if (init_db() == 0) 
		errMsg.information(0, "Registro", "Database initialized/resetted");
}

void adddb::on_btnAddNewSub_clicked() {
	//Get subject name from ui
	this->subject = ui->lnSub->text().toLocal8Bit().constData();
	std::string sqlCreateSubject = "CREATE TABLE IF NOT EXISTS SUBJECTS( " \
		"ID INTEGER PRIMARY KEY AUTOINCREMENT, " \
		"NAME TEXT NOT NULL);";
	std::string sqlAddSubject = "INSERT INTO SUBJECTS (NAME) VALUES (?);";
	std::string adPath = define_path();
	std::string rootDir = adPath + "/registro.db";
	rc = sqlite3_open(rootDir.c_str(), &db);
	if (rc != SQLITE_OK) {
		QMessageBox errMsg;
		errMsg.critical(0, "Error", "Failed to open your database!");
		sqlite3_close(db);
	}
	rc = sqlite3_exec(db, sqlCreateSubject.c_str(), 0, 0, &ErrMsg);
	if (rc != SQLITE_OK) {
		QMessageBox errMsg;
		errMsg.critical(0, "Error", "Failed to create the subjects database");
		sqlite3_free(ErrMsg);
		sqlite3_close(db);
	}
	sqlite3_close(db);

	rc = sqlite3_open(rootDir.c_str(), &db);
	if (rc != SQLITE_OK) {
		QMessageBox errMsg;
		errMsg.critical(0, "Error", "Failed to open your database");
	}
	rc = sqlite3_prepare_v2(db, sqlAddSubject.c_str(), -1, &res, 0);
	if (rc == SQLITE_OK) {
		sqlite3_bind_text(res, 1, this->subject.c_str(), this->subject.length(), SQLITE_TRANSIENT);
		step = sqlite3_step(res);
	}
	else {
		QMessageBox errMsg;
		errMsg.critical(0, "Error", "Failed to insert new subject");
	}
	sqlite3_finalize(res);
	sqlite3_close(db);
}
void adddb::on_actionEditSubjects_triggered() {
	sub = new subjects();
	sub->show();
}
adddb::~adddb() {
	delete ui;
}
