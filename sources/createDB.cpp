#include "createDB.h"

createDB::createDB(QString dbPath, QWidget *parent) : QMainWindow(parent), ui(new Ui::createDBClass) {
    ui->setupUi(this);
    setFixedSize(552, 229);

    // Free QObjects(needed to avoid memory leaks)
    setAttribute(Qt::WA_DeleteOnClose);

    // Initialize the database path if it already exists
    if(dbPath != nullptr)
        this->dbPath = dbPath;
    // Then load the teachers from the database
    loadTeachers();
}

void createDB::loadTeachers() {
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
    QSqlQueryModel *model = new QSqlQueryModel();
    // Our Query
    QSqlQuery *query = new QSqlQuery(db);

    // Append teachers to the combobox
    query->exec("SELECT TSurname FROM teacher;");

    // Error Handling
    if(!query->isActive()) {
        ui->lblQueryStatus->setText("Can't load teacher list, try to add a new record first!");
        return;
    }
    
    model->setQuery(*query);
    ui->cbnTeacher->setModel(model);

    // Close the connection to the database
    QString con;
    con = db.connectionName();
    db.close();
    db = QSqlDatabase();
    db.removeDatabase(con);
}

void createDB::on_btnAddTeacher_clicked() {
    // First check if the user input is empty
    if(ui->lnTeacherName->text().isEmpty() || ui->lnTeacherSurname->text().isEmpty()) {
        ui->lblQueryStatus->setText("Please fill the input boxes!");
        return;
    }

    // Fetch the user input
    this->teacherName = ui->lnTeacherName->text();
    this->teacherSurname = ui->lnTeacherSurname->text();

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

    // Our Query
    QSqlQuery query;

    // Now we want to insert the user input into the database
    // For security reason, we want to bind those values.
    query.prepare("INSERT INTO teacher (TName, TSurname) VALUES (:name, :surname);");
    query.bindValue(":name", this->teacherName);
    query.bindValue(":surname", this->teacherSurname);

    // Error Handling
    if(!query.exec()) {
        ui->lblQueryStatus->setText(query.lastError().text());
        return;
    }
    
    // Print a status message for 1.5 seconds(1500 ms)
    ui->lblQueryStatus->setText("Teacher added successfully!");
    QTimer::singleShot(1500, ui->lblQueryStatus, [&](){ ui->lblQueryStatus->setText(" "); });

    // Close the connection to the database
    QString con;
    con = db.connectionName();
    db.close();
    db = QSqlDatabase();
    db.removeDatabase(con);

    // Then refresh the Subject combobox
    loadTeachers();
}

void createDB::on_btnAddSubject_clicked() {
    // First check if the user input is empty
    if(ui->lnSubject->text().isEmpty() || ui->cbnTeacher->currentText().isEmpty()) {
        ui->lblQueryStatus->setText("Please fill the input boxes!");
        return;
    }

    // Fetch the user input
    this->subject = ui->lnSubject->text();
    this->teacherSurname = ui->cbnTeacher->currentText();

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

    // Our Query
    QSqlQuery query;

    // Retrive the ID of the teacher choosed by the user
    query.prepare("SELECT ID FROM teacher WHERE TSurname = :surname LIMIT 1;");
    query.bindValue(":surname", this->teacherSurname);
    
    if(!query.exec()) {
        ui->lblQueryStatus->setText(query.lastError().text());
        return;
    }

    // Store the result of the query(the Teacher's ID) into a local variable
    unsigned int id;
    if(query.first())
        id = query.value(0).toInt();
    else {
        ui->lblQueryStatus->setText(query.lastError().text());
        return;
    }

    query.prepare("INSERT INTO subject (SubName,CodTeacher) VALUES("
                  ":subname, :codTeacher);");
    query.bindValue(":subname", this->subject);
    query.bindValue(":codTeacher", id);

    // Then insert the subject
    if(!query.exec()) {
        ui->lblQueryStatus->setText(query.lastError().text());
        return;
    }
    else { 
        // else print a status message for 1.5 seconds.
        ui->lblQueryStatus->setText("Subject added successfully!");
        QTimer::singleShot(1500, ui->lblQueryStatus, [&](){ ui->lblQueryStatus->setText(" "); });
    }

    // Close the connection to the database
    QString con;
    con = db.connectionName();
    db.close();
    db = QSqlDatabase();
    db.removeDatabase(con);
}

void createDB::on_actionUpdate_triggered() {
    upTSWin = new upTS(this->dbPath, nullptr);
    upTSWin->show();
    upTSWin->setGeometry(
        QStyle::alignedRect(
            Qt::LeftToRight, 
            Qt::AlignCenter, 
            upTSWin->size(), 
            qApp->desktop()->availableGeometry()
        )
    );
}

void createDB::on_actionDelete_triggered() {
    delTSWin = new delTS(this->dbPath, nullptr);
    delTSWin->show();
    delTSWin->setGeometry(
        QStyle::alignedRect(
            Qt::LeftToRight, 
            Qt::AlignCenter, 
            delTSWin->size(), 
            qApp->desktop()->availableGeometry()
        )
    );
}

void createDB::on_actionDBCreate_triggered() {
    // Get the file path
    path pt;
    this->dbPath = pt.set_path();
    if(this->dbPath.isEmpty())
        return;

    // Load the SQLite driver
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(this->dbPath);
    if(!db.open()) {
        QMessageBox::critical(nullptr, QObject::tr("Cannot open the database!"),
           QObject::tr(db.lastError().text().toLocal8Bit().data()), QMessageBox::Ok); 
        return;
    }

    // Our Query
    QSqlQuery query;

     // drop previous exists tables
    query.exec("DROP TABLE IF EXISTS teacher;");
    query.exec("DROP TABLE IF EXISTS subject;");
    query.exec("DROP TABLE IF EXISTS mark;");

    // Create the three tables
    query.exec("CREATE TABLE teacher ("
                "ID INTEGER PRIMARY KEY,"
                "TName TEXT NOT NULL,"
                "TSurname TEXT NOT NULL);");

    query.exec("CREATE TABLE subject ("
               "ID INTEGER PRIMARY KEY,"
               "SubName TEXT NOT NULL,"
               "CodTeacher INTEGER NOT NULL,"
               "FOREIGN KEY (CodTeacher) REFERENCES teacher(ID));");

    query.exec("CREATE TABLE mark ("
               "ID INTEGER PRIMARY KEY,"
               "Mark FLOAT NOT NULL,"
               "MarkDate DATE NOT NULL,"
               "Description TEXT NOT NULL,"
               "CodSub INTEGER NOT NULL,"
               "FOREIGN KEY (CodSub) REFERENCES subject(ID));");

    // Error Handling
    if(!query.isActive()) {
        ui->lblQueryStatus->setText(query.lastError().text());
        return;
    }

    // Print a status message for 1.5 seconds(1500 ms)
    ui->lblQueryStatus->setText("database created successfully!");
    QTimer::singleShot(1500, ui->lblQueryStatus, [&](){ ui->lblQueryStatus->setText(" "); });

    // Close the connection to the database
    QString con;
    con = db.connectionName();
    db.close();
    db = QSqlDatabase();
    db.removeDatabase(con);
}

createDB::~createDB() { delete ui; }