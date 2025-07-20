#ifndef DATABASESERVICE_H
#define DATABASESERVICE_H

#include <QObject>
#include <QString>
#include <QList>
#include "Models.h"
#include "UserRepository.h"
#include "ThemeRepository.h"

/**
 * Servicio de alto nivel para operaciones comunes de la base de datos.
 * Esta clase proporciona una API simplificada para las operaciones más comunes.
 */
class DatabaseService : public QObject
{
    Q_OBJECT

public:
    explicit DatabaseService(QObject *parent = nullptr);
    
    // User management
    bool createDefaultUser();
    User getCurrentUser();
    bool setCurrentUser(const QString& username);
    
    // Theme management
    bool saveTheme(const QString& themeName, const QString& qssContent);
    bool loadTheme(const QString& themeName, QString& qssContent);
    QList<Theme> getUserThemes();
    bool deleteTheme(const QString& themeName);
    bool applyTheme(const QString& themeName);
    
    // Settings (future implementation)
    bool saveSetting(const QString& key, const QString& value);
    QString getSetting(const QString& key, const QString& defaultValue = QString());
    
    // Utility
    bool isInitialized() const;
    QString getLastError() const;

signals:
    void themeChanged(const QString& themeName);
    void userChanged(const QString& username);
    void databaseError(const QString& error);

private:
    UserRepository m_userRepository;
    ThemeRepository m_themeRepository;
    User m_currentUser;
    QString m_lastError;
    
    void setError(const QString& error);
};

#endif // DATABASESERVICE_H
