#pragma once
#define OS 2 //1 : UNIX; 2 : Windows
#define BUILD_INFO " Built with QT5 and sqlite3"
#define PLATFORM " Windows"
#define AUTHOR " © 2016-2018 Marco Cetica"
#define VERSION " 1.2"
/*MIT License

Copyright(c) 2016 - 2018 Marco 'icebit' Cetica

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
#include <QComboBox>
#include <string>
#include <stdlib.h>

class connection {
protected:
	int init_db();
	std::string define_path();
	static int appendSub(void *qCbnAppend, int argc, char **argv, char **azColName);
public:
	//db variables
	sqlite3 *db;
	int rc;
	char *ErrMsg = 0;
	sqlite3_stmt *res;
	int step;
	std::string sqlAppend = "SELECT NAME FROM SUBJECTS";
};