#include "FileDialog.h"

namespace FileDialog {
    QString getOpenFilePath(QWidget *parent) {
        return QFileDialog::getOpenFileName(parent, "Open file", QString(), "All Files (*)");
    }

    QString getOpenDirectoryPath(QWidget *parent) {
        return QFileDialog::getExistingDirectory(parent, "Open folder", QString());
    }
}
