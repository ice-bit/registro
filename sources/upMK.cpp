#include "upMK.h"

upMK::upMK(QString dbPath, QWidget *parent) : QMainWindow(parent), ui(new Ui::upMKClass) {
    ui->setupUi(this);
    setFixedSize(741, 155);

    // Free QObjects(needed to avoid memory leaks)
    setAttribute(Qt::WA_DeleteOnClose);

    // Update the date widget with current date
    ui->dtMark->setDate(QDate::currentDate());

    // Initialize the database path if it already exists
    if(dbPath != nullptr)
        this->dbPath = dbPath;
}

void upMK::on_actionRefresh_triggered() {
    // Get user path
    if(this->dbPath == nullptr) {
        path pt;
        this->dbPath = pt.get_path();
    }
    // Connect to the database
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

    // Execute the query
    query->exec("SELECT SubName FROM subject;");

    // Error handling
    if(!query->isActive()) {
        ui->lblQueryStatus->setText("Can't load subject list, try to add a new subject first!");
        return;
    }
    
    // Put the result of the query into our model
    model->setQuery(*query);

    // Display our modle into QComboBox
    ui->cbnSubject->setModel(model);

    // Delete heap objects
    delete query;

    // Close the connection to the database
    QString con;
    con = db.connectionName();
    db.close();
    db = QSqlDatabase();
    db.removeDatabase(con);
}

void upMK::on_btnUpdateMark_clicked() {
    // Check user inputs
    if(ui->spnMark->text().isEmpty() || ui->dtMark->text().isEmpty() || ui->lnDescription->text().isEmpty() || ui->cbnSubject->currentText().isEmpty()) {
        ui->lblQueryStatus->setText("Please fill the input boxes!");
        return;
    }

    // Fetch user input
    this->mkMark = ui->spnMark->value();
    this->mkDate = ui->dtMark->date().toString("dd/MM/yyyy");
    this->mkDesc = ui->lnDescription->text();
    this->mkSub = ui->cbnSubject->currentText();
    this->mkID = ui->spnID->value();

    // Get user path
    if(this->dbPath == nullptr) {
        path pt;
        this->dbPath = pt.get_path();
    }
    // Connect to the database
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(this->dbPath);
    if(!db.open()) {
        QMessageBox::critical(nullptr, QObject::tr("Cannot open the database!"),
           QObject::tr(db.lastError().text().toLocal8Bit().data()), QMessageBox::Ok); 
        return;
    }

    // Our query
    QSqlQuery query;

    // Retrive the ID from the Subject name
    query.prepare("SELECT ID FROM subject WHERE SubName = :subname LIMIT 1;");
    query.bindValue(":subname", this->mkSub);

    // Error handling
    if(!query.exec()) {
        ui->lblQueryStatus->setText(query.lastError().text());
        return;
    }
    
    // Store the result into a integer variable
    unsigned int id;
    if(query.first())
        id = query.value(0).toInt();
    else {
        ui->lblQueryStatus->setText(query.lastError().text());
        return;
    }
    

    // Now update the record
    query.prepare("UPDATE mark SET Mark = :mark, MarkDate = :date, Description = "
                  ":desc, CodSub = :sub WHERE ID = :id;");
    query.bindValue(":mark", this->mkMark);
    query.bindValue(":date", this->mkDate);
    query.bindValue(":desc", this->mkDesc);
    query.bindValue(":sub", id);
    query.bindValue(":id", this->mkID);

    if(!query.exec()) {
        ui->lblQueryStatus->setText(query.lastError().text());
        return;
    }

    // Print a status message for 1.5 seconds(1500ms)
    ui->lblQueryStatus->setText("Mark updated successfully!");
    QTimer::singleShot(1500, ui->lblQueryStatus, [&](){ ui->lblQueryStatus->setText(" "); });

    // Close the connection to the database
    QString con;
    con = db.connectionName();
    db.close();
    db = QSqlDatabase();
    db.removeDatabase(con);
}

upMK::~upMK() { delete ui; }