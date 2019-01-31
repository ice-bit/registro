#include "path.h"

path::path() { this->user_path = nullptr; }

QString path::get_path() {
    this->user_path = QFileDialog::getOpenFileName(this,
        tr("Open an existing database"), "",
        tr("Database files (*.db)"));
    return this->user_path;
}

QString path::set_path() {
    this->user_path = QFileDialog::getSaveFileName(this,
        tr("Save a new database"), "",
        tr("Database file (*.db)"));
    #if OS == 0 // If the platform is an UNIX-like system we may need to add
        user_path += ".db"; // the extension manually
    #endif
    return this->user_path;
}

QString path::set_path_pdf() {
    this->user_path = QFileDialog::getSaveFileName(this,
        tr("Save a new pdf"), "",
        tr("PDF File (*.pdf)"));
    #if OS == 0 // If the platform is an UNIX-like system we may need to add
        user_path += ".pdf"; // the extension manually
    #endif
    return this->user_path;
}