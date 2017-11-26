#include "registro_main_gui.h"
#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	registro_main_gui w;
	w.show();

	return a.exec();
}
