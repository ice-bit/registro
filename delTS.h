#ifndef DELTS_H
#define DELTS_H

#include <QtWidgets/QMainWindow>
#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <QMessageBox>
#include <QSqlQuery>
#include <QTimer>
#include "path.h"
#include "ui_delTS.h"

class delTS : public QMainWindow {
    Q_OBJECT

private:
    QString file = nullptr;
    Ui::delTSClass *ui;
    unsigned int subid, teachid;

public:
    delTS(QWidget *parent = Q_NULLPTR);
    ~delTS();

private slots:
    void on_btnDeleteSub_clicked();
    void on_btnDeleteTeach_clicked();

public slots:
    void on_actionRefresh_triggered();
};

#endif