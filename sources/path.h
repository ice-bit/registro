#ifndef PATH_H
#define PATH_H

#include <QtWidgets/QMainWindow>
#include <QString>
#include <QFileDialog>
#include <QObject>
#include "../../config.h" // Needed for OS recognition

class path : public QMainWindow {
    Q_OBJECT

public:
    path();
    QString get_path(); // open existing db
    QString set_path(); // create a new db
    QString set_path_pdf(); // create a new pdf

private:
    QString user_path;
};

#endif
