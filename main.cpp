#include <QApplication>
#include <QDesktopWidget>
#include "registro.h"

int main(int argc, char **argv) {
    QApplication app(argc, argv);
    regMain win;
    win.show();
    win.setGeometry( // Set the default window's location at the center of the screen
        QStyle::alignedRect(
            Qt::LeftToRight,
            Qt::AlignCenter,
            win.size(),
            qApp->desktop()->availableGeometry()
        )
    );

    return app.exec();
}

