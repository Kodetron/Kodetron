#include "DatabaseManager.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QStandardPaths>
#include <QDir>
#include <QDebug>
#include <QCoreApplication>

std::unique_ptr<DatabaseManager> DatabaseManager::m_instance = nullptr;

DatabaseManager* DatabaseManager::instance()
{
    if (!m_instance) {
        m_instance = std::unique_ptr<DatabaseManager>(new DatabaseManager());
    }
    return m_instance.get();
}

DatabaseManager::DatabaseManager(QObject *parent)
    : QObject(parent)
{
    // Constructor privado para patrón Singleton
}

DatabaseManager::~DatabaseManager()
{
    close();
}

bool DatabaseManager::initialize(const QString& databasePath)
{
    if (m_database.isValid() && m_database.isOpen()) {
        return true;
    }
    
    m_database = QSqlDatabase::addDatabase("QSQLITE");
    
    QString dbPath;
    if (databasePath.isEmpty()) {
        // Usar directorio de datos de aplicación por defecto
        QString dataDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
        QDir dir;
        if (!dir.exists(dataDir)) {
            dir.mkpath(dataDir);
        }
        dbPath = dataDir + "/kodetron.db";
    } else {
        dbPath = databasePath;
    }
    
    m_database.setDatabaseName(dbPath);
    
    if (!m_database.open()) {
        m_lastError = m_database.lastError().text();
        qCritical() << "Error opening database:" << m_lastError;
        return false;
    }
    
    qDebug() << "Database opened successfully at:" << dbPath;
    
    // Crear tablas si no existen
    if (!createTables()) {
        m_lastError = "Failed to create database tables";
        return false;
    }
    
    return true;
}

bool DatabaseManager::isConnected() const
{
    return m_database.isValid() && m_database.isOpen();
}

void DatabaseManager::close()
{
    if (m_database.isOpen()) {
        m_database.close();
    }
}

bool DatabaseManager::beginTransaction()
{
    if (!isConnected()) {
        m_lastError = "Database not connected";
        return false;
    }
    
    return m_database.transaction();
}

bool DatabaseManager::commitTransaction()
{
    if (!isConnected()) {
        m_lastError = "Database not connected";
        return false;
    }
    
    return m_database.commit();
}

bool DatabaseManager::rollbackTransaction()
{
    if (!isConnected()) {
        m_lastError = "Database not connected";
        return false;
    }
    
    return m_database.rollback();
}

QSqlQuery DatabaseManager::executeQuery(const QString& query, const QVariantList& params)
{
    QSqlQuery sqlQuery(m_database);
    sqlQuery.prepare(query);
    
    for (const QVariant& param : params) {
        sqlQuery.addBindValue(param);
    }
    
    if (!sqlQuery.exec()) {
        m_lastError = sqlQuery.lastError().text();
        qCritical() << "Query execution failed:" << m_lastError;
        qCritical() << "Query:" << query;
    }
    
    return sqlQuery;
}

QString DatabaseManager::lastError() const
{
    return m_lastError;
}

int DatabaseManager::lastInsertId() const
{
    if (!isConnected()) {
        return -1;
    }
    
    QSqlQuery query("SELECT last_insert_rowid()", m_database);
    if (query.exec() && query.next()) {
        return query.value(0).toInt();
    }
    
    return -1;
}

bool DatabaseManager::createTables()
{
    if (!isConnected()) {
        m_lastError = "Database not connected";
        return false;
    }
    
    bool success = true;
    success &= createUsersTable();
    success &= createSettingsTable();
    success &= createTemplatesTable();
    success &= createCodeforcesAccountsTable();
    success &= createContestsTable();
    success &= createSnippetsTable();
    success &= createThemesTable();
    
    return success;
}

bool DatabaseManager::createUsersTable()
{
    QString query = R"(
        CREATE TABLE IF NOT EXISTS users (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            username VARCHAR NOT NULL UNIQUE,
            email VARCHAR,
            created_at TIMESTAMP NOT NULL
        )
    )";
    
    QSqlQuery sqlQuery = executeQuery(query);
    return !sqlQuery.lastError().isValid();
}

bool DatabaseManager::createSettingsTable()
{
    QString query = R"(
        CREATE TABLE IF NOT EXISTS settings (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            user_id INTEGER NOT NULL,
            key VARCHAR NOT NULL,
            value TEXT NOT NULL,
            updated_at TIMESTAMP NOT NULL,
            FOREIGN KEY (user_id) REFERENCES users(id)
        )
    )";
    
    QSqlQuery sqlQuery = executeQuery(query);
    return !sqlQuery.lastError().isValid();
}

bool DatabaseManager::createTemplatesTable()
{
    QString query = R"(
        CREATE TABLE IF NOT EXISTS templates (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            user_id INTEGER NOT NULL,
            name VARCHAR NOT NULL,
            content TEXT NOT NULL,
            created_at TIMESTAMP NOT NULL,
            updated_at TIMESTAMP NOT NULL,
            FOREIGN KEY (user_id) REFERENCES users(id)
        )
    )";
    
    QSqlQuery sqlQuery = executeQuery(query);
    return !sqlQuery.lastError().isValid();
}

bool DatabaseManager::createCodeforcesAccountsTable()
{
    QString query = R"(
        CREATE TABLE IF NOT EXISTS codeforces_accounts (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            user_id INTEGER NOT NULL,
            handle VARCHAR NOT NULL,
            api_key VARCHAR,
            api_secret VARCHAR,
            created_at TIMESTAMP NOT NULL,
            updated_at TIMESTAMP NOT NULL,
            FOREIGN KEY (user_id) REFERENCES users(id)
        )
    )";
    
    QSqlQuery sqlQuery = executeQuery(query);
    return !sqlQuery.lastError().isValid();
}

bool DatabaseManager::createContestsTable()
{
    QString query = R"(
        CREATE TABLE IF NOT EXISTS contests (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            user_id INTEGER NOT NULL,
            codeforces_account_id INTEGER NOT NULL,
            name VARCHAR NOT NULL,
            start_time TIMESTAMP NOT NULL,
            end_time TIMESTAMP,
            created_at TIMESTAMP NOT NULL,
            FOREIGN KEY (user_id) REFERENCES users(id),
            FOREIGN KEY (codeforces_account_id) REFERENCES codeforces_accounts(id)
        )
    )";
    
    QSqlQuery sqlQuery = executeQuery(query);
    return !sqlQuery.lastError().isValid();
}

bool DatabaseManager::createSnippetsTable()
{
    QString query = R"(
        CREATE TABLE IF NOT EXISTS snippets (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            user_id INTEGER NOT NULL,
            name VARCHAR NOT NULL,
            prefix VARCHAR NOT NULL,
            body TEXT NOT NULL,
            description TEXT,
            created_at TIMESTAMP NOT NULL,
            updated_at TIMESTAMP NOT NULL,
            FOREIGN KEY (user_id) REFERENCES users(id)
        )
    )";
    
    QSqlQuery sqlQuery = executeQuery(query);
    return !sqlQuery.lastError().isValid();
}

bool DatabaseManager::createThemesTable()
{
    QString query = R"(
        CREATE TABLE IF NOT EXISTS themes (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            user_id INTEGER NOT NULL,
            name VARCHAR NOT NULL,
            qss_content TEXT NOT NULL,
            created_at TIMESTAMP NOT NULL,
            updated_at TIMESTAMP NOT NULL,
            FOREIGN KEY (user_id) REFERENCES users(id)
        )
    )";
    
    QSqlQuery sqlQuery = executeQuery(query);
    return !sqlQuery.lastError().isValid();
}
