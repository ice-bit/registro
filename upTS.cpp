#include "upTS.h"

upTS::upTS(QWidget *parent) : QMainWindow(parent), ui(new Ui::upTSClass) {
    ui->setupUi(this);
    setFixedSize(912, 560);

    // Free QObjet(needed to avoid memory leaks)
    setAttribute(Qt::WA_DeleteOnClose);
    ui->tbSubjects->horizontalHeader()->setStretchLastSection(true);
    ui->tbTeachers->horizontalHeader()->setStretchLastSection(true);
}

void upTS::on_actionRefresh_triggered() {
    // Get user path
    if(this->file == nullptr) {
        path pt;
        this->file = pt.get_path();
    }
    // Load the SQLite driver
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(this->file);
    if(!db.open()) {
        QMessageBox::critical(nullptr, QObject::tr("Cannot open the database!"),
            QObject::tr("Unable to create a database connection!"), QMessageBox::Ok);
        return;
    }

    // Create a query model to store query result set.
    QSqlQueryModel *mainmodel = new QSqlQueryModel();
    QSqlQueryModel *submodel = new QSqlQueryModel();
    QSqlQueryModel *cbnmodel = new QSqlQueryModel();
    // Our Query
    QSqlQuery *mainquery = new QSqlQuery(db);
    QSqlQuery *subquery = new QSqlQuery(db);
    QSqlQuery *cbnquery = new QSqlQuery(db);

    // Execute the query
    mainquery->exec("SELECT m.ID, m.SubName, t.TSurname FROM subject AS m "
                    "INNER JOIN teacher AS t ON m.CodTeacher = t.ID;");

    // Error Handling
    if(!mainquery->isActive()) {
        ui->lblQueryStatus->setText("Can't load subject list!");
        return;
    }

    // Put the result of the query into our model
    mainmodel->setQuery(*mainquery);

    // Display our model into the QTableView.
    ui->tbSubjects->setModel(mainmodel);

    // Configure column's labels with appropriate names
    mainmodel->setHeaderData(0, Qt::Horizontal, tr("ID"));
    mainmodel->setHeaderData(1, Qt::Horizontal, tr("Subject"));
    mainmodel->setHeaderData(2, Qt::Horizontal, tr("Teacher Surname"));


    // Now do the same thing with the other table(Teachers)

    // Load teachers into Qtable
    subquery->exec("SELECT ID, TName, TSurname FROM teacher;");

    // Error Handling
    if(!subquery->isActive()) {
        ui->lblQueryStatus->setText("Can't load subject list!");
        return;
    }

    // Put the result of the query into our model
    submodel->setQuery(*subquery);

    // Display our model into the QTableView.
    ui->tbTeachers->setModel(submodel);

    // Configure column's labels with appropriate names
    submodel->setHeaderData(0, Qt::Horizontal, tr("ID"));
    submodel->setHeaderData(1, Qt::Horizontal, tr("Teacher Name"));
    submodel->setHeaderData(2, Qt::Horizontal, tr("Teacher Surname"));

    // Now do the same thing with the other QObject
    cbnquery->exec("SELECT TSurname FROM teacher;");

    if(!cbnquery->isActive()) {
        ui->lblQueryStatus->setText("Can't load teacher list, try to add a new record first!");
        return;
    }

    cbnmodel->setQuery(*cbnquery);
    ui->cbnTeacher->setModel(cbnmodel);

    // Close the connection to the database
    QString con;
    con = db.connectionName();
    db.close();
    db = QSqlDatabase();
    db.removeDatabase(con);

    // Adjust the column width
    ui->tbSubjects->setColumnWidth(1, 150);
    ui->tbTeachers->setColumnWidth(1, 150);
    ui->tbSubjects->setColumnWidth(2, 150);
    ui->tbTeachers->setColumnWidth(2, 150);

    // Remove row index
    ui->tbSubjects->verticalHeader()->setVisible(false);
    ui->tbTeachers->verticalHeader()->setVisible(false);

    // Delete heap objects
    delete mainquery;
    delete subquery;
}

void upTS::on_btnSubUpdate_clicked() {
    // Check if teacher is empty
    if(ui->cbnTeacher->currentText().isEmpty()) {
        ui->lblQueryStatus->setText("Please refresh the list of teachers and choose an available entry");
        return;
    }

    // Fetch user input
    this->id = ui->spnSubID->value();
    this->subName = ui->lnSubName->text();
    this->tcName = ui->cbnTeacher->currentText();


    // Get user path
    if(this->file == nullptr) {
        path pt;
        this->file = pt.get_path();
    }
    // Connect to the database
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(this->file);
    if(!db.open()) {
        QMessageBox::critical(nullptr, QObject::tr("Cannot open the database!"),
            QObject::tr("Unable to create a database connection!"), QMessageBox::Ok);
        return;
    }

    // Our query
    QSqlQuery query;

    query.prepare("SELECT ID FROM teacher WHERE TSurname = :surname LIMIT 1;");
    query.bindValue(":surname", this->tcName);
    

    if(!query.exec()) {
        ui->lblQueryStatus->setText("Error while executing this query!");
        return;
    }
        
    // Store the result of the query into a local variable
    unsigned int id;
    if(query.first())
        id = query.value(0).toInt();
    else
        ui->lblQueryStatus->setText("Error while executing this query!");
        

    if(ui->lnSubName->text().isEmpty() && ui->cbnTeacher->currentText().isEmpty()) {
        ui->lblQueryStatus->setText("Plase fill the input boxes!");
        return;
    } else if(ui->lnSubName->text().isEmpty()) {
        query.prepare("UPDATE subject SET CodTeacher = :tcid WHERE ID = :id;");
        query.bindValue(":tcid", id);
        query.bindValue(":id", this->id);

        // Execute the query
        if(!query.exec()) {
            ui->lblQueryStatus->setText("Error while executing this query!");
            return;
        }
        else {
            ui->lblQueryStatus->setText("Subject updated successfully!");
            QTimer::singleShot(1500, ui->lblQueryStatus, [&](){ ui->lblQueryStatus->setText(" "); });
        }

    } else {
        query.prepare("UPDATE subject SET CodTeacher = :tcid, SubName = :sbname WHERE ID = :id;");
        query.bindValue(":tcid", id);
        query.bindValue(":sbname", this->subName);
        query.bindValue(":id", this->id);

        // Execute the query
        if(!query.exec()) {
            ui->lblQueryStatus->setText("Error while executing this query!");
            return;
        }
        else {
            ui->lblQueryStatus->setText("Subject updated successfully!");
            QTimer::singleShot(1500, ui->lblQueryStatus, [&](){ ui->lblQueryStatus->setText(" "); });
        }
    }

    // Close the connection to the database
    QString con;
    con = db.connectionName();
    db.close();
    db = QSqlDatabase();
    db.removeDatabase(con);
}

void upTS::on_btnTeacherUpdate_clicked() {
    // Check if user inputs are empty
    if(ui->lnTeachName->text().isEmpty() && ui->lnTeachSurname->text().isEmpty()) {
        ui->lblQueryStatus->setText("Plase fill at least one input box");
        return;
    }

    // Fetch user input
    this->id = ui->spnTeachID->value();
    this->tcName = ui->lnTeachName->text();
    this->tcSurname = ui->lnTeachSurname->text();

    // Get user path
    if(this->file == nullptr) {
        path pt;
        this->file = pt.get_path();
    }
    // Connect to the database
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(this->file);
    if(!db.open()) {
        QMessageBox::critical(nullptr, QObject::tr("Cannot open the database!"),
            QObject::tr("Unable to create a database connection!"), QMessageBox::Ok);
        return;
    }

    // Our query
    QSqlQuery query;

    if(ui->lnTeachName->text().isEmpty()) {
        // Prepare the query
        query.prepare("UPDATE teacher SET TSurname = :surname WHERE ID = :id;");
        query.bindValue(":surname", this->tcSurname);
        query.bindValue(":id", this->id);

        // Exec the query
        if(!query.exec()) {
            ui->lblQueryStatus->setText("Error while executing this query!");
            return;
        }
        else {
            ui->lblQueryStatus->setText("Teacher updated successfully!");
            QTimer::singleShot(1500, ui->lblQueryStatus, [&](){ ui->lblQueryStatus->setText(" "); });
        }
    } else if(ui->lnTeachSurname->text().isEmpty()) {
        // Prepare the query
        query.prepare("UPDATE teacher SET TName = :name WHERE ID = :id;");
        query.bindValue(":name", this->tcName);
        query.bindValue(":id", this->id);

        // Exec the query
        if(!query.exec()) {
            ui->lblQueryStatus->setText("Error while executing this query!");
            return;
        }
        else {
            ui->lblQueryStatus->setText("Teacher updated successfully!");
            QTimer::singleShot(1500, ui->lblQueryStatus, [&](){ ui->lblQueryStatus->setText(" "); });
        }
    } else {
        // Prepare the query
        query.prepare("UPDATE teacher SET TName = :name, TSurname = :surname WHERE ID = :id;");
        query.bindValue(":name", this->tcName);
        query.bindValue(":surname", this->tcSurname);
        query.bindValue(":id", this->id);

        // Exec the query
        if(!query.exec()) {
            ui->lblQueryStatus->setText("Error while executing this query!");
            return;
        }
        else {
            ui->lblQueryStatus->setText("Teacher updated successfully!");
            QTimer::singleShot(1500, ui->lblQueryStatus, [&](){ ui->lblQueryStatus->setText(" "); });
        }
    }
    
    // Close the connection to the database
    QString con;
    con = db.connectionName();
    db.close();
    db = QSqlDatabase();
    db.removeDatabase(con);
}

upTS::~upTS() { delete ui; }