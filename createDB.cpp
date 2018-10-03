#include "createDB.h"


createDB::createDB(QWidget *parent) : QMainWindow(parent), ui(new Ui::createDBClass) {
    ui->setupUi(this);
    setFixedSize(639, 189);
}

createDB::~createDB() { delete ui; }