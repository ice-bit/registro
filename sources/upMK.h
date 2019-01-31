#ifndef UPMK_H
#define UPMK_H

#include <QtWidgets/QMainWindow>
#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QTimer>
#include "path.h"
#include "addMK.h"
#include "ui_upMK.h"

class upMK : public QMainWindow {
    Q_OBJECT

public:
    upMK(QString dbPath, QWidget *parent = Q_NULLPTR);
    ~upMK();

private slots:
    void on_btnUpdateMark_clicked();

private:
    void loadSubject();
    Ui::upMKClass *ui;
    float mkMark, mkID;
    QString mkDate, mkDesc, mkSub, dbPath = nullptr;
};

#endif