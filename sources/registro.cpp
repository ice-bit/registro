#include "registro.h"

regMain::regMain(QWidget *parent) : QMainWindow(parent), ui(new Ui::regMainClass) {
    ui->setupUi(this);

    // Set columns auto resize
    ui->tbMain->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    // Disable row's indices
    ui->tbMain->verticalHeader()->setVisible(false);

    // Disable delete button before creating the QTableView
    ui->btnDelElements->setEnabled(false);

    // Update window title with current version
    std::string window_title = std::string("Registro ") + std::string(PROJECT_VERSION);
    setWindowTitle(QString::fromStdString(window_title));

    // Custom Signal
    QObject::connect(ui->lnSearch, SIGNAL(textChanged(QString)), this, SLOT(searchSubject()));
}

void regMain::on_actionCreateDB_triggered() {
    createDBWin = new createDB(this->dbPath, nullptr);
    createDBWin->show();
    createDBWin->setGeometry(
        QStyle::alignedRect(
            Qt::LeftToRight, 
            Qt::AlignCenter, 
            createDBWin->size(), 
            qApp->primaryScreen()->availableGeometry()
        )
    );
}

void regMain::on_btnLoadElements_clicked() {
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

    // Create a new query model(which is read only) and a new query(in the Heap memory)
    QSqlQueryModel *model = new QSqlQueryModel();

    QSqlQuery *query = new QSqlQuery(db);
    // Execute the query

    if(!query->exec("SELECT m.ID, m.mark, s.SubName, m.MarkDate, m.Description, t.TSurname "
                "FROM marks AS m "
                "INNER JOIN subjects AS s "
                "ON m.CodSub = s.ID "
                "INNER JOIN teachers AS t "
                "ON s.CodTeacher = t.ID;")) {
        ui->lblQueryStatus->setText(query->lastError().text());
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
    model->setHeaderData(1, Qt::Horizontal, tr("Grade"));
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
    float marks_avg = 0;
    marks_avg = avg(1, nullptr);

    if(marks_avg == 0) {
        ui->lblAvg->setText("/");
    } else
        ui->lblAvg->setText(QString::number(marks_avg));
        
    // Delete heap objects
    delete query;

    // Enable other functions
    ui->btnDelElements->setEnabled(true);
    ui->lnSearch->setEnabled(true);
    ui->btnAddElements->setEnabled(true);
    ui->btnUpElements->setEnabled(true);
}

void regMain::on_btnAddElements_clicked() {
    addMKWin = new addMK(this->dbPath, nullptr);
    addMKWin->show();
    addMKWin->setGeometry(
        QStyle::alignedRect(
            Qt::LeftToRight, 
            Qt::AlignCenter, 
            addMKWin->size(), 
            qApp->primaryScreen()->availableGeometry()
        )
    );
}

void regMain::on_btnUpElements_clicked() {
    upMKWin = new upMK(this->dbPath, nullptr);
    upMKWin->show();
    upMKWin->setGeometry(
        QStyle::alignedRect(
            Qt::LeftToRight, 
            Qt::AlignCenter, 
            upMKWin->size(), 
            qApp->primaryScreen()->availableGeometry()
        )
    );
}

void regMain::on_btnDelElements_clicked() {
    // Retrive user selection
    QModelIndex index = ui->tbMain->selectionModel()->currentIndex();

    // Check how many cells are being selected
    QModelIndexList selection = ui->tbMain->selectionModel()->selectedIndexes();
    if(selection.count() > 1) {
        ui->lblQueryStatus->setText("Please select one ID at time.");
        return;
    } else if(!ui->tbMain->selectionModel()->hasSelection()) {
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

    // Prepare and execute the delete query
    query.prepare("DELETE FROM marks WHERE ID = :id;");
    query.bindValue(":id", this->userSelection);

    // Error Handling
    if(!query.exec()) {
        ui->lblQueryStatus->setText(query.lastError().text());
        return;
    }
    
    // Print a status message for 1.5 seconds(1500ms)
    ui->lblQueryStatus->setText("Entry deleted successfully!");
    QTimer::singleShot(1500, ui->lblQueryStatus, [&](){ ui->lblQueryStatus->setText(" "); });

    // Close the connection to the database
    QString con;
    con = db.connectionName();
    db.close();
    db = QSqlDatabase();
    db.removeDatabase(con);
}


float regMain::avg(int operation, QString subname) {
    float avg;
    // Connect to the database
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(this->dbPath);
    if(!db.open()) {
        QMessageBox::critical(nullptr, QObject::tr("Cannot open the database!"),
           QObject::tr(db.lastError().text().toLocal8Bit().data()), QMessageBox::Ok); 
        return 0;
    }

    QSqlQuery *query = new QSqlQuery(db);
    if(operation == 1) {
        if(!query->exec("SELECT ROUND(AVG(Mark),1) FROM marks;")) {
            ui->lblQueryStatus->setText(query->lastError().text());
            return 0;
        }

        if(query->first())
            avg = query->value(0).toFloat();
        else {
            ui->lblQueryStatus->setText(query->lastError().text());
            return 0;
        }
    } else if(operation == 2) {
        query->prepare("SELECT ROUND(AVG(p.Mark),1) FROM marks AS p "
                       "INNER JOIN subjects AS s ON p.CodSub = s.ID "
                       "WHERE s.SubName LIKE :subname;");

        query->bindValue(":subname", QString("%%1%").arg(subname));

        if(!query->exec()) {
            ui->lblQueryStatus->setText(query->lastError().text());
            return 0;
        }

        if(query->first())
            avg = query->value(0).toFloat();
        else {
            ui->lblQueryStatus->setText(query->lastError().text());
            return 0;
        }
    }

    // Close the connection to the database
    QString con;
    con = db.connectionName();
    db.close();
    db = QSqlDatabase();
    db.removeDatabase(con);

    return avg;
}

void regMain::searchSubject() {
    // Retrieve requested subject
    this->reqsub = ui->lnSearch->text();
    
    // Connect to the database
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(this->dbPath);
    if(!db.open()) {
        QMessageBox::critical(nullptr, QObject::tr("Cannot open the database!"),
           QObject::tr(db.lastError().text().toLocal8Bit().data()), QMessageBox::Ok); 
        return;
    }

    // Create a new query model(which is read only) and a new query(in the Heap memory)
    QSqlQueryModel *model = new QSqlQueryModel();

    QSqlQuery *query = new QSqlQuery(db);
    // Execute the query
    query->prepare("SELECT m.ID, m.mark, s.SubName, m.MarkDate, m.Description, t.TSurname "
                "FROM marks AS m "
                "INNER JOIN subjects AS s "
                "ON m.CodSub = s.ID "
                "INNER JOIN teachers AS t "
                "ON s.CodTeacher = t.ID "
                "WHERE s.SubName LIKE :subname;");
    query->bindValue(":subname", QString("%%1%").arg(this->reqsub));

    if(!query->exec()) {
        ui->lblQueryStatus->setText(query->lastError().text());
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
    model->setHeaderData(1, Qt::Horizontal, tr("Grade"));
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
    float marks_avg = 0;
    marks_avg = avg(2, this->reqsub);

    if(marks_avg == 0) {
        ui->lblAvg->setText("/");
    } else
        ui->lblAvg->setText(QString::number(marks_avg));

    // Delete heap objects
    delete query;

    // Enable delete button
    ui->btnDelElements->setEnabled(true);
}

void regMain::on_actionChangeDB_triggered() {
    path pt;
    this->dbPath = pt.get_path();
    ui->lblQueryStatus->setText("Database changed, reload it");
}
void regMain::on_actionExportMarks_triggered() {
    // Create an html+css template
    QString *htmlTemplate = new QString();

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

    QSqlQuery *query = new QSqlQuery();

    // Get current year
    QString date = QDate::currentDate().toString("dd/MM/yyyy");

    // Retrieve the mark's count
    if(!query->exec("SELECT COUNT(*) FROM marks;")) {
        ui->lblQueryStatus->setText(query->lastError().text());
        return;
    }

    // Store the result of the query into a variable
    unsigned int marks_count;
    if(query->first())
        marks_count = query->value(0).toInt();
    else {
        ui->lblQueryStatus->setText(query->lastError().text());
        return;
    }

    // Basic html+css template
    *htmlTemplate = 
        "<body><div align='left'>"
            "Number of entries: <b>" + QString::number(marks_count) + "</b>"
        "<div align='right'>"
            "<i>" + date + "</i>"
        "</div><div align='left'>"
            "<b>Summary of school grades<br /></b>"
        "</div>"
        "<table border = 1 padding = 1>"
            "<tr>"
                "<th>Grade</th>"
                "<th>Subject</th>"
                "<th>Date</th>"
                "<th>Description</th>"
                "<th>Teacher</th>"
            "</tr>";

    // Execute the main query
    if(!query->exec("SELECT ROUND(m.mark, 2), s.SubName, m.MarkDate, m.Description, t.TSurname "
                "FROM marks AS m "
                "INNER JOIN subjects AS s "
                "ON m.CodSub = s.ID "
                "INNER JOIN teachers AS t "
                "ON s.CodTeacher = t.ID;")) {
        ui->lblQueryStatus->setText(query->lastError().text());
        return;
    }

    while(query->next()) {
        QString mark, subject, date, description, teacher;

        // Fill vars with query data
        mark = query->value(0).toString();
        subject = query->value(1).toString();
        date = query->value(2).toString();
        description = query->value(3).toString();
        teacher = query->value(4).toString();

        // Update htmlTemplate content
        *htmlTemplate += 
        "<tr>"
            "<td>" + mark + "</td>"
            "<td>" + subject + "</td>"
            "<td>" + date + "</td>"
            "<td>" + description + "</td>"
            "<td>" + teacher + "</td>"
        "</tr>";
    }
    // Close the remaining tags
    *htmlTemplate += "</table></body>";
    
    // Finally, create and save the PDF
    QTextDocument document;
    document.setDefaultStyleSheet("th, td { border: 1px solid black; padding: 5px; }");
    document.setHtml(*htmlTemplate);

    QPrinter pr(QPrinter::PrinterResolution);
    pr.setOutputFormat(QPrinter::PdfFormat);
    pr.setPaperSize(QPrinter::A4);
    pr.setPageMargins(QMarginsF(15, 15, 15, 15));
    // Get a path
    path pt;
    QString path;
    path = pt.set_path_pdf();
    // And set it
    pr.setOutputFileName(path);
    // Finally write the file to the disk
    document.print(&pr);

    ui->lblQueryStatus->setText("PDF successfully created");
    QTimer::singleShot(1500, ui->lblQueryStatus, [&](){ ui->lblQueryStatus->setText(" "); });

    // Close the connection to the database
    QString con;
    con = db.connectionName();
    db.close();
    db = QSqlDatabase();
    db.removeDatabase(con);

    // Delete heap objects
    delete query;
    delete htmlTemplate;
}

void regMain::on_actionExportSubjects_triggered() {
    // Create an html+css template
    QString *htmlTemplate = new QString();

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

    QSqlQuery *query = new QSqlQuery();

    if(!query->exec("SELECT p.SubName, s.TSurname, s.TName FROM subjects AS p "
                    "INNER JOIN teachers AS s ON p.CodTeacher = s.ID;")) {
        ui->lblQueryStatus->setText(query->lastError().text());
        return;
    }

    *htmlTemplate =
        "<body><table border = 1 padding = 1>"
        "<tr>"
            "<th>Subject</th>"
            "<th>Surname</th>"
            "<th>Name</th>"
        "</tr>";
        
    while(query->next()) {
        QString subject, surname, name;

        // Fill vars with query data
        subject = query->value(0).toString();
        surname = query->value(1).toString();
        name = query->value(2).toString();

        // Update htmlTemplate content
        *htmlTemplate += 
        "<tr>"
            "<td>" + subject + "</td>"
            "<td>" + surname + "</td>"
            "<td>" + name + "</td>"
        "</tr>";
    }
    // Close the remaining tags
    *htmlTemplate += "</table></body>";
    
    // Finally, create and save the PDF
    QTextDocument document;
    document.setDefaultStyleSheet("th, td { border: 1px solid black; padding-left: 35px; padding-right: 35px; }");
    document.setHtml(*htmlTemplate);

    QPrinter pr(QPrinter::PrinterResolution);
    pr.setOutputFormat(QPrinter::PdfFormat);
    pr.setPaperSize(QPrinter::A4);
    pr.setPageMargins(QMarginsF(15, 15, 15, 15));
    // Get a path
    path pt;
    QString path;
    path = pt.set_path_pdf();
    // And set it
    pr.setOutputFileName(path);
    // Finally write the file to the disk
    document.print(&pr);

    ui->lblQueryStatus->setText("PDF successfully created");
    QTimer::singleShot(1500, ui->lblQueryStatus, [&](){ ui->lblQueryStatus->setText(" "); });

    // Close the connection to the database
    QString con;
    con = db.connectionName();
    db.close();
    db = QSqlDatabase();
    db.removeDatabase(con);

    // Delete heap objects
    delete query;
    delete htmlTemplate;
}


void regMain::on_actionAbout_triggered() {
    aboutWin = new about();
    aboutWin->show();
    aboutWin->setGeometry(
        QStyle::alignedRect(
            Qt::LeftToRight, 
            Qt::AlignCenter, 
            aboutWin->size(), 
            qApp->primaryScreen()->availableGeometry()
        )
    );
}

void regMain::on_actionExit_triggered() {
    exit(0);
}

regMain::~regMain() { delete ui; }
