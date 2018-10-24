#ifndef REGISTRO_H
#define REGISTRO_H

#include <QtWidgets/QMainWindow>
#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <QSqlRelationalTableModel>
#include <QDebug>
#include "ui_registro.h"
#include "createDB.h"

class regMain : public QMainWindow {
    
    Q_OBJECT

private:
    Ui::regMainClass *ui;
    createDB *createDBWin;

public:
    regMain(QWidget *parent = Q_NULLPTR);
    ~regMain();

public slots:
    void on_actionCreateDB_triggered();

private slots:
    void on_btnLoadElements_clicked();
};

#endif