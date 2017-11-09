#ifndef REGISTRO_MAIN_GUI_H
#define REGISTRO_MAIN_GUI_H
/*MIT License

Copyright(c) 2016 - 2017 Marco 'icebit' Cetica

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files(the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions :

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.*/
#include <QtWidgets/QMainWindow>
#include "ui_registro_main_gui.h"
#include "adddb.h"
#include "addrecord.h"
#include "editrecord.h"
#include "about.h"
#include "db_connection.h"

class registro_main_gui : public QMainWindow, public connection
{
    Q_OBJECT
public:
    registro_main_gui(QWidget *parent = Q_NULLPTR);
    ~registro_main_gui();
public slots:
    void on_ActionAddDB_triggered();
    void on_ActionAbout_triggered();
    void on_ActionExit_triggered();
private slots:
    void on_btnNewElement_clicked();
    void on_btnEditRecord_clicked();
    void on_btnLoadElements_clicked();
    void on_btnSearchElements_clicked();
    void on_btnLoadBadElements_clicked();
private:
    Ui::registro_main_guiClass *ui;
    adddb *addDB;
    addrecord *addRC;
    editrecord *editRC;
    about *aboutWin;
    const char * sqlQuery = "SELECT * FROM REGISTRO";
    std::string searchType; //Get type from QComboBox
    std::string sqlQuerySearch; //needed for searchElement function
    std::string search;//
    static int addLoadResult(void *qTextAppend, int argc, char **argv, char **azColName);
    static int addBadResult(void *qTextAppend, int argc,  char **argv, char **azColName);
    void setHeader(int type);
};
#endif // REGISTRO_MAIN_GUI_H
