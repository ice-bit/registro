#include "registro_main_gui.h" 

registro_main_gui::registro_main_gui(QWidget *parent) : QMainWindow(parent), ui(new Ui::registro_main_guiClass)
{
    ui->setupUi(this);
    ui->cbnSearchBy->addItem("Mark");
    ui->cbnSearchBy->addItem("Subject");
    ui->cbnSearchBy->addItem("Date");
    ui->cbnSearchBy->addItem("Description");
}
void registro_main_gui::setHeader(int type) {
    if (type == 0) {
        ui->txtShow->setFontWeight(QFont::Bold);
        ui->txtShow->setTextColor(QColor("red"));
        ui->txtShow->insertPlainText("BAD MARKS LIST: ");
        ui->txtShow->setTextColor(QColor("black"));
        ui->txtShow->setFontWeight(QFont::Normal);
    }
    else if (type == 1) {
        ui->txtShow->setFontWeight(QFont::Bold);
        ui->txtShow->setTextColor(QColor("green"));
        ui->txtShow->insertPlainText("LIST OF MARKS: ");
        ui->txtShow->setTextColor(QColor("black"));
        ui->txtShow->setFontWeight(QFont::Normal);
    }
    else if (type == 3) {
        ui->txtShow->setFontWeight(QFont::Bold);
        ui->txtShow->setTextColor(QColor("cyan"));
        ui->txtShow->insertPlainText("SORTED MARKS: ");
        ui->txtShow->setTextColor(QColor("black"));
        ui->txtShow->setFontWeight(QFont::Normal);
    }
    else {
        std::cerr << "unhandled exception" << std::endl;
    }
}
int registro_main_gui::addLoadResult(void *qTextAppend, int argc, char **argv, char **azColName) {
    QTextEdit* qText = (QTextEdit*)qTextAppend;
    for (int i = 0; i < argc; i++) {
        qText->insertPlainText("\n");
        qText->setFontWeight(QFont::Bold);
        qText->insertPlainText(azColName[i]);
        qText->setFontWeight(QFont::Normal);
        qText->insertPlainText(": ");
        qText->insertPlainText(argv[i]);
    }
    qText->insertPlainText("\n");
    return 0;
}
int registro_main_gui::addBadResult(void *qTextAppend, int argc, char **argv, char **azColName) {
    QTextEdit* qText = (QTextEdit*)qTextAppend;

    for (int i = 0; i < argc; i++) {
        qText->insertPlainText("\n");
        qText->setFontWeight(QFont::Bold);
        qText->insertPlainText(azColName[i]);
        qText->setFontWeight(QFont::Normal);
        qText->insertPlainText(": ");
        qText->insertPlainText(argv[i]);
    }
    qText->insertPlainText("\n");
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
    ui->txtShow->clear();
    std::string adPath = define_path();
    std::string rootDir = adPath + "/.registro.db";
    setHeader(1); //Set title

    rc = sqlite3_open(rootDir.c_str(), &db);
    if (rc != SQLITE_OK) {
        QMessageBox errMsg;
        errMsg.critical(0, "Error", "Failed to open the database");
    }
    rc = sqlite3_exec(db, sqlQuery, addLoadResult, ui->txtShow, &ErrMsg);
    if (rc != SQLITE_OK) {
        QMessageBox errMsg;
        errMsg.critical(0, "Error", "Failed to execute your query!");
    }
    sqlite3_free(ErrMsg);
    sqlite3_close(db);
}
void registro_main_gui::on_btnSearchElements_clicked() {
    ui->txtShow->clear();
    std::string adPath = define_path();
    std::string rootDir = adPath + "/.registro.db";
    this->searchType = ui->cbnSearchBy->currentText().toLocal8Bit().constData();
    setHeader(3);
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
    rc = sqlite3_exec(db, sqlQuerySearch.c_str(), addLoadResult, ui->txtShow, &ErrMsg);
    if (rc != SQLITE_OK) {
        QMessageBox errMsg;
        errMsg.critical(0, "Error", "Failed to execute your query!");
    }
    sqlite3_free(ErrMsg);
    sqlite3_close(db);
}
void registro_main_gui::on_btnLoadBadElements_clicked() {
    ui->txtShow->clear();
    std::string SqlQuerybadVotes = "SELECT * FROM REGISTRO WHERE MARK <= 6";
    std::string adPath = define_path();
    std::string rootDir = adPath + "/.registro.db";
    setHeader(0); //Set title

    rc = sqlite3_open(rootDir.c_str(), &db);
    if (rc != SQLITE_OK) {
        QMessageBox errMsg;
        errMsg.critical(0, "Error", "Failed to open the database");
    }
    rc = sqlite3_exec(db, SqlQuerybadVotes.c_str(), addBadResult, ui->txtShow, &ErrMsg);
    if (rc != SQLITE_OK) {
        QMessageBox errMsg;
        errMsg.critical(0, "Error", "Failed to execute your query!");
    }
    sqlite3_free(ErrMsg);
    sqlite3_close(db);
}
void registro_main_gui::on_ActionExit_triggered() {
    delete ui;
    exit(0);
}
registro_main_gui::~registro_main_gui() {
    delete ui;
}
