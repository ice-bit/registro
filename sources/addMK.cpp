#include "addMK.h"

addMK::addMK(QString dbPath, QWidget *parent) : QMainWindow(parent), ui(new Ui::addMKClass) {
    ui->setupUi(this);
    setFixedSize(681, 155);

    // Free QObjects(needed to avoid memory leaks)
    setAttribute(Qt::WA_DeleteOnClose);

    // Update the date widget with current date
    ui->dtMark->setDate(QDate::currentDate());

    // Initialize the database path if it already exists
    if(dbPath != nullptr)
        this->dbPath = dbPath;
    // Then load the subject from the database
    loadSubject();
}

void addMK::loadSubject() {
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
    query->exec("SELECT SubName FROM subjects;");

    // Error handling
    if(!query->isActive()) {
        ui->lblQueryStatus->setText("Can't load subject list, try to add a new subject first!");
        return;
    }
    
    // Put the result of the query into our model
    model->setQuery(*query);

    // Display our modle into QComboBox
    ui->cbnSubject->setModel(model);

    // Close the connection to the database
    QString con;
    con = db.connectionName();
    db.close();
    db = QSqlDatabase();
    db.removeDatabase(con);

    // Delete heap objects
    delete query;
}

void addMK::on_btnInsertMark_clicked() {
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

    query.prepare("SELECT ID FROM subjects WHERE SubName = :subname LIMIT 1;");
    query.bindValue(":subname", this->mkSub);

    // Error Handling
    if(!query.exec()) {
        ui->lblQueryStatus->setText(query.lastError().text());
        return;
    }
    
    // Store the result of the query into a local variable
    unsigned int id;
    if(query.first())
        id = query.value(0).toInt();
    else
        ui->lblQueryStatus->setText(query.lastError().text());

    // Now we're able to prepare our insert query
    query.prepare("INSERT INTO marks(Mark, MarkDate, Description, CodSub) VALUES ("
                  ":mark, :date, :desc, :sub);");
    query.bindValue(":mark", this->mkMark);
    query.bindValue(":date", this->mkDate);
    query.bindValue(":desc", this->mkDesc);
    query.bindValue(":sub", id);

    // Execute the query(with error handling)
    if(!query.exec()) {
        ui->lblQueryStatus->setText(query.lastError().text());
        return;
    }

    // Print a status message for 1.5 seconds(1500 ms)
    ui->lblQueryStatus->setText("Entry added successfully!");
    QTimer::singleShot(1500, ui->lblQueryStatus, [&](){ ui->lblQueryStatus->setText(" "); });
    
    // Close the connection to the database
    QString con;
    con = db.connectionName();
    db.close();
    db = QSqlDatabase();
    db.removeDatabase(con);
}

addMK::~addMK() { delete ui; }