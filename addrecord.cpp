#include "addrecord.h"

addrecord::addrecord(QWidget *parent) : QMainWindow(parent), ui(new Ui::addrecord) {
    //Load Subjects to QSpinBox
    ui->setupUi(this);
    setFixedSize(469, 487);
    //Set Current Date
    ui->dtVote->setDate(QDate::currentDate());
    std::string adPath = define_path();
    std::string rootDir = adPath + "/.registro.db";

    rc = sqlite3_open(rootDir.c_str(), &db);
    if(rc != SQLITE_OK) {
        QMessageBox errMsg;
        errMsg.critical(0, "Error", "Failed to load subjects!");
    }
    rc = sqlite3_exec(db, sqlAppend.c_str(), appendSub, ui->cbnSubject, &ErrMsg);
    if(rc != SQLITE_OK) {
        QMessageBox errMsg;
        errMsg.critical(0, "Error", "Failed to load subjects!");
    }
    sqlite3_free(ErrMsg);
    sqlite3_close(db);
}
void addrecord::get_all_elements(int op) {
    if (op == 0) {
        this->mark = ui->spnMark->value();
        this->subject = ui->cbnSubject->currentText().toLocal8Bit().constData(); //Cast QString to String
        this->markd = ui->dtVote->date().toString("dd/MM/yyyy").toLocal8Bit().constData(); //Cast QString to String and set European Date Format
        this->description = ui->lnDesc->text().toLocal8Bit().constData(); //Cast QString to String
    }
    else if (op == 1) {
        this->delid = ui->spnDelById->text().toLocal8Bit().constData(); //Cast QString to String
    }
    else {
        std::cerr << "Error by the developer" << std::endl; //No debug messages required
    }
}
void addrecord::on_btnSaveVote_clicked() {
    get_all_elements(0);
    sqlQuery = "INSERT INTO REGISTRO(MARK, SUBJECT, DAY, DESCRIPTION) VALUES (?,?,?,?);";
    std::string adPath = define_path();
    std::string rootDir = adPath + "/.registro.db";

    rc = sqlite3_open(rootDir.c_str(), &db);
    if (rc != SQLITE_OK) {
        QMessageBox errMsg;
        errMsg.critical(0, "Error", "Failed to open the database");
    }
    rc = sqlite3_prepare_v2(db, sqlQuery, -1, &res, 0);
    if (rc == SQLITE_OK) {
        sqlite3_bind_double(res, 1, this->mark);
        sqlite3_bind_text(res, 2, this->subject.c_str(), this->subject.length(), SQLITE_TRANSIENT);
        sqlite3_bind_text(res, 3, this->markd.c_str(), this->markd.length(), SQLITE_TRANSIENT);
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
    std::string adPath = define_path();
    std::string rootDir = adPath + "/.registro.db";

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
