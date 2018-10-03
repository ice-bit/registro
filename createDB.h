#ifndef CREATEDB_H 
#define CREATEDB_H 

#include <QtWidgets/QMainWindow>
#include "ui_createDB.h"

class createDB : public QMainWindow {
    Q_OBJECT

private:
    Ui::createDBClass *ui;

public:
    createDB(QWidget *parent = Q_NULLPTR);
    ~createDB();
};

#endif