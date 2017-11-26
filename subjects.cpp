#include "subjects.h"

int subjects::addSubjects(void *qTextAppend, int argc, char **argv, char **azColName) {
	QTextEdit* qText = (QTextEdit*)qTextAppend;
	for (int i = 0; i < argc; i++) {
		qText->setFontWeight(QFont::Bold);
		qText->insertPlainText(azColName[i]);
		qText->setFontWeight(QFont::Normal);
		qText->insertPlainText(": ");
		qText->insertPlainText(argv[i]);
		qText->insertPlainText("\n");
	}
	qText->insertPlainText("\n");
	return 0;
}
subjects::subjects(QWidget *parent) : QMainWindow(parent), ui(new Ui::subjects) {
	ui->setupUi(this);
	setFixedSize(827, 440);
	//Load subjects
	ui->txtSub->clear();
	std::string sqlQuery = "SELECT * FROM SUBJECTS;";
	std::string adPath = define_path();
	std::string rootDir = adPath + "/registro.db";

	rc = sqlite3_open(rootDir.c_str(), &db);
	if (rc != SQLITE_OK) {
		QMessageBox errMsg;
		errMsg.critical(0, "Error", "Failed to open the database!");
	}
	rc = sqlite3_exec(db, sqlQuery.c_str(), addSubjects, ui->txtSub, &ErrMsg);
	/*if(rc != SQLITE_OK) {
	QMessageBox errMsg;
	errMsg.critical(0, "Error", "Failed to execute your query!");
	Do nothing beacuse SUBJECTS table could be empty
	}*/
	sqlite3_free(ErrMsg);
	sqlite3_close(db);
}
void subjects::on_btnUpdate_clicked() {
	this->id = ui->spnIdUp->value();
	this->subjectName = ui->lnNameUp->text().toLocal8Bit().constData();

	sqlQuery = "UPDATE SUBJECTS SET NAME = ? WHERE ID = ?;";
	std::string adPath = define_path();
	std::string rootDir = adPath + "/registro.db";

	rc = sqlite3_open(rootDir.c_str(), &db);
	if (rc != SQLITE_OK) {
		QMessageBox errMsg;
		errMsg.critical(0, "Error", "Failed to open database file!");
	}
	rc = sqlite3_prepare_v2(db, sqlQuery.c_str(), -1, &res, 0);
	if (rc == SQLITE_OK) {
		sqlite3_bind_text(res, 1, this->subjectName.c_str(), this->subjectName.length(), SQLITE_TRANSIENT);
		sqlite3_bind_int(res, 2, this->id);
		step = sqlite3_step(res);
	}
	else {
		QMessageBox errMsg;
		errMsg.critical(0, "Error", "Failed to update subject!");
	}
	sqlite3_finalize(res);
	sqlite3_close(db);
}
void subjects::on_btnDel_clicked() {
	this->id = ui->spnIdDel->value();
	sqlQuery = "DELETE FROM SUBJECTS WHERE ID = ?";
	std::string adPath = define_path();
	std::string rootDir = adPath + "/registro.db";

	rc = sqlite3_open(rootDir.c_str(), &db);
	if (rc != SQLITE_OK) {
		QMessageBox errMsg;
		errMsg.critical(0, "Error", "Failed to open database file!");
	}
	rc = sqlite3_prepare_v2(db, sqlQuery.c_str(), -1, &res, 0);
	if (rc == SQLITE_OK) {
		sqlite3_bind_int(res, 1, this->id);
		step = sqlite3_step(res);
	}
	else {
		QMessageBox errMsg;
		errMsg.critical(0, "Error", "Failed to delete subject!");
	}
	sqlite3_finalize(res);
	sqlite3_close(db);
}
void subjects::on_btnRefresh_clicked() {
	ui->txtSub->clear();
	std::string sqlQuery = "SELECT * FROM SUBJECTS;";
	std::string adPath = define_path();
	std::string rootDir = adPath + "/registro.db";

	rc = sqlite3_open(rootDir.c_str(), &db);
	if (rc != SQLITE_OK) {
		QMessageBox errMsg;
		errMsg.critical(0, "Error", "Failed to open the database!");
	}
	rc = sqlite3_exec(db, sqlQuery.c_str(), addSubjects, ui->txtSub, &ErrMsg);
	if (rc != SQLITE_OK) {
		QMessageBox errMsg;
		errMsg.critical(0, "Error", "Failed to execute your query!");
	}
	sqlite3_free(ErrMsg);
	sqlite3_close(db);
}
subjects::~subjects() {
	delete ui;
}
