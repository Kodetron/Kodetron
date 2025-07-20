#include "ThemeRepository.h"
#include "DatabaseManager.h"
#include <QSqlQuery>
#include <QVariant>
#include <QDebug>

bool ThemeRepository::create(Theme& theme)
{
    if (theme.name.isEmpty() || theme.userId < 0) {
        qWarning() << "Cannot create theme with empty name or invalid user ID";
        return false;
    }
    
    QString query = R"(
        INSERT INTO themes (user_id, name, qss_content, created_at, updated_at)
        VALUES (?, ?, ?, ?, ?)
    )";
    
    QVariantList params;
    params << theme.userId << theme.name << theme.qssContent 
           << theme.createdAt << theme.updatedAt;
    
    QSqlQuery sqlQuery = DatabaseManager::instance()->executeQuery(query, params);
    
    if (sqlQuery.lastError().isValid()) {
        qCritical() << "Failed to create theme:" << sqlQuery.lastError().text();
        return false;
    }
    
    theme.id = DatabaseManager::instance()->lastInsertId();
    return theme.id > 0;
}

Theme ThemeRepository::findById(int id)
{
    QString query = R"(
        SELECT id, user_id, name, qss_content, created_at, updated_at
        FROM themes
        WHERE id = ?
    )";
    
    QVariantList params;
    params << id;
    
    QSqlQuery sqlQuery = DatabaseManager::instance()->executeQuery(query, params);
    
    if (sqlQuery.next()) {
        return mapFromQuery(sqlQuery);
    }
    
    return Theme(); // Return invalid theme
}

QList<Theme> ThemeRepository::findByUserId(int userId)
{
    QString query = R"(
        SELECT id, user_id, name, qss_content, created_at, updated_at
        FROM themes
        WHERE user_id = ?
        ORDER BY name
    )";
    
    QVariantList params;
    params << userId;
    
    QSqlQuery sqlQuery = DatabaseManager::instance()->executeQuery(query, params);
    QList<Theme> themes;
    
    while (sqlQuery.next()) {
        themes.append(mapFromQuery(sqlQuery));
    }
    
    return themes;
}

QList<Theme> ThemeRepository::findAll()
{
    QString query = R"(
        SELECT id, user_id, name, qss_content, created_at, updated_at
        FROM themes
        ORDER BY name
    )";
    
    QSqlQuery sqlQuery = DatabaseManager::instance()->executeQuery(query);
    QList<Theme> themes;
    
    while (sqlQuery.next()) {
        themes.append(mapFromQuery(sqlQuery));
    }
    
    return themes;
}

bool ThemeRepository::update(const Theme& theme)
{
    if (!theme.isValid()) {
        qWarning() << "Cannot update invalid theme";
        return false;
    }
    
    QString query = R"(
        UPDATE themes
        SET name = ?, qss_content = ?, updated_at = ?
        WHERE id = ?
    )";
    
    QVariantList params;
    params << theme.name << theme.qssContent << QDateTime::currentDateTime() << theme.id;
    
    QSqlQuery sqlQuery = DatabaseManager::instance()->executeQuery(query, params);
    
    if (sqlQuery.lastError().isValid()) {
        qCritical() << "Failed to update theme:" << sqlQuery.lastError().text();
        return false;
    }
    
    return sqlQuery.numRowsAffected() > 0;
}

bool ThemeRepository::deleteById(int id)
{
    QString query = R"(
        DELETE FROM themes
        WHERE id = ?
    )";
    
    QVariantList params;
    params << id;
    
    QSqlQuery sqlQuery = DatabaseManager::instance()->executeQuery(query, params);
    
    if (sqlQuery.lastError().isValid()) {
        qCritical() << "Failed to delete theme:" << sqlQuery.lastError().text();
        return false;
    }
    
    return sqlQuery.numRowsAffected() > 0;
}

bool ThemeRepository::exists(int id)
{
    QString query = R"(
        SELECT COUNT(*) FROM themes WHERE id = ?
    )";
    
    QVariantList params;
    params << id;
    
    QSqlQuery sqlQuery = DatabaseManager::instance()->executeQuery(query, params);
    
    if (sqlQuery.next()) {
        return sqlQuery.value(0).toInt() > 0;
    }
    
    return false;
}

Theme ThemeRepository::findByUserIdAndName(int userId, const QString& name)
{
    QString query = R"(
        SELECT id, user_id, name, qss_content, created_at, updated_at
        FROM themes
        WHERE user_id = ? AND name = ?
    )";
    
    QVariantList params;
    params << userId << name;
    
    QSqlQuery sqlQuery = DatabaseManager::instance()->executeQuery(query, params);
    
    if (sqlQuery.next()) {
        return mapFromQuery(sqlQuery);
    }
    
    return Theme(); // Return invalid theme
}

int ThemeRepository::countByUserId(int userId)
{
    QString query = R"(
        SELECT COUNT(*) FROM themes WHERE user_id = ?
    )";
    
    QVariantList params;
    params << userId;
    
    QSqlQuery sqlQuery = DatabaseManager::instance()->executeQuery(query, params);
    
    if (sqlQuery.next()) {
        return sqlQuery.value(0).toInt();
    }
    
    return 0;
}

Theme ThemeRepository::mapFromQuery(const QSqlQuery& query)
{
    Theme theme;
    theme.id = query.value("id").toInt();
    theme.userId = query.value("user_id").toInt();
    theme.name = query.value("name").toString();
    theme.qssContent = query.value("qss_content").toString();
    theme.createdAt = query.value("created_at").toDateTime();
    theme.updatedAt = query.value("updated_at").toDateTime();
    
    return theme;
}
