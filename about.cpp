#include "about.h"

about::about(QWidget *parent) : QMainWindow(parent), ui(new Ui::aboutClass) {
    ui->setupUi(this);
    setFixedSize(413, 159);

    // Get label values
    QString version = ui->lblVersion->text();
    QString author = ui->lblAuthor->text();
    QString license = ui->lblLicense->text();
    QString build = ui->lblBuild->text();

    // Fill about labels
    ui->lblVersion->setText(version + PROJECT_VERSION + " " + PROJECT_OS + " " + PROJECT_ARCH);
    ui->lblAuthor->setText(author + PROJECT_AUTHOR);
    ui->lblLicense->setText(license + PROJECT_LICENSE);
    ui->lblBuild->setText(build + PROJECT_BUILD_DATE + " " + PROJECT_BUILD_TIME); 

    // Free QObjects(needed to avoid memory leaks)
    setAttribute(Qt::WA_DeleteOnClose);
}

about::~about() { delete ui; }