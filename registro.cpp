#include "registro.h"

regMain::regMain(QWidget *parent) : QMainWindow(parent), ui(new Ui::regMainClass) {
    ui->setupUi(this);
}

regMain::~regMain() { delete ui; }

