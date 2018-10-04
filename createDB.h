#ifndef CREATEDB_H 
#define CREATEDB_H 

#include <QtWidgets/QMainWindow>
#include <QSqlDatabase>
#include <QMessageBox>
#include <QSqlQuery>
#include <QDebug>
#include "ui_createDB.h"

class createDB : public QMainWindow {
    Q_OBJECT

private:
    Ui::createDBClass *ui;
    QString teacher, subject;

private slots:
    void on_btnAddSubject_clicked();

public:
    createDB(QWidget *parent = Q_NULLPTR);
    ~createDB();
};

#endif