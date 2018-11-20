#ifndef UPTS_H
#define UPTS_H

#include <QtWidgets/QMainWindow>
#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <QMessageBox>
#include <QSqlQuery>
#include <QTimer>
#include "path.h"
#include "path.h"
#include "ui_upTS.h"

class upTS : public QMainWindow {
    Q_OBJECT

private:
    Ui::upTSClass *ui;
    unsigned int id, teachid;
    QString subName, tcName, tcSurname, file = nullptr;

public:
    upTS(QWidget *parent = Q_NULLPTR);
    ~upTS();

private slots:
    void on_btnSubUpdate_clicked();
    void on_btnTeacherUpdate_clicked();

public slots:
    void on_actionRefresh_triggered();
};

#endif