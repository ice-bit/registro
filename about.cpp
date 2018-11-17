#include "about.h"

about::about(QWidget *parent) : QMainWindow(parent), ui(new Ui::aboutClass) {
    ui->setupUi(this);
    setFixedSize(413, 140);

    // Fill about labels
    ui->lblVersion->setText(VERSION);
    ui->lblAuthor->setText(AUTHOR);
    ui->lblLicense->setText(LICENSE);

    // Free QObjects(needed to avoid memory leaks)
    setAttribute(Qt::WA_DeleteOnClose);
}

about::~about() { delete ui; }