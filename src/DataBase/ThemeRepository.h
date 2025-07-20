#ifndef THEMEREPOSITORY_H
#define THEMEREPOSITORY_H

#include "Models.h"
#include <QList>

class ThemeRepository
{
public:
    ThemeRepository() = default;
    
    // CRUD operations
    bool create(Theme& theme);
    Theme findById(int id);  // Returns invalid theme if not found
    QList<Theme> findByUserId(int userId);
    QList<Theme> findAll();
    bool update(const Theme& theme);
    bool deleteById(int id);
    
    // Utility methods
    bool exists(int id);
    Theme findByUserIdAndName(int userId, const QString& name);  // Returns invalid theme if not found
    int countByUserId(int userId);
    
private:
    Theme mapFromQuery(const class QSqlQuery& query);
};

#endif // THEMEREPOSITORY_H
