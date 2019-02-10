#include "delSBTS.h"

delSBTS::delSBTS(QString dbPath, QWidget *parent) : QMainWindow(parent), ui(new Ui::delSBTSClass) {
    ui->setupUi(this);
    setFixedSize(372, 537);

    // Free QObjects(needed to avoid memory leaks)
    setAttribute(Qt::WA_DeleteOnClose);
    ui->tbSubjects->horizontalHeader()->setStretchLastSection(true);
    ui->tbTeachers->horizontalHeader()->setStretchLastSection(true);

    // Initialize the database path if it already exists
    if(dbPath != nullptr)
        this->dbPath = dbPath;
    // Then load the teachers from the database
    loadTeachersSubjects();
}

void delSBTS::on_btnDeleteSub_clicked() {
    // Retrive user selection
    QModelIndex index = ui->tbSubjects->selectionModel()->currentIndex();

    // Check how many cells are being selected
    QModelIndexList selection = ui->tbSubjects->selectionModel()->selectedIndexes();
    if(selection.count() > 1) {
        ui->lblQueryStatus->setText("Please select one ID at time.");
        return;
    } else if(!ui->tbSubjects->selectionModel()->hasSelection()) {
        ui->lblQueryStatus->setText("Please select at least one ID.");
        return;
    } else if(index.column() != 0) {
        ui->lblQueryStatus->setText("Please select an element from the ID column.");
        return;
    }

    // Retrive selected cell
    QVariant indexValue = index.sibling(index.row(), index.column()).data();
    // Convert it to an integer
    this->userSelection = indexValue.value<int>();

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

    // Enable foreign key support(disabled by default in SQLite)
    query.exec("PRAGMA foreign_keys = ON;");
    if(!query.isActive()){
        ui->lblQueryStatus->setText("Cannot enabled FK support.");
        return;
    }

    // Delete the subject
    query.prepare("DELETE FROM subjects WHERE ID = :id;");
    query.bindValue(":id", this->userSelection);

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
    
    // Then refresh the Subject and the Teachers tables
    loadTeachersSubjects();
}

void delSBTS::on_btnDeleteTeach_clicked() {
    // Retrieve user selection
    QModelIndex index = ui->tbTeachers->selectionModel()->currentIndex();

    // Check how many cells are being selected
    QModelIndexList selection = ui->tbTeachers->selectionModel()->selectedIndexes();
    if(selection.count() > 1) {
        ui->lblQueryStatus->setText("Please select one ID at time.");
        return;
    } else if(!ui->tbTeachers->selectionModel()->hasSelection()) {
        ui->lblQueryStatus->setText("Please select at least one ID.");
        return;
    } else if(index.column() != 0) {
        ui->lblQueryStatus->setText("Please select an element from the ID column.");
        return;
    }

    // Retrieve selected cell
    QVariant indexValue = index.sibling(index.row(), index.column()).data();
    // Convert it to an integer
    this->userSelection = indexValue.value<int>();
    
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

    // Enable foreign key support(disabled by default in SQLite)
    query.exec("PRAGMA foreign_keys = ON;");
    if(!query.isActive()){
        ui->lblQueryStatus->setText("Cannot enabled FK support.");
        return;
    }

    // Delete the subject
    query.prepare("DELETE FROM teachers WHERE ID = :id;");
    query.bindValue(":id", this->userSelection);

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
    loadTeachersSubjects();
}
void delSBTS::loadTeachersSubjects() {

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
    mainquery->exec("SELECT ID, SubName FROM subjects;");

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
    mainmodel->setHeaderData(1, Qt::Horizontal, tr("Subject Name"));

    // Now do the same thing with the other table(Teachers)

    // Load teachers into Qtable
    subquery->exec("SELECT ID, TSurname FROM teachers;");

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
    submodel->setHeaderData(1, Qt::Horizontal, tr("Teacher Surname"));
    
    // Close the connection to the database
    QString con;
    con = db.connectionName();
    db.close();
    db = QSqlDatabase();
    db.removeDatabase(con);
    
    // Adjust the column width
    ui->tbSubjects->setColumnWidth(0, 150);
    ui->tbTeachers->setColumnWidth(0, 150);

    // Remove row index
    ui->tbSubjects->verticalHeader()->setVisible(false);
    ui->tbTeachers->verticalHeader()->setVisible(false);

    // Delete heap objects
    delete mainquery;
    delete subquery;
}

delSBTS::~delSBTS() { delete ui; }