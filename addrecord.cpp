#include "addrecord.h"

addrecord::addrecord(QWidget *parent) : QMainWindow(parent), ui(new Ui::addrecord) {
	ui->setupUi(this);
	setFixedSize(469, 487);
	//Set Current Date
	ui->dtVote->setDate(QDate::currentDate());
	//Add Subject to comboBox
	ui->cbnSubject->addItem("Diritto");
	ui->cbnSubject->addItem("Economia Az.");
	ui->cbnSubject->addItem("Economia Pol.");
	ui->cbnSubject->addItem("Informatica");
	ui->cbnSubject->addItem("Inglese");
	ui->cbnSubject->addItem("Italiano");
	ui->cbnSubject->addItem("Matematica");
	ui->cbnSubject->addItem("Storia");
}
void addrecord::get_all_elements(int op) {
	if (op == 0) {
		this->vote = ui->spnVote->value();
		this->subject = ui->cbnSubject->currentText().toLocal8Bit().constData(); //Cast QString to String
		this->date = ui->dtVote->date().toString("dd/MM/yyyy").toLocal8Bit().constData(); //Cast QString to String and set European Date Format
		this->description = ui->lnDesc->text().toLocal8Bit().constData(); //Cast QString to String
	}
	else if (op == 1) {
		this->delid = ui->lnDelById->text().toLocal8Bit().constData(); //Cast QString to String
	}
	else {
		std::cerr << "Error by the developer" << std::endl; //No debug messages required
	}
}
void addrecord::on_btnSaveVote_clicked() {
	get_all_elements(0);
	sqlQuery = "INSERT INTO REGISTRO(VOTE, SUBJECT, DATE, DESCRIPTION) VALUES (?,?,?,?);";
	std::string adPath = getenv("APPDATA");
	std::string rootDir = adPath + "/Registro/registro.db";

	rc = sqlite3_open(rootDir.c_str(), &db);
	if (rc != SQLITE_OK) {
		QMessageBox errMsg;
		errMsg.critical(0, "Error", "Failed to open the database");
	}
	rc = sqlite3_prepare_v2(db, sqlQuery, -1, &res, 0);
	if (rc == SQLITE_OK) {
		sqlite3_bind_int(res, 1, this->vote);
		sqlite3_bind_text(res, 2, this->subject.c_str(), this->subject.length(), SQLITE_TRANSIENT);
		sqlite3_bind_text(res, 3, this->date.c_str(), this->date.length(), SQLITE_TRANSIENT);
		sqlite3_bind_text(res, 4, this->description.c_str(), this->description.length(), SQLITE_TRANSIENT);
		ui->lblDebug->setText("Query executed successfully!");
		step = sqlite3_step(res);
	}
	else {
		QMessageBox errMsg;
		errMsg.critical(0, "Error", "Failed to executing your query!");
	}
	sqlite3_finalize(res);
	sqlite3_close(db);
}
void addrecord::on_btnDelRecord_clicked() {
	get_all_elements(1);
	sqlQuery = "DELETE FROM REGISTRO WHERE ID = ?;";
	std::string adPath = getenv("APPDATA");
	std::string rootDir = adPath + "/Registro/registro.db";


	rc = sqlite3_open(rootDir.c_str(), &db);
	if (rc != SQLITE_OK) {
		QMessageBox errMsg;
		errMsg.critical(0, "Error", "Failed to open the database");
	}
	rc = sqlite3_prepare_v2(db, sqlQuery, -1, &res, 0);
	if (rc == SQLITE_OK) {
		sqlite3_bind_text(res, 1, this->delid.c_str(), this->delid.length(), SQLITE_TRANSIENT);
		ui->lblDebug->setText("Query executed successfully!");
		step = sqlite3_step(res);
	}
	else {
		QMessageBox errMsg;
		errMsg.critical(0, "Error", "Failed to executing your query!");
	}
	sqlite3_finalize(res);
	sqlite3_close(db);
}
addrecord::~addrecord() {
	delete ui;
}