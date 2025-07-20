#include "StyleReader.h"

QString StyleLoader::read(const QString &filePath) {
    QFile file(filePath);
    if (file.open(QFile::ReadOnly | QFile::Text)) {
        QTextStream stream(&file);
        QString styleSheet = stream.readAll();
        file.close();
        return styleSheet;
    }
    return QString(); // Return an empty string if the file cannot be read
}
