#include "about.h"

about::about(QWidget *parent) : QMainWindow(parent), ui(new Ui::aboutWindow) {
        ui->setupUi(this);
        setFixedSize(300, 318);
        QString old_value;

        old_value = ui->lblPr->text().toLocal8Bit().constData();
        ui->lblPr->setText(old_value+QString::fromStdString(PRODUCT_INFO));
        old_value = ui->lblOs->text().toLocal8Bit().constData();
        ui->lblOs->setText(old_value+QString::fromStdString(PLATFORM));
        old_value = ui->lblAuthor->text().toLocal8Bit().constData();
        ui->lblAuthor->setText(old_value+QString::fromStdString(AUTHOR));
        old_value = ui->lblVersion->text().toLocal8Bit().constData();
        ui->lblVersion->setText(old_value+QString::fromStdString(VERSION));
}
about::~about() {
        delete ui;
}
