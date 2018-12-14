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
    upMK(QWidget *parent = Q_NULLPTR);
    ~upMK();

public slots:
    void on_actionRefresh_triggered();

private slots:
    void on_btnUpdateMark_clicked();

private:
    Ui::upMKClass *ui;
    float mkMark, mkID;
    QString mkDate, mkDesc, mkSub, file = nullptr;
};

#endif