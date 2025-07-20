#include "Models.h"

// User implementation
User::User(const QString& username, const QString& email)
    : username(username), email(email), createdAt(QDateTime::currentDateTime())
{
}

// Setting implementation
Setting::Setting(int userId, const QString& key, const QString& value)
    : userId(userId), key(key), value(value), updatedAt(QDateTime::currentDateTime())
{
}

// Template implementation
Template::Template(int userId, const QString& name, const QString& content)
    : userId(userId), name(name), content(content), 
      createdAt(QDateTime::currentDateTime()), updatedAt(QDateTime::currentDateTime())
{
}

// CodeforcesAccount implementation
CodeforcesAccount::CodeforcesAccount(int userId, const QString& handle, const QString& apiKey, const QString& apiSecret)
    : userId(userId), handle(handle), apiKey(apiKey), apiSecret(apiSecret),
      createdAt(QDateTime::currentDateTime()), updatedAt(QDateTime::currentDateTime())
{
}

// Contest implementation
Contest::Contest(int userId, int codeforcesAccountId, const QString& name, const QDateTime& startTime, const QDateTime& endTime)
    : userId(userId), codeforcesAccountId(codeforcesAccountId), name(name), 
      startTime(startTime), endTime(endTime), createdAt(QDateTime::currentDateTime())
{
}

// Snippet implementation
Snippet::Snippet(int userId, const QString& name, const QString& prefix, const QString& body, const QString& description)
    : userId(userId), name(name), prefix(prefix), body(body), description(description),
      createdAt(QDateTime::currentDateTime()), updatedAt(QDateTime::currentDateTime())
{
}

// Theme implementation
Theme::Theme(int userId, const QString& name, const QString& qssContent)
    : userId(userId), name(name), qssContent(qssContent),
      createdAt(QDateTime::currentDateTime()), updatedAt(QDateTime::currentDateTime())
{
}
