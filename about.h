#ifndef ABOUT_H
#define ABOUT_H

#define VERSION "Version: 1.3Beta"
#define AUTHOR "Copyright: 2018 (c) Marco 'icebit' Cetica"
#define LICENSE "License: GNU General Public License v3.0"

#include <QtWidgets/QMainWindow>
#include <QDebug>
#include "ui_about.h"


class about : public QMainWindow {
    
private:
    Ui::aboutClass *ui;

public:
    about(QWidget *parent = Q_NULLPTR);
    ~about();
};

#endif