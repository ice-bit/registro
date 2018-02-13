#include "registro_main_gui.h"
#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	registro_main_gui w;
	std::cout << "Starting Registro 1.2..." << std::endl;
	w.show();
	std::cout << "Registro 1.2 has been closed!" << std::endl;
	return a.exec();
}
