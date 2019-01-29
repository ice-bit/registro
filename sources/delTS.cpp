#include "delTS.h"

delTS::delTS(QString dbPath, QWidget *parent) : QMainWindow(parent), ui(new Ui::delTSClass) {
    ui->setupUi(this);
    setFixedSize(472, 512);

    // Free QObjects(needed to avoid memory leaks)
    setAttribute(Qt::WA_DeleteOnClose);
    ui->tbSubjects->horizontalHeader()->setStretchLastSection(true);
    ui->tbTeachers->horizontalHeader()->setStretchLastSection(true);

    // Initialize the database path if it already exists
    if(dbPath != nullptr)
        this->dbPath = dbPath;
}

void delTS::on_btnDeleteSub_clicked() {
    // Get the user input
    this->subid = ui->spnSub->value();

    // Get user path
    if(this->dbPath == nullptr) {
        path pt;
        this->dbPath = pt.get_path();
    }

    // Load the SQLite driver
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(this->dbPath);
    if(!db.open()) {
        QMessageBox::critical(nullptr, QObject::tr("Cannot open the database!"),
           QObject::tr(db.lastError().text().toLocal8Bit().data()), QMessageBox::Ok); 
        return;
    }

    // Our query
    QSqlQuery query;

    // Delete the subject
    query.prepare("DELETE FROM subject WHERE ID = :id;");
    query.bindValue(":id", this->subid);

    if(!query.exec()) {
        ui->lblQueryStatus->setText(query.lastError().text());
        return;
    }
    else {
        // else print a status message for 1.5 seconds.
        ui->lblQueryStatus->setText("Subject deleted successfully!");
        QTimer::singleShot(1500, ui->lblQueryStatus, [&](){ ui->lblQueryStatus->setText(" "); });
    }

    // Close the connection to the database
    QString con;
    con = db.connectionName();
    db.close();
    db = QSqlDatabase();
    db.removeDatabase(con);
    
    // Then refresh the Subject combobox
    on_actionRefresh_triggered();
}

void delTS::on_btnDeleteTeach_clicked() {
    // Get the user input
    this->teachid = ui->spnTeach->value();

    // Get user path
    if(this->dbPath == nullptr) {
        path pt;
        this->dbPath = pt.get_path();
    }

    // Load the SQLite driver
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(this->dbPath);
    if(!db.open()) {
        QMessageBox::critical(nullptr, QObject::tr("Cannot open the database!"),
           QObject::tr(db.lastError().text().toLocal8Bit().data()), QMessageBox::Ok); 
        return;
    }

    // Our query
    QSqlQuery query;

    // Delete the subject
    query.prepare("DELETE FROM teacher WHERE ID = :id;");
    query.bindValue(":id", this->teachid);

    if(!query.exec()) {
        ui->lblQueryStatus->setText(query.lastError().text());
        return;
    }
    else {
        // else print a status message for 1.5 seconds.
        ui->lblQueryStatus->setText("Teacher deleted successfully!");
        QTimer::singleShot(1500, ui->lblQueryStatus, [&](){ ui->lblQueryStatus->setText(" "); });
    }

    // Close the connection to the database
    QString con;
    con = db.connectionName();
    db.close();
    db = QSqlDatabase();
    db.removeDatabase(con);

    // Then refresh the tables
    on_actionRefresh_triggered();
}
void delTS::on_actionRefresh_triggered() {

    // Get user path
    if(this->dbPath == nullptr) {
        path pt;
        this->dbPath = pt.get_path();
    }

    // Load the SQLite driver
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(this->dbPath);
    if(!db.open()) {
        QMessageBox::critical(nullptr, QObject::tr("Cannot open the database!"),
           QObject::tr(db.lastError().text().toLocal8Bit().data()), QMessageBox::Ok); 
        return;
    }

    // Create a query model to store query result set.
    QSqlQueryModel *mainmodel = new QSqlQueryModel();
    QSqlQueryModel *submodel = new QSqlQueryModel();
    // Our Query
    QSqlQuery *mainquery = new QSqlQuery(db);
    QSqlQuery *subquery = new QSqlQuery(db);
    // Execute the query
    mainquery->exec("SELECT ID, SubName FROM subject;");

    // Error Handling
    if(!mainquery->isActive()) {
        ui->lblQueryStatus->setText("Can't load subjects list : " + mainquery->lastError().text());
        return;
    }

    // Put the result of the query into our model
    mainmodel->setQuery(*mainquery);

    // Display our model into the QTableView.
    ui->tbSubjects->setModel(mainmodel);

    // Configure column's labels with appropriate names
    mainmodel->setHeaderData(0, Qt::Horizontal, tr("ID"));
    mainmodel->setHeaderData(1, Qt::Horizontal, tr("Subject"));

    // Now do the same thing with the other table(Teachers)

    // Load teachers into Qtable
    subquery->exec("SELECT ID, TSurname FROM teacher;");

    // Error Handling
    if(!subquery->isActive()) {
        ui->lblQueryStatus->setText("Can't load teachers list : " + subquery->lastError().text());
        return;
    }

    // Put the result of the query into our model
    submodel->setQuery(*subquery);

    // Display our model into the QTableView.
    ui->tbTeachers->setModel(submodel);

    // Configure column's labels with appropriate names
    submodel->setHeaderData(0, Qt::Horizontal, tr("ID"));
    submodel->setHeaderData(1, Qt::Horizontal, tr("Teacher"));
    
    // Close the connection to the database
    QString con;
    con = db.connectionName();
    db.close();
    db = QSqlDatabase();
    db.removeDatabase(con);
    
    // Adjust the column width
    ui->tbSubjects->setColumnWidth(1, 150);
    ui->tbTeachers->setColumnWidth(1, 150);

    // Remove row index
    ui->tbSubjects->verticalHeader()->setVisible(false);
    ui->tbTeachers->verticalHeader()->setVisible(false);

    // Delete heap objects
    delete mainquery;
    delete subquery;
}

delTS::~delTS() { delete ui; }