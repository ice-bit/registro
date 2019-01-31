#ifndef UPTS_H
#define UPTS_H

#include <QtWidgets/QMainWindow>
#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QTimer>
#include "path.h"
#include "ui_upTS.h"

class upTS : public QMainWindow {
    Q_OBJECT

public:
    upTS(QString dbPath, QWidget *parent = Q_NULLPTR);
    ~upTS();

private slots:
    void on_btnSubUpdate_clicked();
    void on_btnTeacherUpdate_clicked();

private:
    void loadTeachers();
    Ui::upTSClass *ui;
    unsigned int id, teachid;
    QString subName, tcName, tcSurname, dbPath = nullptr;
};

#endif