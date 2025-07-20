#include "UserRepository.h"
#include "DatabaseManager.h"
#include <QSqlQuery>
#include <QVariant>
#include <QDebug>

bool UserRepository::create(User& user)
{
    if (user.username.isEmpty()) {
        qWarning() << "Cannot create user with empty username";
        return false;
    }
    
    QString query = R"(
        INSERT INTO users (username, email, created_at)
        VALUES (?, ?, ?)
    )";
    
    QVariantList params;
    params << user.username << user.email << user.createdAt;
    
    QSqlQuery sqlQuery = DatabaseManager::instance()->executeQuery(query, params);
    
    if (sqlQuery.lastError().isValid()) {
        qCritical() << "Failed to create user:" << sqlQuery.lastError().text();
        return false;
    }
    
    user.id = DatabaseManager::instance()->lastInsertId();
    return user.id > 0;
}

User UserRepository::findById(int id)
{
    QString query = R"(
        SELECT id, username, email, created_at
        FROM users
        WHERE id = ?
    )";
    
    QVariantList params;
    params << id;
    
    QSqlQuery sqlQuery = DatabaseManager::instance()->executeQuery(query, params);
    
    if (sqlQuery.next()) {
        return mapFromQuery(sqlQuery);
    }
    
    return User(); // Return invalid user
}

User UserRepository::findByUsername(const QString& username)
{
    QString query = R"(
        SELECT id, username, email, created_at
        FROM users
        WHERE username = ?
    )";
    
    QVariantList params;
    params << username;
    
    QSqlQuery sqlQuery = DatabaseManager::instance()->executeQuery(query, params);
    
    if (sqlQuery.next()) {
        return mapFromQuery(sqlQuery);
    }
    
    return User(); // Return invalid user
}

QList<User> UserRepository::findAll()
{
    QString query = R"(
        SELECT id, username, email, created_at
        FROM users
        ORDER BY username
    )";
    
    QSqlQuery sqlQuery = DatabaseManager::instance()->executeQuery(query);
    QList<User> users;
    
    while (sqlQuery.next()) {
        users.append(mapFromQuery(sqlQuery));
    }
    
    return users;
}

bool UserRepository::update(const User& user)
{
    if (!user.isValid()) {
        qWarning() << "Cannot update invalid user";
        return false;
    }
    
    QString query = R"(
        UPDATE users
        SET username = ?, email = ?
        WHERE id = ?
    )";
    
    QVariantList params;
    params << user.username << user.email << user.id;
    
    QSqlQuery sqlQuery = DatabaseManager::instance()->executeQuery(query, params);
    
    if (sqlQuery.lastError().isValid()) {
        qCritical() << "Failed to update user:" << sqlQuery.lastError().text();
        return false;
    }
    
    return sqlQuery.numRowsAffected() > 0;
}

bool UserRepository::deleteById(int id)
{
    QString query = R"(
        DELETE FROM users
        WHERE id = ?
    )";
    
    QVariantList params;
    params << id;
    
    QSqlQuery sqlQuery = DatabaseManager::instance()->executeQuery(query, params);
    
    if (sqlQuery.lastError().isValid()) {
        qCritical() << "Failed to delete user:" << sqlQuery.lastError().text();
        return false;
    }
    
    return sqlQuery.numRowsAffected() > 0;
}

bool UserRepository::exists(int id)
{
    QString query = R"(
        SELECT COUNT(*) FROM users WHERE id = ?
    )";
    
    QVariantList params;
    params << id;
    
    QSqlQuery sqlQuery = DatabaseManager::instance()->executeQuery(query, params);
    
    if (sqlQuery.next()) {
        return sqlQuery.value(0).toInt() > 0;
    }
    
    return false;
}

bool UserRepository::existsByUsername(const QString& username)
{
    QString query = R"(
        SELECT COUNT(*) FROM users WHERE username = ?
    )";
    
    QVariantList params;
    params << username;
    
    QSqlQuery sqlQuery = DatabaseManager::instance()->executeQuery(query, params);
    
    if (sqlQuery.next()) {
        return sqlQuery.value(0).toInt() > 0;
    }
    
    return false;
}

int UserRepository::count()
{
    QString query = R"(
        SELECT COUNT(*) FROM users
    )";
    
    QSqlQuery sqlQuery = DatabaseManager::instance()->executeQuery(query);
    
    if (sqlQuery.next()) {
        return sqlQuery.value(0).toInt();
    }
    
    return 0;
}

User UserRepository::mapFromQuery(const QSqlQuery& query)
{
    User user;
    user.id = query.value("id").toInt();
    user.username = query.value("username").toString();
    user.email = query.value("email").toString();
    user.createdAt = query.value("created_at").toDateTime();
    
    return user;
}
