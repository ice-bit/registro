#include "createDB.h"


createDB::createDB(QWidget *parent) : QMainWindow(parent), ui(new Ui::createDBClass) {
    ui->setupUi(this);
    setFixedSize(552, 229);

    // Free QObjects(needed to avoid memory leaks)
    setAttribute(Qt::WA_DeleteOnClose);
}

void createDB::on_actionRefresh_triggered() {
    // Load the SQLite driver
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("debug.db");
    if(!db.open()) {
        QMessageBox::critical(nullptr, QObject::tr("Cannot open the database!"),
            QObject::tr("Unable to create a database connection!"), QMessageBox::Cancel);
        return;
    }

    // Create a query model to store query result set.
    QSqlQueryModel *model = new QSqlQueryModel();
    // Our Query
    QSqlQuery *query = new QSqlQuery(db);

    // Append teachers to the combobox
    query->exec("SELECT TSurname FROM teacher;");

    // Error Handling
    if(!query->isActive())
        ui->lblQueryStatus->setText("Can't load teacher list, try to add a new record first!");
    
    model->setQuery(*query);
    ui->cbnTeacher->setModel(model);
}

void createDB::on_btnAddTeacher_clicked() {
    // Fetch the user input
    this->teacherName = ui->lnTeacherName->text();
    this->teacherSurname = ui->lnTeacherSurname->text();

    // Load the SQLite driver
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("debug.db");
    if(!db.open()) {
        QMessageBox::critical(nullptr, QObject::tr("Cannot open the database!"),
            QObject::tr("Unable to create a database connection!"), QMessageBox::Cancel);
        return;
    }

    // Our Query
    QSqlQuery query;
    // Create the three tables
    query.exec("CREATE TABLE IF NOT EXISTS teacher ("
                "ID INTEGER PRIMARY KEY,"
                "TName TEXT NOT NULL,"
                "TSurname TEXT NOT NULL);");

    query.exec("CREATE TABLE IF NOT EXISTS subject ("
               "ID INTEGER PRIMARY KEY,"
               "SubName TEXT NOT NULL,"
               "CodTeacher INTEGER NOT NULL,"
               "FOREIGN KEY (CodTeacher) REFERENCES teacher(ID));");

    query.exec("CREATE TABLE IF NOT EXISTS mark ("
               "ID INTEGER PRIMARY KEY,"
               "Mark FLOAT NOT NULL,"
               "MarkDate DATE NOT NULL,"
               "Description TEXT NOT NULL,"
               "CodSub INTEGER NOT NULL,"
               "FOREIGN KEY (CodSub) REFERENCES subject(ID));");

    // Error Handling
    if(!query.isActive())
        ui->lblQueryStatus->setText("Error while executing this query!");

    // Now we want to insert the user input into the database
    // For security reason, we want to bind those values.
    query.prepare("INSERT INTO teacher (TName, TSurname) VALUES (:name, :surname);");
    query.bindValue(":name", this->teacherName);
    query.bindValue(":surname", this->teacherSurname);

    // Error Handling
    if(!query.exec())
        ui->lblQueryStatus->setText("Error while executing this query!");
    
    // Print a status message for 1.5 seconds(1500 ms)
    ui->lblQueryStatus->setText("Teacher added successfully!");
        QTimer::singleShot(1500, ui->lblQueryStatus, [&](){ ui->lblQueryStatus->setText(" "); });

    // Close the connection to the database
    db.close();
}

void createDB::on_btnAddSubject_clicked() {
    // Fetch the user input
    this->subject = ui->lnSubject->text();
    this->teacherSurname = ui->cbnTeacher->currentText();

    // Load the SQLite driver
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("debug.db");
    if(!db.open()) {
        QMessageBox::critical(nullptr, QObject::tr("Cannot open the database!"),
            QObject::tr("Unable to create a database connection!"), QMessageBox::Cancel);
        return;
    }

    // Our Query
    QSqlQuery query;
    // Create the three tables
    query.exec("CREATE TABLE IF NOT EXISTS teacher ("
                "ID INTEGER PRIMARY KEY,"
                "TName TEXT NOT NULL,"
                "TSurname TEXT NOT NULL);");

    query.exec("CREATE TABLE IF NOT EXISTS subject ("
               "ID INTEGER PRIMARY KEY,"
               "SubName TEXT NOT NULL,"
               "CodTeacher INTEGER NOT NULL,"
               "FOREIGN KEY (CodTeacher) REFERENCES teacher(ID));");

    query.exec("CREATE TABLE IF NOT EXISTS mark ("
               "ID INTEGER PRIMARY KEY,"
               "Mark FLOAT NOT NULL,"
               "MarkDate DATE NOT NULL,"
               "Description TEXT NOT NULL,"
               "CodSub INTEGER NOT NULL,"
               "FOREIGN KEY (CodSub) REFERENCES subject(ID));");

    // Error Handling
    if(!query.isActive())
        ui->lblQueryStatus->setText("Error while executing this query!");

    // Retrive the ID of the teacher choosed by the user
    query.prepare("SELECT ID FROM teacher WHERE TSurname = :surname LIMIT 1;");
    query.bindValue(":surname", this->teacherSurname);
    
    if(!query.exec())
        ui->lblQueryStatus->setText("Error while executing this query!");

    // Store the result of the query(the Teacher's ID) into a local variable
    int id;
    if(query.first())
        id = query.value(0).toInt();
    else
        ui->lblQueryStatus->setText("Error while executing this query!");

    query.prepare("INSERT INTO subject (SubName,CodTeacher) VALUES("
                  ":subname, :codTeacher);");
    query.bindValue(":subname", this->subject);
    query.bindValue(":codTeacher", id);

    // Then insert the subject
    if(!query.exec())
        ui->lblQueryStatus->setText("Error while executing this query!");
    else { 
        // else print a status message for 1.5 seconds.
        ui->lblQueryStatus->setText("Subject added successfully!");
        QTimer::singleShot(1500, ui->lblQueryStatus, [&](){ ui->lblQueryStatus->setText(" "); });
    }

    // Close the connection to the database
    db.close();
}

createDB::~createDB() { delete ui; }