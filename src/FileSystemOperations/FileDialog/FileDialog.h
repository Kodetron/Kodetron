#ifndef FILEDIALOG_H
#define FILEDIALOG_H

#include <QString>
#include <QFileDialog>

namespace FileDialog {
    QString getOpenFilePath(QWidget *parent = nullptr);
    QString getOpenDirectoryPath(QWidget *parent = nullptr);
    QString getOpenCppFilePath(QWidget *parent = nullptr);
    QString readFileContents(const QString& filePath);
    bool writeFileContents(const QString& filePath, const QString& contents);
}

#endif // FILEDIALOG_H
