#ifndef ADDMK_H
#define ADDMK_H

#include <QtWidgets/QMainWindow>
#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QTimer>
#include "path.h"
#include "ui_addMK.h"

class addMK : public QMainWindow {
    Q_OBJECT

public:
    addMK(QWidget *parent = Q_NULLPTR);
    ~addMK();

public slots:
    void on_actionRefresh_triggered();

private slots:
    void on_btnInsertMark_clicked();

private:
    Ui::addMKClass *ui;
    float mkMark;
    QString mkDate, mkDesc, mkSub, file = nullptr;

};

#endif