#ifndef FILEDIALOG_H
#define FILEDIALOG_H

#include <QString>
#include <QFileDialog>

namespace FileDialog {
    QString getOpenFilePath(QWidget *parent = nullptr);
    QString getOpenDirectoryPath(QWidget *parent = nullptr);
}

#endif // FILEDIALOG_H
