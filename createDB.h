#ifndef CREATEDB_H 
#define CREATEDB_H 

#include <QtWidgets/QMainWindow>
#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <QMessageBox>
#include <QSqlQuery>
#include <QTimer>
#include "ui_createDB.h"

class createDB : public QMainWindow {
    Q_OBJECT

private:
    Ui::createDBClass *ui;
    QString teacherName, teacherSurname, subject;

private slots:
    void on_btnAddSubject_clicked();
    void on_btnAddTeacher_clicked();

public slots:
    void on_actionRefresh_triggered();

public:
    createDB(QWidget *parent = Q_NULLPTR);
    ~createDB();
};

#endif