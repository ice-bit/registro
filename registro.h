#ifndef REGISTRO_H
#define REGISTRO_H

#include <QtWidgets/QMainWindow>
#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <QDebug>
#include "createDB.h"
#include "addMK.h"
#include "upMK.h"
#include "ui_registro.h"

class regMain : public QMainWindow {
    Q_OBJECT

private:
    Ui::regMainClass *ui;
    createDB *createDBWin;
    addMK *addMKWin;
    upMK *upMKWin;
    unsigned int userSelection;

public:
    regMain(QWidget *parent = Q_NULLPTR);
    ~regMain();

public slots:
    void on_actionCreateDB_triggered();

private slots:
    void on_btnLoadElements_clicked();
    void on_btnAddElements_clicked();
    void on_btnUpElements_clicked();
    void on_btnDelElements_clicked();
};

#endif