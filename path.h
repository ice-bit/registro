#ifndef PATH_H
#define PATH_H

#include <QtWidgets/QMainWindow>
#include <QString>
#include <QFileDialog>
#include <QObject>

class path : public QMainWindow {
    Q_OBJECT

public:
    path();
    QString get_path(); // open existing file
    QString set_path(); // Create new file

private:
    QString user_path;

};

#endif