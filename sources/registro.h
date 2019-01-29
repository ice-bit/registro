#ifndef REGISTRO_H
#define REGISTRO_H

#include <QtWidgets/QMainWindow>
#include <QDesktopWidget>
#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <QPrinter>
#include <QTextDocument>
#include <vector>
#include <QSqlError>
#include <math.h>
#include "path.h"
#include "createDB.h"
#include "addMK.h"
#include "upMK.h"
#include "about.h"
#include "ui_registro.h"

class regMain : public QMainWindow {
    Q_OBJECT

public:
    regMain(QWidget *parent = Q_NULLPTR);
    ~regMain();

public slots:
    void on_actionCreateDB_triggered();
    void on_actionAbout_triggered();
    void on_actionExit_triggered();
    void on_actionChangeDB_triggered();
    void on_actionExportMarks_triggered();
    void on_actionExportSubjects_triggered();
    void searchSubject();

private slots:
    void on_btnLoadElements_clicked();
    void on_btnAddElements_clicked();
    void on_btnUpElements_clicked();
    void on_btnDelElements_clicked();

private:
    Ui::regMainClass *ui;
    createDB *createDBWin;
    addMK *addMKWin;
    upMK *upMKWin;
    about *aboutWin;
    unsigned int userSelection;
    QString reqsub, dbPath = nullptr;
    float avg(int operation, QString subname);
};

#endif