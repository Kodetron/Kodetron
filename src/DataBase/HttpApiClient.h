#ifndef HTTPAPICLIENT_H
#define HTTPAPICLIENT_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QString>
#include <QList>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <functional>
#include "DataBase/Models.h"

class HttpApiClient : public QObject
{
    Q_OBJECT

public:
    explicit HttpApiClient(const QString& baseUrl = "http://localhost:8080/api", QObject *parent = nullptr);
    
    // User operations
    void getAllUsers(std::function<void(QList<User>, QString)> callback);
    void createUser(const User& user, std::function<void(User, QString)> callback);
    void getUserById(int userId, std::function<void(User, QString)> callback);
    void updateUser(const User& user, std::function<void(User, QString)> callback);
    void deleteUser(int userId, std::function<void(bool, QString)> callback);
    
    // Theme operations
    void getThemes(int userId, std::function<void(QList<Theme>, QString)> callback);
    void createTheme(const Theme& theme, std::function<void(Theme, QString)> callback);
    void getThemeById(int themeId, std::function<void(Theme, QString)> callback);
    void updateTheme(const Theme& theme, std::function<void(Theme, QString)> callback);
    void deleteTheme(int themeId, std::function<void(bool, QString)> callback);
    
    // Server health check
    void checkHealth(std::function<void(bool, QString)> callback);
    
    // Configuration
    void setBaseUrl(const QString& baseUrl);
    void setTimeout(int timeoutMs);
    
signals:
    void networkError(const QString& error);

private slots:
    void onReplyFinished();

private:
    QNetworkAccessManager* m_manager;
    QString m_baseUrl;
    int m_timeout;
    
    void makeRequest(const QString& endpoint, const QString& method, const QByteArray& data,
                    std::function<void(QNetworkReply*)> callback);
    
    QNetworkRequest createRequest(const QString& endpoint);
    QString userToJson(const User& user);
    QString themeToJson(const Theme& theme);
    User parseUser(const QJsonObject& json);
    Theme parseTheme(const QJsonObject& json);
    QList<User> parseUsers(const QJsonArray& json);
    QList<Theme> parseThemes(const QJsonArray& json);
};

#endif // HTTPAPICLIENT_H
