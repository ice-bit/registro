#include "editrecord.h"

editrecord::editrecord(QWidget *parent) : QMainWindow(parent), ui(new Ui::editrecord) {
	ui->setupUi(this);
	setFixedSize(469, 487);
	//Set Current Date
	ui->dtNewDate->setDate(QDate::currentDate());
	//Add subject to comboBox
	ui->cbnNewSubject->addItem("Diritto");
	ui->cbnNewSubject->addItem("Economia Az.");
	ui->cbnNewSubject->addItem("Economia Pol.");
	ui->cbnNewSubject->addItem("Informatica");
	ui->cbnNewSubject->addItem("Inglese");
	ui->cbnNewSubject->addItem("Italiano");
	ui->cbnNewSubject->addItem("Matematica");
	ui->cbnNewSubject->addItem("Storia");

}
void editrecord::get_all_elements() {
	this->id = ui->spnNewId->value();
	this->vote = ui->spnNewVote->value();
	this->subject = ui->cbnNewSubject->currentText().toLocal8Bit().constData(); //Cast QString to String
	this->date = ui->dtNewDate->date().toString("dd/MM/yyyy").toLocal8Bit().constData(); //Cast QString to String and set European Date Format
	this->description = ui->lnNewDesc->text().toLocal8Bit().constData(); //Cast QString to String
}
void editrecord::on_btnUpdateRecord_clicked() {
	get_all_elements();
	this->sqlQuery = "UPDATE REGISTRO SET VOTE = ?, SUBJECT = ?, DATE = ?, DESCRIPTION = ? WHERE ID = ?;";
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
		sqlite3_bind_int(res, 5, this->id);
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
editrecord::~editrecord() {
	delete ui;
}