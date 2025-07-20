#ifndef MODELS_H
#define MODELS_H

#include <QString>
#include <QDateTime>
#include <QVariant>

struct User {
    int id = -1;
    QString username;
    QString email;
    QDateTime createdAt;
    
    User() = default;
    User(const QString& username, const QString& email = QString());
    
    bool isValid() const { return id >= 0; }
};

struct Setting {
    int id = -1;
    int userId = -1;
    QString key;
    QString value;
    QDateTime updatedAt;
    
    Setting() = default;
    Setting(int userId, const QString& key, const QString& value);
    
    bool isValid() const { return id >= 0; }
};

struct Template {
    int id = -1;
    int userId = -1;
    QString name;
    QString content;
    QDateTime createdAt;
    QDateTime updatedAt;
    
    Template() = default;
    Template(int userId, const QString& name, const QString& content);
    
    bool isValid() const { return id >= 0; }
};

struct CodeforcesAccount {
    int id = -1;
    int userId = -1;
    QString handle;
    QString apiKey;
    QString apiSecret;
    QDateTime createdAt;
    QDateTime updatedAt;
    
    CodeforcesAccount() = default;
    CodeforcesAccount(int userId, const QString& handle, const QString& apiKey = QString(), const QString& apiSecret = QString());
    
    bool isValid() const { return id >= 0; }
};

struct Contest {
    int id = -1;
    int userId = -1;
    int codeforcesAccountId = -1;
    QString name;
    QDateTime startTime;
    QDateTime endTime;
    QDateTime createdAt;
    
    Contest() = default;
    Contest(int userId, int codeforcesAccountId, const QString& name, const QDateTime& startTime, const QDateTime& endTime = QDateTime());
    
    bool isValid() const { return id >= 0; }
};

struct Snippet {
    int id = -1;
    int userId = -1;
    QString name;
    QString prefix;
    QString body;
    QString description;
    QDateTime createdAt;
    QDateTime updatedAt;
    
    Snippet() = default;
    Snippet(int userId, const QString& name, const QString& prefix, const QString& body, const QString& description = QString());
    
    bool isValid() const { return id >= 0; }
};

struct Theme {
    int id = -1;
    int userId = -1;
    QString name;
    QString qssContent;
    QDateTime createdAt;
    QDateTime updatedAt;
    
    Theme() = default;
    Theme(int userId, const QString& name, const QString& qssContent);
    
    bool isValid() const { return id >= 0; }
};

#endif // MODELS_H
