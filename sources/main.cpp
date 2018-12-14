#include <QApplication>
#include <QDesktopWidget>
#include <iostream>
#include <string>
#include "config.h"
#include "registro.h"

void version();
void helper();

int main(int argc, char **argv) {
    std::string arg;

    if(argc > 1)
        arg = argv[1];

    if(arg == "-v" || arg == "--version") {
        version();
        return 0;
    } else if(arg == "-h" || arg == "--help") {
        helper();
        return 0;
    }

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

void version() {
    #if OS == 0
        std::cout << "Registro " << PROJECT_VERSION << " " << PROJECT_OS << " " << PROJECT_ARCH << " ("
                << PROJECT_SOURCE_VERSION << ", " << 
                    PROJECT_BUILD_DATE << ", " <<
                    PROJECT_BUILD_TIME << ")" << std::endl;
    #elif OS == 1
        std::cout << "Registro " << PROJECT_VERSION << " " << PROJECT_OS << " " << PROJECT_ARCH << " ("
                << PROJECT_SOURCE_VERSION << ")" << std::endl;
    #endif
}

void helper() {
    std::cout << "Registro " << PROJECT_VERSION << " " << PROJECT_ARCH << "\n"
              << "-v, --version | Print version informations\n" 
              << "-h, --helper  | Show this helper" << std::endl;
}