#ifndef STYLE_LOADER_H
#define STYLE_LOADER_H

#include <QString>
#include <QFile>
#include <QTextStream>

class StyleLoader {
  public:
    static QString read(const QString &filePath);
};

#endif // STYLE_LOADER_H
