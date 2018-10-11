#include "registro.h"

regMain::regMain(QWidget *parent) : QMainWindow(parent), ui(new Ui::regMainClass) {
    ui->setupUi(this);
}

void regMain::on_actionCreateDB_triggered() {
    createDBWin = new createDB();
    createDBWin->show();
}

void regMain::on_btnLoadElements_clicked() {
    // Connect to the database
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("debug.db");
    if(!db.open()) {
        QMessageBox::critical(nullptr, QObject::tr("Cannot open the database!"),
            QObject::tr("Unable to create a database connection!"), QMessageBox::Cancel);
        return;
    }

    // Create a new query model(which is read only) and a new query(in the Heap memory)
    QSqlQueryModel *model = new QSqlQueryModel();


    QSqlQuery *query = new QSqlQuery(db);
    // Execute the query
    query->exec("SELECT m.mark, s.SubName, m.MarkDate, m.Description, t.TSurname "
                "FROM mark AS m "
                "INNER JOIN subject AS s "
                "ON m.CodSub = s.ID "
                "INNER JOIN teacher AS t "
                "ON s.CodTeacher = t.ID;");
    // put the result of the query into our model
    model->setQuery(*query);
    // Display our model into the QTableView.
    ui->tbMain->setModel(model);
    // For a better result, set tbe Columns width automatically
    ui->tbMain->horizontalHeader()->setStretchLastSection(true);
    // Configure column's labels with appropriate names
    model->setHeaderData(0, Qt::Horizontal, tr("Mark"));
    model->setHeaderData(1, Qt::Horizontal, tr("Subject"));
    model->setHeaderData(2, Qt::Horizontal, tr("Date"));
    model->setHeaderData(3, Qt::Horizontal, tr("Description"));
    model->setHeaderData(4, Qt::Horizontal, tr("Teacher Name"));
}

regMain::~regMain() { delete ui; }
