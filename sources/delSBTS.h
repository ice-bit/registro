#ifndef DELSBTS_H
#define DELSBTS_H

#include <QtWidgets/QMainWindow>
#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QTimer>
#include "path.h"
#include "ui_delSBTS.h"

class delSBTS : public QMainWindow {
    Q_OBJECT

public:
    delSBTS(QString dbPath, QWidget *parent = Q_NULLPTR);
    ~delSBTS();

private slots:
    void on_btnDeleteSub_clicked();
    void on_btnDeleteTeach_clicked();

private:
    void loadTeachersSubjects();
    QString dbPath = nullptr;
    Ui::delSBTSClass *ui;
    unsigned int userSelection;
};

#endif