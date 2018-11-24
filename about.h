#ifndef ABOUT_H
#define ABOUT_H

#define VERSION "Version: 1.3Beta"
#define AUTHOR "Copyright: 2018 (c) Marco 'icebit' Cetica"
#define LICENSE "License: GNU General Public License v3.0"

#include <QtWidgets/QMainWindow>
#include "ui_about.h"


class about : public QMainWindow {
    
public:
    about(QWidget *parent = Q_NULLPTR);
    ~about();

private:
    Ui::aboutClass *ui;

};

#endif