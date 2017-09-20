#pragma once
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
#include <QtWidgets\QMainWindow>
#include "ui_addrecord.h"
#include "db_connection.h"

class addrecord : public QMainWindow, public connection {
	Q_OBJECT
public:
	addrecord(QWidget *parent = Q_NULLPTR);
	~addrecord();
private slots:
	void on_btnSaveVote_clicked();
	void on_btnDelRecord_clicked();
private:
	Ui::addrecord *ui;

	void get_all_elements(int op); //If operation is equal to 0 then set all the 'add' variables 
								   //else if operation is equal to 1 then set only the 'del' variable.
	const char * sqlQuery;
	int vote;
	std::string subject;
	std::string date;
	std::string description;
	std::string delid;
};