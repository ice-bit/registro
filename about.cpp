#include "about.h"

about::about(QWidget *parent) : QMainWindow(parent), ui(new Ui::aboutWindow) {
	ui->setupUi(this);
	setFixedSize(557, 334);
}
about::~about() {
	delete ui;
}