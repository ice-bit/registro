#ifndef REGISTRO_H
#define REGISTRO_H

#include <QtWidgets/QMainWindow>
#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <vector>
#include <math.h>
#include <QSqlError>
#include "path.h"
#include "createDB.h"
#include "addMK.h"
#include "upMK.h"
#include "about.h"
#include "ui_registro.h"


class regMain : public QMainWindow {
    Q_OBJECT

private:
    Ui::regMainClass *ui;
    createDB *createDBWin;
    addMK *addMKWin;
    upMK *upMKWin;
    about *aboutWin;
    unsigned int userSelection;
    std::vector<float> marks;
    QString reqsub, file = nullptr;
    float avg(std::vector<float> marks);

public:
    regMain(QWidget *parent = Q_NULLPTR);
    ~regMain();

public slots:
    void on_actionCreateDB_triggered();
    void on_actionAbout_triggered();
    void on_actionExit_triggered();
    void on_actionChangeDB_triggered();
    void searchSubject();

private slots:
    void on_btnLoadElements_clicked();
    void on_btnAddElements_clicked();
    void on_btnUpElements_clicked();
    void on_btnDelElements_clicked();
};

#endif