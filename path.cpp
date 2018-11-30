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
    return this->user_path;
}

QString path::set_path_pdf() {
    this->user_path = QFileDialog::getSaveFileName(this,
        tr("Save a new pdf"), "",
        tr("PDF File (*.pdf"));
    return this->user_path;
}