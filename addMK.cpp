#include "addMK.h"

addMK::addMK(QWidget *parent) : QMainWindow(parent), ui(new Ui::addMKClass) {
    ui->setupUi(this);
    setFixedSize(681, 155);

    // Free QObjects(needed to avoid memory leaks)
    setAttribute(Qt::WA_DeleteOnClose);

    // Update date widget with current date
    ui->dtMark->setDate(QDate::currentDate());
}

void addMK::on_actionRefresh_triggered() {
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

    // Execute the query
    query->exec("SELECT SubName FROM subject;");

    // Error handling
    if(!query->isActive())
        ui->lblQueryStatus->setText("Can't load subject list, try to add a new subject first!");
    
    // Put the result of the query into our model
    model->setQuery(*query);

    // Display our modle into QComboBox
    ui->cbnSubject->setModel(model);

    // Delete heap objects
    delete query;
}

void addMK::on_btnInsertSubject_clicked() {
    // Check if user inputs are empty
    if(ui->spnMark->text().isEmpty() || ui->dtMark->text().isEmpty() || ui->lnDescription->text().isEmpty() || ui->cbnSubject->currentText().isEmpty()) {
        ui->lblQueryStatus->setText("Please fill the input boxes!");
        return;
    }

    // Fetch user input 
    this->mkMark = ui->spnMark->value();
    this->mkDate = ui->dtMark->date().toString("dd/MM/yyyy");
    this->mkDesc = ui->lnDescription->text();
    this->mkSub = ui->cbnSubject->currentText();
        
    // Load the SQLite driver
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("debug.db");
    if(!db.open()) {
        QMessageBox::critical(nullptr, QObject::tr("Cannot open the database!"),
            QObject::tr("Unable to create a database connection!"), QMessageBox::Cancel);
        return;
    }
    
    // Our query
    QSqlQuery query;

    query.prepare("SELECT ID FROM subject WHERE SubName = :subname LIMIT 1;");
    query.bindValue(":subname", this->mkSub);

    // Error Handling
    if(!query.exec())
        ui->lblQueryStatus->setText("Error while executing this query!");
    
    // Store the result of the query into a local variable
    unsigned int id;
    if(query.first())
        id = query.value(0).toInt();
    else
        ui->lblQueryStatus->setText("Error while executing this query!");

    // Now we're able to prepare our insert query
    query.prepare("INSERT INTO mark(Mark, MarkDate, Description, CodSub) VALUES ("
                  ":mark, :date, :desc, :sub);");
    query.bindValue(":mark", this->mkMark);
    query.bindValue(":date", this->mkDate);
    query.bindValue(":desc", this->mkDesc);
    query.bindValue(":sub", id);

    // Execute the query(with error handling)
    if(!query.exec())
        ui->lblQueryStatus->setText("Error while executing this query!");

    // Print a status message for 1.5 seconds(1500 ms)
    ui->lblQueryStatus->setText("Mark added successfully!");
    QTimer::singleShot(1500, ui->lblQueryStatus, [&](){ ui->lblQueryStatus->setText(" "); });
    
    // Close the connection to the database
    db.close();
}

addMK::~addMK() { delete ui; }