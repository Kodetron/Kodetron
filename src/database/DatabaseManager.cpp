#include "DatabaseManager.h"
#include <QSqlRecord>
#include <QSqlField>
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QStandardPaths>

DatabaseManager::DatabaseManager(QObject *parent)
    : QObject(parent)
    , m_initialized(false)
{
}

DatabaseManager::~DatabaseManager()
{
    if (m_database.isOpen()) {
        m_database.close();
    }
}

bool DatabaseManager::initialize(const QString &dbPath)
{
    // Add SQLite driver
    if (!QSqlDatabase::isDriverAvailable("QSQLITE")) {
        setLastError("SQLite driver not available");
        return false;
    }

    // Create database connection
    m_database = QSqlDatabase::addDatabase("QSQLITE", "kodetron_connection");
    m_database.setDatabaseName(dbPath);

    if (!m_database.open()) {
        setLastError("Failed to open database: " + m_database.lastError().text());
        return false;
    }

    // Create tables
    if (!createTables()) {
        return false;
    }

    m_initialized = true;
    return true;
}

bool DatabaseManager::createTables()
{
    // Read and execute schema file
    QString schemaPath = "database/schema.sql";
    if (!executeFile(schemaPath)) {
        // Try relative path from build directory
        schemaPath = "../database/schema.sql";
        if (!executeFile(schemaPath)) {
            setLastError("Could not find schema file");
            return false;
        }
    }
    return true;
}

bool DatabaseManager::executeFile(const QString &filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return false;
    }

    QTextStream in(&file);
    QString content = in.readAll();
    file.close();

    // Split by semicolon and execute each statement
    QStringList statements = content.split(';', Qt::SkipEmptyParts);
    
    for (const QString &statement : statements) {
        QString trimmed = statement.trimmed();
        if (!trimmed.isEmpty()) {
            if (!executeQuery(trimmed)) {
                return false;
            }
        }
    }
    
    return true;
}

bool DatabaseManager::isInitialized() const
{
    return m_initialized;
}

// User management
int DatabaseManager::createUser(const QString &username, const QString &email)
{
    QSqlQuery query = prepareQuery(
        "INSERT INTO users (username, email) VALUES (?, ?)"
    );
    query.addBindValue(username);
    query.addBindValue(email);

    if (!query.exec()) {
        setLastError("Failed to create user: " + query.lastError().text());
        return -1;
    }

    return query.lastInsertId().toInt();
}

bool DatabaseManager::updateUser(int userId, const QString &username, const QString &email)
{
    QSqlQuery query = prepareQuery(
        "UPDATE users SET username = ?, email = ? WHERE id = ?"
    );
    query.addBindValue(username);
    query.addBindValue(email);
    query.addBindValue(userId);

    if (!query.exec()) {
        setLastError("Failed to update user: " + query.lastError().text());
        return false;
    }

    return query.numRowsAffected() > 0;
}

bool DatabaseManager::deleteUser(int userId)
{
    QSqlQuery query = prepareQuery("DELETE FROM users WHERE id = ?");
    query.addBindValue(userId);

    if (!query.exec()) {
        setLastError("Failed to delete user: " + query.lastError().text());
        return false;
    }

    return query.numRowsAffected() > 0;
}

QVariantMap DatabaseManager::getUser(int userId)
{
    QSqlQuery query = prepareQuery("SELECT * FROM users WHERE id = ?");
    query.addBindValue(userId);

    if (!query.exec() || !query.next()) {
        return QVariantMap();
    }

    QVariantMap user;
    QSqlRecord record = query.record();
    for (int i = 0; i < record.count(); ++i) {
        user[record.fieldName(i)] = record.value(i);
    }
    return user;
}

QVariantMap DatabaseManager::getUserByUsername(const QString &username)
{
    QSqlQuery query = prepareQuery("SELECT * FROM users WHERE username = ?");
    query.addBindValue(username);

    if (!query.exec() || !query.next()) {
        return QVariantMap();
    }

    QVariantMap user;
    QSqlRecord record = query.record();
    for (int i = 0; i < record.count(); ++i) {
        user[record.fieldName(i)] = record.value(i);
    }
    return user;
}

QList<QVariantMap> DatabaseManager::getAllUsers()
{
    QSqlQuery query = prepareQuery("SELECT * FROM users ORDER BY username");
    
    if (!query.exec()) {
        setLastError("Failed to get users: " + query.lastError().text());
        return QList<QVariantMap>();
    }

    QList<QVariantMap> users;
    while (query.next()) {
        QVariantMap user;
        QSqlRecord record = query.record();
        for (int i = 0; i < record.count(); ++i) {
            user[record.fieldName(i)] = record.value(i);
        }
        users.append(user);
    }
    return users;
}

// Settings management
bool DatabaseManager::setSetting(int userId, const QString &key, const QString &value)
{
    // Check if setting exists
    QSqlQuery checkQuery = prepareQuery(
        "SELECT id FROM settings WHERE user_id = ? AND key = ?"
    );
    checkQuery.addBindValue(userId);
    checkQuery.addBindValue(key);

    if (checkQuery.exec() && checkQuery.next()) {
        // Update existing setting
        QSqlQuery updateQuery = prepareQuery(
            "UPDATE settings SET value = ?, updated_at = CURRENT_TIMESTAMP WHERE user_id = ? AND key = ?"
        );
        updateQuery.addBindValue(value);
        updateQuery.addBindValue(userId);
        updateQuery.addBindValue(key);
        return updateQuery.exec();
    } else {
        // Insert new setting
        QSqlQuery insertQuery = prepareQuery(
            "INSERT INTO settings (user_id, key, value) VALUES (?, ?, ?)"
        );
        insertQuery.addBindValue(userId);
        insertQuery.addBindValue(key);
        insertQuery.addBindValue(value);
        return insertQuery.exec();
    }
}

QString DatabaseManager::getSetting(int userId, const QString &key, const QString &defaultValue)
{
    QSqlQuery query = prepareQuery(
        "SELECT value FROM settings WHERE user_id = ? AND key = ?"
    );
    query.addBindValue(userId);
    query.addBindValue(key);

    if (query.exec() && query.next()) {
        return query.value(0).toString();
    }
    return defaultValue;
}

bool DatabaseManager::deleteSetting(int userId, const QString &key)
{
    QSqlQuery query = prepareQuery(
        "DELETE FROM settings WHERE user_id = ? AND key = ?"
    );
    query.addBindValue(userId);
    query.addBindValue(key);

    return query.exec();
}

QVariantMap DatabaseManager::getAllSettings(int userId)
{
    QSqlQuery query = prepareQuery(
        "SELECT key, value FROM settings WHERE user_id = ?"
    );
    query.addBindValue(userId);

    QVariantMap settings;
    if (query.exec()) {
        while (query.next()) {
            settings[query.value(0).toString()] = query.value(1);
        }
    }
    return settings;
}

// Template management
int DatabaseManager::createTemplate(int userId, const QString &name, const QString &content)
{
    QSqlQuery query = prepareQuery(
        "INSERT INTO templates (user_id, name, content) VALUES (?, ?, ?)"
    );
    query.addBindValue(userId);
    query.addBindValue(name);
    query.addBindValue(content);

    if (!query.exec()) {
        setLastError("Failed to create template: " + query.lastError().text());
        return -1;
    }

    return query.lastInsertId().toInt();
}

bool DatabaseManager::updateTemplate(int templateId, const QString &name, const QString &content)
{
    QSqlQuery query = prepareQuery(
        "UPDATE templates SET name = ?, content = ?, updated_at = CURRENT_TIMESTAMP WHERE id = ?"
    );
    query.addBindValue(name);
    query.addBindValue(content);
    query.addBindValue(templateId);

    if (!query.exec()) {
        setLastError("Failed to update template: " + query.lastError().text());
        return false;
    }

    return query.numRowsAffected() > 0;
}

bool DatabaseManager::deleteTemplate(int templateId)
{
    QSqlQuery query = prepareQuery("DELETE FROM templates WHERE id = ?");
    query.addBindValue(templateId);

    if (!query.exec()) {
        setLastError("Failed to delete template: " + query.lastError().text());
        return false;
    }

    return query.numRowsAffected() > 0;
}

QVariantMap DatabaseManager::getTemplate(int templateId)
{
    QSqlQuery query = prepareQuery("SELECT * FROM templates WHERE id = ?");
    query.addBindValue(templateId);

    if (!query.exec() || !query.next()) {
        return QVariantMap();
    }

    QVariantMap template_;
    QSqlRecord record = query.record();
    for (int i = 0; i < record.count(); ++i) {
        template_[record.fieldName(i)] = record.value(i);
    }
    return template_;
}

QList<QVariantMap> DatabaseManager::getTemplates(int userId)
{
    QSqlQuery query = prepareQuery(
        "SELECT * FROM templates WHERE user_id = ? ORDER BY name"
    );
    query.addBindValue(userId);

    QList<QVariantMap> templates;
    if (query.exec()) {
        while (query.next()) {
            QVariantMap template_;
            QSqlRecord record = query.record();
            for (int i = 0; i < record.count(); ++i) {
                template_[record.fieldName(i)] = record.value(i);
            }
            templates.append(template_);
        }
    }
    return templates;
}

// Codeforces account management
int DatabaseManager::createCodeforcesAccount(int userId, const QString &handle, 
                                           const QString &apiKey, const QString &apiSecret)
{
    QSqlQuery query = prepareQuery(
        "INSERT INTO codeforces_accounts (user_id, handle, api_key, api_secret) VALUES (?, ?, ?, ?)"
    );
    query.addBindValue(userId);
    query.addBindValue(handle);
    query.addBindValue(apiKey);
    query.addBindValue(apiSecret);

    if (!query.exec()) {
        setLastError("Failed to create Codeforces account: " + query.lastError().text());
        return -1;
    }

    return query.lastInsertId().toInt();
}

bool DatabaseManager::updateCodeforcesAccount(int accountId, const QString &handle, 
                                            const QString &apiKey, const QString &apiSecret)
{
    QSqlQuery query = prepareQuery(
        "UPDATE codeforces_accounts SET handle = ?, api_key = ?, api_secret = ?, updated_at = CURRENT_TIMESTAMP WHERE id = ?"
    );
    query.addBindValue(handle);
    query.addBindValue(apiKey);
    query.addBindValue(apiSecret);
    query.addBindValue(accountId);

    if (!query.exec()) {
        setLastError("Failed to update Codeforces account: " + query.lastError().text());
        return false;
    }

    return query.numRowsAffected() > 0;
}

bool DatabaseManager::deleteCodeforcesAccount(int accountId)
{
    QSqlQuery query = prepareQuery("DELETE FROM codeforces_accounts WHERE id = ?");
    query.addBindValue(accountId);

    if (!query.exec()) {
        setLastError("Failed to delete Codeforces account: " + query.lastError().text());
        return false;
    }

    return query.numRowsAffected() > 0;
}

QVariantMap DatabaseManager::getCodeforcesAccount(int accountId)
{
    QSqlQuery query = prepareQuery("SELECT * FROM codeforces_accounts WHERE id = ?");
    query.addBindValue(accountId);

    if (!query.exec() || !query.next()) {
        return QVariantMap();
    }

    QVariantMap account;
    QSqlRecord record = query.record();
    for (int i = 0; i < record.count(); ++i) {
        account[record.fieldName(i)] = record.value(i);
    }
    return account;
}

QList<QVariantMap> DatabaseManager::getCodeforcesAccounts(int userId)
{
    QSqlQuery query = prepareQuery(
        "SELECT * FROM codeforces_accounts WHERE user_id = ? ORDER BY handle"
    );
    query.addBindValue(userId);

    QList<QVariantMap> accounts;
    if (query.exec()) {
        while (query.next()) {
            QVariantMap account;
            QSqlRecord record = query.record();
            for (int i = 0; i < record.count(); ++i) {
                account[record.fieldName(i)] = record.value(i);
            }
            accounts.append(account);
        }
    }
    return accounts;
}

// Contest management
int DatabaseManager::createContest(int userId, int codeforcesAccountId, const QString &name, 
                                 const QDateTime &startTime, const QDateTime &endTime)
{
    QSqlQuery query = prepareQuery(
        "INSERT INTO contests (user_id, codeforces_account_id, name, start_time, end_time) VALUES (?, ?, ?, ?, ?)"
    );
    query.addBindValue(userId);
    query.addBindValue(codeforcesAccountId);
    query.addBindValue(name);
    query.addBindValue(startTime);
    query.addBindValue(endTime.isValid() ? endTime : QVariant());

    if (!query.exec()) {
        setLastError("Failed to create contest: " + query.lastError().text());
        return -1;
    }

    return query.lastInsertId().toInt();
}

bool DatabaseManager::updateContest(int contestId, const QString &name, 
                                  const QDateTime &startTime, const QDateTime &endTime)
{
    QSqlQuery query = prepareQuery(
        "UPDATE contests SET name = ?, start_time = ?, end_time = ? WHERE id = ?"
    );
    query.addBindValue(name);
    query.addBindValue(startTime);
    query.addBindValue(endTime.isValid() ? endTime : QVariant());
    query.addBindValue(contestId);

    if (!query.exec()) {
        setLastError("Failed to update contest: " + query.lastError().text());
        return false;
    }

    return query.numRowsAffected() > 0;
}

bool DatabaseManager::deleteContest(int contestId)
{
    QSqlQuery query = prepareQuery("DELETE FROM contests WHERE id = ?");
    query.addBindValue(contestId);

    if (!query.exec()) {
        setLastError("Failed to delete contest: " + query.lastError().text());
        return false;
    }

    return query.numRowsAffected() > 0;
}

QVariantMap DatabaseManager::getContest(int contestId)
{
    QSqlQuery query = prepareQuery("SELECT * FROM contests WHERE id = ?");
    query.addBindValue(contestId);

    if (!query.exec() || !query.next()) {
        return QVariantMap();
    }

    QVariantMap contest;
    QSqlRecord record = query.record();
    for (int i = 0; i < record.count(); ++i) {
        contest[record.fieldName(i)] = record.value(i);
    }
    return contest;
}

QList<QVariantMap> DatabaseManager::getContests(int userId)
{
    QSqlQuery query = prepareQuery(
        "SELECT * FROM contests WHERE user_id = ? ORDER BY start_time DESC"
    );
    query.addBindValue(userId);

    QList<QVariantMap> contests;
    if (query.exec()) {
        while (query.next()) {
            QVariantMap contest;
            QSqlRecord record = query.record();
            for (int i = 0; i < record.count(); ++i) {
                contest[record.fieldName(i)] = record.value(i);
            }
            contests.append(contest);
        }
    }
    return contests;
}

// Snippet management
int DatabaseManager::createSnippet(int userId, const QString &name, const QString &prefix, 
                                 const QString &body, const QString &description)
{
    QSqlQuery query = prepareQuery(
        "INSERT INTO snippets (user_id, name, prefix, body, description) VALUES (?, ?, ?, ?, ?)"
    );
    query.addBindValue(userId);
    query.addBindValue(name);
    query.addBindValue(prefix);
    query.addBindValue(body);
    query.addBindValue(description);

    if (!query.exec()) {
        setLastError("Failed to create snippet: " + query.lastError().text());
        return -1;
    }

    return query.lastInsertId().toInt();
}

bool DatabaseManager::updateSnippet(int snippetId, const QString &name, const QString &prefix, 
                                  const QString &body, const QString &description)
{
    QSqlQuery query = prepareQuery(
        "UPDATE snippets SET name = ?, prefix = ?, body = ?, description = ?, updated_at = CURRENT_TIMESTAMP WHERE id = ?"
    );
    query.addBindValue(name);
    query.addBindValue(prefix);
    query.addBindValue(body);
    query.addBindValue(description);
    query.addBindValue(snippetId);

    if (!query.exec()) {
        setLastError("Failed to update snippet: " + query.lastError().text());
        return false;
    }

    return query.numRowsAffected() > 0;
}

bool DatabaseManager::deleteSnippet(int snippetId)
{
    QSqlQuery query = prepareQuery("DELETE FROM snippets WHERE id = ?");
    query.addBindValue(snippetId);

    if (!query.exec()) {
        setLastError("Failed to delete snippet: " + query.lastError().text());
        return false;
    }

    return query.numRowsAffected() > 0;
}

QVariantMap DatabaseManager::getSnippet(int snippetId)
{
    QSqlQuery query = prepareQuery("SELECT * FROM snippets WHERE id = ?");
    query.addBindValue(snippetId);

    if (!query.exec() || !query.next()) {
        return QVariantMap();
    }

    QVariantMap snippet;
    QSqlRecord record = query.record();
    for (int i = 0; i < record.count(); ++i) {
        snippet[record.fieldName(i)] = record.value(i);
    }
    return snippet;
}

QList<QVariantMap> DatabaseManager::getSnippets(int userId)
{
    QSqlQuery query = prepareQuery(
        "SELECT * FROM snippets WHERE user_id = ? ORDER BY name"
    );
    query.addBindValue(userId);

    QList<QVariantMap> snippets;
    if (query.exec()) {
        while (query.next()) {
            QVariantMap snippet;
            QSqlRecord record = query.record();
            for (int i = 0; i < record.count(); ++i) {
                snippet[record.fieldName(i)] = record.value(i);
            }
            snippets.append(snippet);
        }
    }
    return snippets;
}

// Theme management
int DatabaseManager::createTheme(int userId, const QString &name, const QString &qssContent)
{
    QSqlQuery query = prepareQuery(
        "INSERT INTO themes (user_id, name, qss_content) VALUES (?, ?, ?)"
    );
    query.addBindValue(userId);
    query.addBindValue(name);
    query.addBindValue(qssContent);

    if (!query.exec()) {
        setLastError("Failed to create theme: " + query.lastError().text());
        return -1;
    }

    return query.lastInsertId().toInt();
}

bool DatabaseManager::updateTheme(int themeId, const QString &name, const QString &qssContent)
{
    QSqlQuery query = prepareQuery(
        "UPDATE themes SET name = ?, qss_content = ?, updated_at = CURRENT_TIMESTAMP WHERE id = ?"
    );
    query.addBindValue(name);
    query.addBindValue(qssContent);
    query.addBindValue(themeId);

    if (!query.exec()) {
        setLastError("Failed to update theme: " + query.lastError().text());
        return false;
    }

    return query.numRowsAffected() > 0;
}

bool DatabaseManager::deleteTheme(int themeId)
{
    QSqlQuery query = prepareQuery("DELETE FROM themes WHERE id = ?");
    query.addBindValue(themeId);

    if (!query.exec()) {
        setLastError("Failed to delete theme: " + query.lastError().text());
        return false;
    }

    return query.numRowsAffected() > 0;
}

QVariantMap DatabaseManager::getTheme(int themeId)
{
    QSqlQuery query = prepareQuery("SELECT * FROM themes WHERE id = ?");
    query.addBindValue(themeId);

    if (!query.exec() || !query.next()) {
        return QVariantMap();
    }

    QVariantMap theme;
    QSqlRecord record = query.record();
    for (int i = 0; i < record.count(); ++i) {
        theme[record.fieldName(i)] = record.value(i);
    }
    return theme;
}

QList<QVariantMap> DatabaseManager::getThemes(int userId)
{
    QSqlQuery query = prepareQuery(
        "SELECT * FROM themes WHERE user_id = ? ORDER BY name"
    );
    query.addBindValue(userId);

    QList<QVariantMap> themes;
    if (query.exec()) {
        while (query.next()) {
            QVariantMap theme;
            QSqlRecord record = query.record();
            for (int i = 0; i < record.count(); ++i) {
                theme[record.fieldName(i)] = record.value(i);
            }
            themes.append(theme);
        }
    }
    return themes;
}

// Utility methods
QSqlDatabase DatabaseManager::getDatabase() const
{
    return m_database;
}

QString DatabaseManager::getLastError() const
{
    return m_lastError;
}

bool DatabaseManager::executeQuery(const QString &query)
{
    QSqlQuery sqlQuery(m_database);
    if (!sqlQuery.exec(query)) {
        setLastError("Query execution failed: " + sqlQuery.lastError().text());
        return false;
    }
    return true;
}

QSqlQuery DatabaseManager::prepareQuery(const QString &query)
{
    QSqlQuery sqlQuery(m_database);
    sqlQuery.prepare(query);
    return sqlQuery;
}

void DatabaseManager::setLastError(const QString &error)
{
    m_lastError = error;
    qDebug() << "DatabaseManager Error:" << error;
} 