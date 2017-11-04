#ifndef DB_CONNECTION_H
#define DB_CONNECTION_H
#define OS 1 //1 : UNIX; 2 : Windows
#define PRODUCT_INFO " - Build with QT5 and SQLITE3" //Change these directives
#define PLATFORM " - GNU/Linux" //with
#define AUTHOR " - Marco 'icebit' Cetica" //correct
#define VERSION " - 1.0" //values
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
#include <iostream>
#include <sqlite3.h>
#include <QMessageBox>
#include <string>
#include <stdlib.h>

class connection {
protected:
    int init_db();
    std::string define_path();
public:
    //db variables
    sqlite3 *db;
    int rc;
    char *ErrMsg = 0;
    sqlite3_stmt *res;
    int step;
};
#endif // DB_CONNECTION_H
