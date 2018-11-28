#include "registro.h"

regMain::regMain(QWidget *parent) : QMainWindow(parent), ui(new Ui::regMainClass) {
    ui->setupUi(this);

    // Set columns auto resize
    ui->tbMain->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    // Disable row's indices
    ui->tbMain->verticalHeader()->setVisible(false);

    // Disable delete button before creating the QTableView
    ui->btnDelElements->setEnabled(false);

    // Custom Signal
    QObject::connect(ui->lnSearch, SIGNAL(textChanged(QString)), this, SLOT(searchSubject()));
}

void regMain::on_actionCreateDB_triggered() {
    createDBWin = new createDB();
    createDBWin->show();
}

void regMain::on_btnLoadElements_clicked() {
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

    // Create a new query model(which is read only) and a new query(in the Heap memory)
    QSqlQueryModel *model = new QSqlQueryModel();

    QSqlQuery *query = new QSqlQuery(db);
    // Execute the query

    if(!query->exec("SELECT m.ID, m.mark, s.SubName, m.MarkDate, m.Description, t.TSurname "
                "FROM mark AS m "
                "INNER JOIN subject AS s "
                "ON m.CodSub = s.ID "
                "INNER JOIN teacher AS t "
                "ON s.CodTeacher = t.ID;")) {
        ui->lblQueryStatus->setText("Error while executing this query!");
        return;
    }
    // put the result of the query into our model
    model->setQuery(*query);
    // Display our model into the QTableView.
    ui->tbMain->setModel(model);
    // For a better result, set tbe Columns width automatically
    ui->tbMain->horizontalHeader()->setStretchLastSection(true);
    // Configure column's labels with appropriate names
    model->setHeaderData(0, Qt::Horizontal, tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, tr("Mark"));
    model->setHeaderData(2, Qt::Horizontal, tr("Subject"));
    model->setHeaderData(3, Qt::Horizontal, tr("Date"));
    model->setHeaderData(4, Qt::Horizontal, tr("Description"));
    model->setHeaderData(5, Qt::Horizontal, tr("Teacher Name"));

    // Close the connection to the database
    QString con;
    con = db.connectionName();
    db.close();
    db = QSqlDatabase();
    db.removeDatabase(con);

    /* Average section */

    // Clear the vector
    this->marks.clear();
    // put the values of the marks column into a vector
    for(int col = 0; col < ui->tbMain->model()->rowCount(); col++) {
        QVariant index = ui->tbMain->model()->data(ui->tbMain->model()->index(col, 1));
        float value = index.value<float>();
        this->marks.push_back(value);
    }
    
    if(!this->marks.empty()) {
        // Compute the average
        float marks_avg = avg(this->marks);
        // Display it to the user
        if(std::isnan(marks_avg))
            ui->lblAvg->setText("/");
        else
            ui->lblAvg->setText(QString::number(marks_avg));
    } else
        ui->lblAvg->setText("/");
        
    // Delete heap objects
    delete query;

    // Enable delete button and search field
    ui->btnDelElements->setEnabled(true);
    ui->lnSearch->setEnabled(true);
}

void regMain::on_btnAddElements_clicked() {
    addMKWin = new addMK();
    addMKWin->show();
}

void regMain::on_btnUpElements_clicked() {
    upMKWin = new upMK;
    upMKWin->show();
}

void regMain::on_btnDelElements_clicked() {
    // Retrive user selection
    QModelIndex index = ui->tbMain->selectionModel()->currentIndex();

    // Check how many cells are being selected
    QModelIndexList selection = ui->tbMain->selectionModel()->selectedIndexes();
    if(selection.count() > 1) {
        ui->lblQueryStatus->setText("Please select one ID at a time.");
        return;
    }
    else if(!ui->tbMain->selectionModel()->hasSelection()) {
        ui->lblQueryStatus->setText("Please select at least one ID.");
        return;
    }
    else if(index.column() != 0) {
        ui->lblQueryStatus->setText("Please select an element from the ID column.");
        return;
    }

    // Retrive the selected cell
    QVariant indexValue = index.sibling(index.row(), index.column()).data();
    // Convert it to an integer
    this->userSelection = indexValue.value<int>();
    
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

    // Our query
    QSqlQuery query;

    // Prepare and execute the delete query
    query.prepare("DELETE FROM mark WHERE ID = :id;");
    query.bindValue(":id", this->userSelection);

    // Error Handling
    if(!query.exec()) {
        ui->lblQueryStatus->setText("Error while executing this query!");
        return;
    }
    
    // Print a status message for 1.5 seconds(1500ms)
    ui->lblQueryStatus->setText("Mark deleted successfully!");
    QTimer::singleShot(1500, ui->lblQueryStatus, [&](){ ui->lblQueryStatus->setText(" "); });

    // Close the connection to the database
    QString con;
    con = db.connectionName();
    db.close();
    db = QSqlDatabase();
    db.removeDatabase(con);
}


float regMain::avg(std::vector<float> marks) {
    long sum = 0;

    for(int i = 0; i < marks.size(); i++)
        sum += marks.at(i);
    
    return round(sum / marks.size() * 100) / 100;
}

void regMain::searchSubject() {
    // Retrieve requested subject
    this->reqsub = ui->lnSearch->text();
    
    // Connect to the database
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(this->file);
    if(!db.open()) {
        QMessageBox::critical(nullptr, QObject::tr("Cannot open the database!"),
            QObject::tr("Unable to create a database connection!"), QMessageBox::Ok);
        return;
    }

    // Create a new query model(which is read only) and a new query(in the Heap memory)
    QSqlQueryModel *model = new QSqlQueryModel();

    QSqlQuery *query = new QSqlQuery(db);
    // Execute the query
    query->prepare("SELECT m.ID, m.mark, s.SubName, m.MarkDate, m.Description, t.TSurname "
                "FROM mark AS m "
                "INNER JOIN subject AS s "
                "ON m.CodSub = s.ID "
                "INNER JOIN teacher AS t "
                "ON s.CodTeacher = t.ID "
                "WHERE s.SubName LIKE :subname;");
    query->bindValue(":subname", QString("%%1%").arg(this->reqsub));

    if(!query->exec()) {
        ui->lblQueryStatus->setText("Error while executing this query!");
        return;
    }

    // put the result of the query into our model
    model->setQuery(*query);
    // Display our model into the QTableView.
    ui->tbMain->setModel(model);
    // For a better result, set tbe Columns width automatically
    ui->tbMain->horizontalHeader()->setStretchLastSection(true);
    // Configure column's labels with appropriate names
    model->setHeaderData(0, Qt::Horizontal, tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, tr("Mark"));
    model->setHeaderData(2, Qt::Horizontal, tr("Subject"));
    model->setHeaderData(3, Qt::Horizontal, tr("Date"));
    model->setHeaderData(4, Qt::Horizontal, tr("Description"));
    model->setHeaderData(5, Qt::Horizontal, tr("Teacher Name"));
    
    // Close the connection to the database
    QString con;
    con = db.connectionName();
    db.close();
    db = QSqlDatabase();
    db.removeDatabase(con);

    /* Average section */

    // Clear the vector
    this->marks.clear();

    // put the values of the marks column into a vector
    for(int col = 0; col < ui->tbMain->model()->rowCount(); col++) {
        QVariant index = ui->tbMain->model()->data(ui->tbMain->model()->index(col, 1));
        float value = index.value<float>();
        this->marks.push_back(value);
    
    }
    if(!this->marks.empty()) {
        // Compute the average
        float marks_avg = avg(this->marks);
        // Display it to the user
        if(std::isnan(marks_avg))
            ui->lblAvg->setText("/");
        else
            ui->lblAvg->setText(QString::number(marks_avg));
    } else
        ui->lblAvg->setText("/");

    // Delete heap objects
    delete query;

    // Enable delete button
    ui->btnDelElements->setEnabled(true);
}

void regMain::on_actionChangeDB_triggered() {
    path pt;
    this->file = pt.get_path();
    ui->lblQueryStatus->setText("Database changed, reload it");
}

void regMain::on_actionAbout_triggered() {
    aboutWin = new about();
    aboutWin->show();
}

void regMain::on_actionExit_triggered() {
    exit(0);
}

regMain::~regMain() { delete ui; }

