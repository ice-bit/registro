#include "registro_main_gui.h" 
/*Initialize row position
These two variables are required because the callback function of SQLite *MUST* be static,
so any variable that hasn't been declared as static are not allowed to be in that
scope
*/
int registro_main_gui::row = 0;
int registro_main_gui::dynamic_rows = 0;
registro_main_gui::registro_main_gui(QWidget *parent) : QMainWindow(parent), ui(new Ui::registro_main_guiClass)
{
	ui->setupUi(this);
	//Combobox Elements
	ui->cbnSearchBy->addItem("Mark");
	ui->cbnSearchBy->addItem("Subject");
	ui->cbnSearchBy->addItem("Date");
	ui->cbnSearchBy->addItem("Description");
	//Table structure
	QStringList header;
	ui->tbResult->setColumnCount(5);
	header << "ID" <<"Mark" << "Subject" << "Day" << "Description";
	ui->tbResult->setHorizontalHeaderLabels(header);
	ui->tbResult->horizontalHeader()->setStretchLastSection(true); //Set Columns width automatically
	ui->tbResult->setEditTriggers(QAbstractItemView::NoEditTriggers); //Set The Table ReadOnly
	ui->tbResult->verticalHeader()->setVisible(false);
}
int registro_main_gui::addLoadResult(void *qTableAppend, int argc, char **argv, char **azColName) {
	int col = 0;
	QTableWidget* qTable = (QTableWidget*)qTableAppend;
	qTable->insertRow(dynamic_rows);
	for (int i = 0; i < argc; i++) {
		qTable->setItem(row, col, new QTableWidgetItem(argv[i]));
		col++;
	}
	row++;
	dynamic_rows++;
	return 0;
}
void registro_main_gui::on_ActionAddDB_triggered() {
	addDB = new adddb();
	addDB->show();
}
void registro_main_gui::on_ActionAbout_triggered() {
	aboutWin = new about();
	aboutWin->show();
}
void registro_main_gui::on_btnNewElement_clicked() {
	addRC = new addrecord();
	addRC->show();
}
void registro_main_gui::on_btnEditRecord_clicked() {
	editRC = new editrecord();
	editRC->show();
}
void registro_main_gui::on_btnLoadElements_clicked() {
	dynamic_rows = 0;
	row = 0;
	std::string adPath = define_path();
	std::string rootDir = adPath + "/registro.db";
	rc = sqlite3_open(rootDir.c_str(), &db);
	if (rc != SQLITE_OK) {
		QMessageBox errMsg;
		errMsg.critical(0, "Error", "Failed to open the database");
	}
	ui->tbResult->setRowCount(0);
	rc = sqlite3_exec(db, sqlQuery.c_str(), addLoadResult, ui->tbResult, &ErrMsg);
	if (rc != SQLITE_OK) {
		QMessageBox errMsg;
		errMsg.critical(0, "Error", "Failed to execute your query!");
	}
	ui->lblElements->setText("Numbers Of Elements: " + QString::number(ui->tbResult->rowCount()));
	sqlite3_free(ErrMsg);
	sqlite3_close(db);
}
void registro_main_gui::on_btnSearchElements_clicked() {
	dynamic_rows = 0;
	row = 0;
	std::string adPath = define_path();
	std::string rootDir = adPath + "/registro.db";
	this->searchType = ui->cbnSearchBy->currentText().toLocal8Bit().constData();
	if (this->searchType == "Mark") {
		this->sqlQuerySearch = "SELECT * FROM REGISTRO WHERE MARK LIKE '%";
		this->search = ui->lnSearch->text().toLocal8Bit().constData();
		sqlQuerySearch += search + "%';";
	}
	else if (this->searchType == "Subject") {
		this->sqlQuerySearch = "SELECT * FROM REGISTRO WHERE SUBJECT LIKE '%";
		this->search = ui->lnSearch->text().toLocal8Bit().constData();
		sqlQuerySearch += search + "%';";
	}
	else if (this->searchType == "Date") {
		this->sqlQuerySearch = "SELECT * FROM REGISTRO WHERE DAY LIKE '%";
		this->search = ui->lnSearch->text().toLocal8Bit().constData();
		sqlQuerySearch += search + "%';";
	}
	else if (this->searchType == "Description") {
		this->sqlQuerySearch = "SELECT * FROM REGISTRO WHERE DESCRIPTION LIKE '%";
		this->search = ui->lnSearch->text().toLocal8Bit().constData();
		sqlQuerySearch += search + "%';";
	}
	else {
		QMessageBox errMsg;
		errMsg.critical(0, "Error", "Unhandled Exception!");
	}
	rc = sqlite3_open(rootDir.c_str(), &db);
	if (rc != SQLITE_OK) {
		QMessageBox errMsg;
		errMsg.critical(0, "Error", "Failed to open the database");
	}
	ui->tbResult->setRowCount(0);
	rc = sqlite3_exec(db, sqlQuerySearch.c_str(), addLoadResult, ui->tbResult, &ErrMsg);
	if (rc != SQLITE_OK) {
		QMessageBox errMsg;
		errMsg.critical(0, "Error", "Failed to execute your query!");
	}
	ui->lblElements->setText("Numbers Of Elements: " + QString::number(ui->tbResult->rowCount()));
	sqlite3_free(ErrMsg);
	sqlite3_close(db);
}
void registro_main_gui::on_btnLoadBadElements_clicked() {
	dynamic_rows = 0;
	row = 0;
	std::string adPath = define_path();
	std::string rootDir = adPath + "/registro.db";
	std::string sqlQueryBadVotes = "SELECT ID, MARK, SUBJECT, DAY, DESCRIPTION FROM REGISTRO WHERE MARK < 6";
	rc = sqlite3_open(rootDir.c_str(), &db);
	if (rc != SQLITE_OK) {
		QMessageBox errMsg;
		errMsg.critical(0, "Error", "Failed to open the database");
	}
	ui->tbResult->setRowCount(0);
	rc = sqlite3_exec(db, sqlQueryBadVotes.c_str(), addLoadResult, ui->tbResult, &ErrMsg);
	if (rc != SQLITE_OK) {
		QMessageBox errMsg;
		errMsg.critical(0, "Error", "Failed to execute your query!");
	}
	ui->lblElements->setText("Numbers Of Elements: " + QString::number(ui->tbResult->rowCount()));
	sqlite3_free(ErrMsg);
	sqlite3_close(db);
}
void registro_main_gui::on_btnDeleteRecord_clicked() {
	this->delid = ui->spnDelID->text().toLocal8Bit().constData(); //Cast QString to String
	std::string delQuery = "DELETE FROM REGISTRO WHERE ID = ?;";
	std::string adPath = define_path();
	std::string rootDir = adPath + "/registro.db";

	rc = sqlite3_open(rootDir.c_str(), &db);
	if (rc != SQLITE_OK) {
		QMessageBox errMsg;
		errMsg.critical(0, "Error", "Failed to open the database");
	}
	rc = sqlite3_prepare_v2(db, delQuery.c_str(), -1, &res, 0);
	if (rc == SQLITE_OK) {
		sqlite3_bind_text(res, 1, this->delid.c_str(), this->delid.length(), SQLITE_TRANSIENT);
		step = sqlite3_step(res);
	}
	else {
		QMessageBox errMsg;
		errMsg.critical(0, "Error", "Failed to executing your query!");
	}
	sqlite3_finalize(res);
	sqlite3_close(db);
}
void registro_main_gui::on_ActionExit_triggered() {
	delete ui;
	exit(0);
}
registro_main_gui::~registro_main_gui() {
	delete ui;
}
