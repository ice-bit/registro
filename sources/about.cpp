#include "about.h"

about::about(QWidget *parent) : QMainWindow(parent), ui(new Ui::aboutClass) {
    ui->setupUi(this);
    setFixedSize(582, 455);

    // Get label values
    QString version = ui->lblVersion->text();
    QString os = ui->lblOS->text();
    QString author = ui->lblAuthor->text();
    QString build = ui->lblBuild->text();

    // Fill about labels
    ui->lblVersion->setText(version + " " + PROJECT_VERSION);
    ui->lblOS->setText(os + PROJECT_OS + " " + PROJECT_ARCH);
    ui->lblAuthor->setText(author + PROJECT_AUTHOR);
    
    #if OS == 0
        ui->lblBuild->setText(build + PROJECT_BUILD_DATE + " " + PROJECT_BUILD_TIME); 
    #elif OS == 1
        ui->lblBuild->setText(build + "Not supported for this platform");
    #endif

    // Free QObjects(needed to avoid memory leaks)
    setAttribute(Qt::WA_DeleteOnClose);
}

about::~about() { delete ui; }