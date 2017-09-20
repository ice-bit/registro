#include "adddb.h"

adddb::adddb(QWidget *parent) : QMainWindow(parent), ui(new Ui::adddb) {
	ui->setupUi(this);
	setFixedSize(533, 303);
}
void adddb::on_btnNewDB_clicked() { 
	if (init_db() == 1) {
		QMessageBox errMsg;
		errMsg.critical(0, "Error", "Failed to initialize/reset your database");
	}
	else if (init_db() == 0) {
		ui->lblStatusDb->setText("Database initialized/resetted");
	}
}
adddb::~adddb() {
	delete ui;
}