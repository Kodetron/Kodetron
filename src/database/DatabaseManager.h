#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QString>
#include <QVariant>
#include <QDateTime>
#include <QDebug>

class DatabaseManager : public QObject
{
    Q_OBJECT

public:
    explicit DatabaseManager(QObject *parent = nullptr);
    ~DatabaseManager();

    // Database initialization
    bool initialize(const QString &dbPath = "kodetron.db");
    bool createTables();
    bool isInitialized() const;

    // User management
    int createUser(const QString &username, const QString &email = QString());
    bool updateUser(int userId, const QString &username, const QString &email);
    bool deleteUser(int userId);
    QVariantMap getUser(int userId);
    QVariantMap getUserByUsername(const QString &username);
    QList<QVariantMap> getAllUsers();

    // Settings management
    bool setSetting(int userId, const QString &key, const QString &value);
    QString getSetting(int userId, const QString &key, const QString &defaultValue = QString());
    bool deleteSetting(int userId, const QString &key);
    QVariantMap getAllSettings(int userId);

    // Template management
    int createTemplate(int userId, const QString &name, const QString &content);
    bool updateTemplate(int templateId, const QString &name, const QString &content);
    bool deleteTemplate(int templateId);
    QVariantMap getTemplate(int templateId);
    QList<QVariantMap> getTemplates(int userId);

    // Codeforces account management
    int createCodeforcesAccount(int userId, const QString &handle, 
                               const QString &apiKey = QString(), 
                               const QString &apiSecret = QString());
    bool updateCodeforcesAccount(int accountId, const QString &handle, 
                                const QString &apiKey = QString(), 
                                const QString &apiSecret = QString());
    bool deleteCodeforcesAccount(int accountId);
    QVariantMap getCodeforcesAccount(int accountId);
    QList<QVariantMap> getCodeforcesAccounts(int userId);

    // Contest management
    int createContest(int userId, int codeforcesAccountId, const QString &name, 
                     const QDateTime &startTime, const QDateTime &endTime = QDateTime());
    bool updateContest(int contestId, const QString &name, 
                      const QDateTime &startTime, const QDateTime &endTime = QDateTime());
    bool deleteContest(int contestId);
    QVariantMap getContest(int contestId);
    QList<QVariantMap> getContests(int userId);

    // Snippet management
    int createSnippet(int userId, const QString &name, const QString &prefix, 
                     const QString &body, const QString &description = QString());
    bool updateSnippet(int snippetId, const QString &name, const QString &prefix, 
                      const QString &body, const QString &description = QString());
    bool deleteSnippet(int snippetId);
    QVariantMap getSnippet(int snippetId);
    QList<QVariantMap> getSnippets(int userId);

    // Theme management
    int createTheme(int userId, const QString &name, const QString &qssContent);
    bool updateTheme(int themeId, const QString &name, const QString &qssContent);
    bool deleteTheme(int themeId);
    QVariantMap getTheme(int themeId);
    QList<QVariantMap> getThemes(int userId);

    // Utility methods
    QSqlDatabase getDatabase() const;
    QString getLastError() const;
    bool executeQuery(const QString &query);
    QSqlQuery prepareQuery(const QString &query);

private:
    QSqlDatabase m_database;
    bool m_initialized;
    QString m_lastError;

    void setLastError(const QString &error);
    bool executeFile(const QString &filePath);
};

#endif // DATABASEMANAGER_H 