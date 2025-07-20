#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QtCore/QObject>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QtCore/QString>
#include <QtCore/QVariant>
#include <QtCore/QDateTime>
#include <memory>

class DatabaseManager : public QObject
{
    Q_OBJECT

public:
    static DatabaseManager* instance();
    
    bool initialize(const QString& databasePath = "kodetron.db");
    bool isConnected() const;
    void close();
    
    // Transaction methods
    bool beginTransaction();
    bool commitTransaction();
    bool rollbackTransaction();
    
    // Utility methods
    QSqlQuery executeQuery(const QString& query, const QVariantList& params = QVariantList());
    QString lastError() const;
    int lastInsertId() const;
    
    // Database structure
    bool createTables();
    
private:
    explicit DatabaseManager(QObject *parent = nullptr);
    ~DatabaseManager();
    
    bool createUsersTable();
    bool createSettingsTable();
    bool createTemplatesTable();
    bool createCodeforcesAccountsTable();
    bool createContestsTable();
    bool createSnippetsTable();
    bool createThemesTable();
    
    static std::unique_ptr<DatabaseManager> m_instance;
    QSqlDatabase m_database;
    QString m_lastError;
};

#endif // DATABASEMANAGER_H
