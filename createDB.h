#ifndef CREATEDB_H 
#define CREATEDB_H 

#include <QtWidgets/QMainWindow>
#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <QMessageBox>
#include <QSqlQuery>
#include <QTimer>
#include "path.h"
#include "upTS.h"
#include "delTS.h"
#include "ui_createDB.h"

class createDB : public QMainWindow {
    Q_OBJECT

private:
    Ui::createDBClass *ui;
    upTS *upTSWin;
    delTS *delTSWin;
    QString teacherName, teacherSurname, subject, file = nullptr;

public:
    createDB(QWidget *parent = Q_NULLPTR);
    ~createDB();

private slots:
    void on_btnAddSubject_clicked();
    void on_btnAddTeacher_clicked();

public slots:
    void on_actionRefresh_triggered();
    void on_actionUpdate_triggered();
    void on_actionDelete_triggered();
    void on_actionDBCreate_triggered();
};

#endif