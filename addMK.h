#ifndef ADDMK_H
#define ADDMK_H

#include <QtWidgets/QMainWindow>
#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <QMessageBox>
#include <QSqlQuery>
#include <QTimer>
#include "ui_addMK.h"

class addMK : public QMainWindow {

private:
    Ui::addMKClass *ui;
    float mkMark;
    QString mkDate, mkDesc, mkSub;

public:
    addMK(QWidget *parent = Q_NULLPTR);
    ~addMK();

public slots:
    void on_actionRefresh_triggered();

private slots:
    void on_btnInsertMark_clicked();
};

#endif