#include "HttpApiClient.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QTimer>
#include <QDebug>

HttpApiClient::HttpApiClient(const QString& baseUrl, QObject *parent)
    : QObject(parent)
    , m_manager(new QNetworkAccessManager(this))
    , m_baseUrl(baseUrl)
    , m_timeout(5000) // 5 seconds default timeout
{
    connect(m_manager, &QNetworkAccessManager::finished,
            this, &HttpApiClient::onReplyFinished);
}

void HttpApiClient::getAllUsers(std::function<void(QList<User>, QString)> callback) {
    makeRequest("/users", "GET", QByteArray(), [this, callback](QNetworkReply* reply) {
        if (reply->error() == QNetworkReply::NoError) {
            QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
            QList<User> users = parseUsers(doc.array());
            callback(users, QString());
        } else {
            callback(QList<User>(), reply->errorString());
        }
    });
}

void HttpApiClient::createUser(const User& user, std::function<void(User, QString)> callback) {
    QByteArray data = userToJson(user).toUtf8();
    makeRequest("/users", "POST", data, [this, callback](QNetworkReply* reply) {
        if (reply->error() == QNetworkReply::NoError) {
            QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
            User createdUser = parseUser(doc.object());
            callback(createdUser, QString());
        } else {
            callback(User(), reply->errorString());
        }
    });
}

void HttpApiClient::getUserById(int userId, std::function<void(User, QString)> callback) {
    makeRequest("/users/" + QString::number(userId), "GET", QByteArray(), 
               [this, callback](QNetworkReply* reply) {
        if (reply->error() == QNetworkReply::NoError) {
            QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
            User user = parseUser(doc.object());
            callback(user, QString());
        } else {
            callback(User(), reply->errorString());
        }
    });
}

void HttpApiClient::updateUser(const User& user, std::function<void(User, QString)> callback) {
    QByteArray data = userToJson(user).toUtf8();
    makeRequest("/users/" + QString::number(user.id), "PUT", data, 
               [this, callback](QNetworkReply* reply) {
        if (reply->error() == QNetworkReply::NoError) {
            QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
            User updatedUser = parseUser(doc.object());
            callback(updatedUser, QString());
        } else {
            callback(User(), reply->errorString());
        }
    });
}

void HttpApiClient::deleteUser(int userId, std::function<void(bool, QString)> callback) {
    makeRequest("/users/" + QString::number(userId), "DELETE", QByteArray(), 
               [callback](QNetworkReply* reply) {
        bool success = (reply->error() == QNetworkReply::NoError);
        callback(success, success ? QString() : reply->errorString());
    });
}

void HttpApiClient::getThemes(int userId, std::function<void(QList<Theme>, QString)> callback) {
    QString endpoint = "/themes?user_id=" + QString::number(userId);
    makeRequest(endpoint, "GET", QByteArray(), [this, callback](QNetworkReply* reply) {
        if (reply->error() == QNetworkReply::NoError) {
            QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
            QList<Theme> themes = parseThemes(doc.array());
            callback(themes, QString());
        } else {
            callback(QList<Theme>(), reply->errorString());
        }
    });
}

void HttpApiClient::createTheme(const Theme& theme, std::function<void(Theme, QString)> callback) {
    QByteArray data = themeToJson(theme).toUtf8();
    makeRequest("/themes", "POST", data, [this, callback](QNetworkReply* reply) {
        if (reply->error() == QNetworkReply::NoError) {
            QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
            Theme createdTheme = parseTheme(doc.object());
            callback(createdTheme, QString());
        } else {
            callback(Theme(), reply->errorString());
        }
    });
}

void HttpApiClient::getThemeById(int themeId, std::function<void(Theme, QString)> callback) {
    makeRequest("/themes/" + QString::number(themeId), "GET", QByteArray(), 
               [this, callback](QNetworkReply* reply) {
        if (reply->error() == QNetworkReply::NoError) {
            QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
            Theme theme = parseTheme(doc.object());
            callback(theme, QString());
        } else {
            callback(Theme(), reply->errorString());
        }
    });
}

void HttpApiClient::updateTheme(const Theme& theme, std::function<void(Theme, QString)> callback) {
    QByteArray data = themeToJson(theme).toUtf8();
    makeRequest("/themes/" + QString::number(theme.id), "PUT", data, 
               [this, callback](QNetworkReply* reply) {
        if (reply->error() == QNetworkReply::NoError) {
            QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
            Theme updatedTheme = parseTheme(doc.object());
            callback(updatedTheme, QString());
        } else {
            callback(Theme(), reply->errorString());
        }
    });
}

void HttpApiClient::deleteTheme(int themeId, std::function<void(bool, QString)> callback) {
    makeRequest("/themes/" + QString::number(themeId), "DELETE", QByteArray(), 
               [callback](QNetworkReply* reply) {
        bool success = (reply->error() == QNetworkReply::NoError);
        callback(success, success ? QString() : reply->errorString());
    });
}

void HttpApiClient::checkHealth(std::function<void(bool, QString)> callback) {
    makeRequest("/health", "GET", QByteArray(), [callback](QNetworkReply* reply) {
        bool healthy = (reply->error() == QNetworkReply::NoError);
        QString message = healthy ? "Server is healthy" : reply->errorString();
        callback(healthy, message);
    });
}

void HttpApiClient::setBaseUrl(const QString& baseUrl) {
    m_baseUrl = baseUrl;
}

void HttpApiClient::setTimeout(int timeoutMs) {
    m_timeout = timeoutMs;
}

void HttpApiClient::onReplyFinished() {
    // This is handled by individual request callbacks
}

void HttpApiClient::makeRequest(const QString& endpoint, const QString& method, 
                               const QByteArray& data, std::function<void(QNetworkReply*)> callback) {
    QNetworkRequest request = createRequest(endpoint);
    QNetworkReply* reply = nullptr;
    
    if (method == "GET") {
        reply = m_manager->get(request);
    } else if (method == "POST") {
        reply = m_manager->post(request, data);
    } else if (method == "PUT") {
        reply = m_manager->put(request, data);
    } else if (method == "DELETE") {
        reply = m_manager->deleteResource(request);
    }
    
    if (reply) {
        // Set timeout
        QTimer::singleShot(m_timeout, reply, &QNetworkReply::abort);
        
        connect(reply, &QNetworkReply::finished, [reply, callback]() {
            callback(reply);
            reply->deleteLater();
        });
    }
}

QNetworkRequest HttpApiClient::createRequest(const QString& endpoint) {
    QNetworkRequest request(QUrl(m_baseUrl + endpoint));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("Accept", "application/json");
    return request;
}

QString HttpApiClient::userToJson(const User& user) {
    QJsonObject json;
    json["username"] = user.username;
    json["email"] = user.email;
    return QJsonDocument(json).toJson(QJsonDocument::Compact);
}

QString HttpApiClient::themeToJson(const Theme& theme) {
    QJsonObject json;
    json["user_id"] = theme.userId;
    json["name"] = theme.name;
    json["qss_content"] = theme.qssContent;
    return QJsonDocument(json).toJson(QJsonDocument::Compact);
}

User HttpApiClient::parseUser(const QJsonObject& json) {
    User user;
    user.id = json["id"].toInt();
    user.username = json["username"].toString();
    user.email = json["email"].toString();
    user.createdAt = QDateTime::fromString(json["created_at"].toString(), Qt::ISODate);
    return user;
}

Theme HttpApiClient::parseTheme(const QJsonObject& json) {
    Theme theme;
    theme.id = json["id"].toInt();
    theme.userId = json["user_id"].toInt();
    theme.name = json["name"].toString();
    theme.qssContent = json["qss_content"].toString();
    theme.createdAt = QDateTime::fromString(json["created_at"].toString(), Qt::ISODate);
    theme.updatedAt = QDateTime::fromString(json["updated_at"].toString(), Qt::ISODate);
    return theme;
}

QList<User> HttpApiClient::parseUsers(const QJsonArray& json) {
    QList<User> users;
    for (const auto& value : json) {
        users.append(parseUser(value.toObject()));
    }
    return users;
}

QList<Theme> HttpApiClient::parseThemes(const QJsonArray& json) {
    QList<Theme> themes;
    for (const auto& value : json) {
        themes.append(parseTheme(value.toObject()));
    }
    return themes;
}
