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
    createDBWin->setGeometry(
        QStyle::alignedRect(
            Qt::LeftToRight, 
            Qt::AlignCenter, 
            createDBWin->size(), 
            qApp->desktop()->availableGeometry()
        )
    );
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
           QObject::tr(db.lastError().text().toLocal8Bit().data()), QMessageBox::Ok); 
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
    float marks_avg = 0;
    marks_avg = avg(1, nullptr);

    if(marks_avg == 0) {
        ui->lblAvg->setText("/");
    } else
        ui->lblAvg->setText(QString::number(marks_avg));
        
    // Delete heap objects
    delete query;

    // Enable delete button and search field
    ui->btnDelElements->setEnabled(true);
    ui->lnSearch->setEnabled(true);
}

void regMain::on_btnAddElements_clicked() {
    addMKWin = new addMK();
    addMKWin->show();
    addMKWin->setGeometry(
        QStyle::alignedRect(
            Qt::LeftToRight, 
            Qt::AlignCenter, 
            addMKWin->size(), 
            qApp->desktop()->availableGeometry()
        )
    );
}

void regMain::on_btnUpElements_clicked() {
    upMKWin = new upMK;
    upMKWin->show();
    upMKWin->setGeometry(
        QStyle::alignedRect(
            Qt::LeftToRight, 
            Qt::AlignCenter, 
            upMKWin->size(), 
            qApp->desktop()->availableGeometry()
        )
    );
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
           QObject::tr(db.lastError().text().toLocal8Bit().data()), QMessageBox::Ok); 
        return;
    }

    // Our query
    QSqlQuery query;

    // Prepare and execute the delete query
    query.prepare("DELETE FROM mark WHERE ID = :id;");
    query.bindValue(":id", this->userSelection);

    // Error Handling
    if(!query.exec()) {
        ui->lblQueryStatus->setText(query.lastError().text());
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


float regMain::avg(int operation, QString subname) {
    float avg;
    // Connect to the database
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(this->file);
    if(!db.open()) {
        QMessageBox::critical(nullptr, QObject::tr("Cannot open the database!"),
           QObject::tr(db.lastError().text().toLocal8Bit().data()), QMessageBox::Ok); 
        return 0;
    }

    QSqlQueryModel *modle = new QSqlQueryModel();
    QSqlQuery *query = new QSqlQuery(db);
    if(operation == 1) {
        if(!query->exec("SELECT ROUND(AVG(Mark),1) FROM mark;")) {
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
        query->prepare("SELECT ROUND(AVG(p.Mark),1) FROM mark AS p "
                       "INNER JOIN subject AS s ON p.CodSub = s.ID "
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
    db.setDatabaseName(this->file);
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
                "FROM mark AS m "
                "INNER JOIN subject AS s "
                "ON m.CodSub = s.ID "
                "INNER JOIN teacher AS t "
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
    this->file = pt.get_path();
    ui->lblQueryStatus->setText("Database changed, reload it");
}
void regMain::on_actionExportMarks_triggered() {
    // Create an html+css template
    QString *htmlTemplate = new QString();

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
           QObject::tr(db.lastError().text().toLocal8Bit().data()), QMessageBox::Ok); 
        return;
    }

    QSqlQuery *query = new QSqlQuery();

    if(!query->exec("SELECT ROUND(m.mark, 2), s.SubName, m.MarkDate, m.Description, t.TSurname "
                "FROM mark AS m "
                "INNER JOIN subject AS s "
                "ON m.CodSub = s.ID "
                "INNER JOIN teacher AS t "
                "ON s.CodTeacher = t.ID;")) {
        ui->lblQueryStatus->setText(query->lastError().text());
        return;
    }
     // Get current year
    QString date = QDate::currentDate().toString("dd/MM/yyyy");

    // Basic html+css template
    *htmlTemplate = 
        "<body><div align='right'>"
            "<i>" + date + "</i>"
        "</div><div align='left'>"
            "<b>Summary of school marks<br /></b>"
        "</div>"
        "<table border = 1 padding = 1>"
            "<tr>"
                "<th>Mark</th>"
                "<th>Subject</th>"
                "<th>Date</th>"
                "<th>Description</th>"
                "<th>Teacher</th>"
            "</tr>";

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

void regMain::on_actionExportTeachers_triggered() {
    // Create an html+css template
    QString *htmlTemplate = new QString();

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
           QObject::tr(db.lastError().text().toLocal8Bit().data()), QMessageBox::Ok); 
        return;
    }

    QSqlQuery *query = new QSqlQuery();

    if(!query->exec("SELECT p.TName, p.TSurname, s.SubName FROM teacher AS p "
                   "INNER JOIN subject AS s ON p.ID = s.CodTeacher;")) {
        ui->lblQueryStatus->setText(query->lastError().text());
        return;
    }

    *htmlTemplate =
        "<body><table border = 1 padding = 1>"
        "<tr>"
            "<th>Name</th>"
            "<th>Surname</th>"
            "<th>Subject</th>"
        "</tr>";
        
    while(query->next()) {
        QString name, surname, subject;

        // Fill vars with query data
        name = query->value(0).toString();
        surname = query->value(1).toString();
        subject = query->value(2).toString();

        // Update htmlTemplate content
        *htmlTemplate += 
        "<tr>"
            "<td>" + name + "</td>"
            "<td>" + surname + "</td>"
            "<td>" + subject + "</td>"
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
            qApp->desktop()->availableGeometry()
        )
    );
}

void regMain::on_actionExit_triggered() {
    exit(0);
}

regMain::~regMain() { delete ui; }