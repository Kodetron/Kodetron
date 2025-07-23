#include "FileDialog.h"

namespace FileDialog {
    QString getOpenFilePath(QWidget *parent) {
        return QFileDialog::getOpenFileName(parent, "Open file", QString(), "All Files (*)");
    }

    QString getOpenDirectoryPath(QWidget *parent) {
        return QFileDialog::getExistingDirectory(parent, "Open folder", QString());
    }

    QString getOpenCppFilePath(QWidget *parent) {
        return QFileDialog::getOpenFileName(parent, "Open C++ file", QString(), "C++ Files (*.cpp)");
    }
    QString readFileContents(const QString& filePath) {
        QFile file(filePath);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            return QString();
        }
        QTextStream in(&file);
        QString contents = in.readAll();
        file.close();
        return contents;
    }

    bool writeFileContents(const QString& filePath, const QString& contents) {
        QFile file(filePath);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            return false;
        }
        QTextStream out(&file);
        out << contents;
        file.close();
        return true;
    }
}
