#include "createDB.h"


createDB::createDB(QWidget *parent) : QMainWindow(parent), ui(new Ui::createDBClass) {
    ui->setupUi(this);
    setFixedSize(639, 189);

    // Free QObjects(needed to avoid memory leaks)
    setAttribute(Qt::WA_DeleteOnClose);
}

void createDB::on_btnAddSubject_clicked() {
    // Fetch the user input
    this->teacher = ui->lnTeacher->text();
    this->subject = ui->lnSubject->text();

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
    query.prepare("INSERT INTO teacher (TSurname) VALUES (:surname);");
    query.bindValue(":surname", this->teacher);

    // Error Handling
    if(!query.exec())
        ui->lblQueryStatus->setText("Error while executing this query!");

    // After that we need to insert the subject.
    query.prepare("SELECT ID FROM teacher WHERE TSurname = :surname LIMIT 1;");
    query.bindValue(":surname", this->teacher);

    // Error Handling
    if(!query.exec())
        ui->lblQueryStatus->setText("Error while executing this query!");

    // We need to retrive the ID of the teacher
    int id;
    if(query.first())
        id = query.value(0).toInt(); // store the result of the query into id variable
    else
        ui->lblQueryStatus->setText("Error while executing this query!");

    query.prepare("INSERT INTO subject (SubName,CodTeacher) VALUES (:subject, :id);");
    query.bindValue(":subject", this->subject);
    query.bindValue(":id", id);

    // Then insert the subject
    if(!query.exec())
        ui->lblQueryStatus->setText("Error while executing this query!");
    else { 
        // else print a status message for 1.5 seconds.
        ui->lblQueryStatus->setText("Subject added successfully!");
        QTimer::singleShot(1500, ui->lblQueryStatus, [&](){ ui->lblQueryStatus->setText(" "); });
    }
    
}

createDB::~createDB() { delete ui; }