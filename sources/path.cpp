#include "path.h"

path::path() { this->user_path = nullptr; }

QString path::get_path() {
    QFileDialog dialog(this);
    dialog.setDefaultSuffix(".db");
    this->user_path = dialog.getOpenFileName(this,
        tr("Open an existing database"), "",
        tr("Database files (*.db)"));
    return this->user_path;
}

QString path::set_path() {
    QFileDialog dialog(this);
    dialog.setDefaultSuffix(".db");
    this->user_path = dialog.getSaveFileName(this,
        tr("Save an new database"), "",
        tr("Database files (*.db)"));
/*     #if OS == 0 // If platform is Linux add file extension manually
        this->user_path += ".db";
    #endif */
    return this->user_path;
}

QString path::set_path_pdf() {
    QFileDialog dialog(this);
    this->user_path = dialog.getSaveFileName(this,
        tr("Save a new pdf"), "",
        tr("PDF File (*.pdf)"));
/*     #if OS == 0 // If platform is Linux add file extension manually
        this->user_path += ".pdf";
    #endif */
    return this->user_path;
}