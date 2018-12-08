#ifndef ABOUT_H
#define ABOUT_H

#include <QtWidgets/QMainWindow>
#include "config.h"
#include "ui_about.h"


class about : public QMainWindow {
    
public:
    about(QWidget *parent = Q_NULLPTR);
    ~about();

private:
    Ui::aboutClass *ui;

};

#endif