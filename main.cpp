#include <QApplication>
#include "registro.h"

int main(int argc, char **argv) {
    QApplication app(argc, argv);
    regMain win;
    win.show();

    return app.exec();
}

