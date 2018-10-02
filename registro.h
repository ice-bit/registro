#ifndef REGISTRO_H
#define REGISTRO_H

#include <QtWidgets/QMainWindow>
#include "ui_registro.h"

class regMain : public QMainWindow {
    
    Q_OBJECT

private:
    Ui::regMainClass *ui;

public:
    regMain(QWidget *parent = Q_NULLPTR);
    ~regMain();
};

#endif

