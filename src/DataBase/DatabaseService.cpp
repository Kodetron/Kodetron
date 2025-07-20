#include "DatabaseService.h"
#include "DatabaseManager.h"
#include <QDebug>
#include <QApplication>

DatabaseService::DatabaseService(QObject *parent)
    : QObject(parent)
{
    // Initialize with default user if none exists
    if (m_userRepository.count() == 0) {
        createDefaultUser();
    }
    
    // Set current user to the first user found
    auto users = m_userRepository.findAll();
    if (!users.isEmpty()) {
        m_currentUser = users.first();
    }
}

bool DatabaseService::createDefaultUser()
{
    QString defaultUsername = "default_user";
    QString defaultEmail = "";
    
    // Check if default user already exists
    if (m_userRepository.existsByUsername(defaultUsername)) {
        User user = m_userRepository.findByUsername(defaultUsername);
        if (user.isValid()) {
            m_currentUser = user;
            return true;
        }
    }
    
    User defaultUser(defaultUsername, defaultEmail);
    if (m_userRepository.create(defaultUser)) {
        m_currentUser = defaultUser;
        emit userChanged(defaultUsername);
        return true;
    }
    
    setError("Failed to create default user");
    return false;
}

User DatabaseService::getCurrentUser()
{
    return m_currentUser;
}

bool DatabaseService::setCurrentUser(const QString& username)
{
    User user = m_userRepository.findByUsername(username);
    if (user.isValid()) {
        m_currentUser = user;
        emit userChanged(username);
        return true;
    }
    
    setError("User not found: " + username);
    return false;
}

bool DatabaseService::saveTheme(const QString& themeName, const QString& qssContent)
{
    if (!m_currentUser.isValid()) {
        setError("No current user set");
        return false;
    }
    
    // Check if theme already exists
    Theme existingTheme = m_themeRepository.findByUserIdAndName(m_currentUser.id, themeName);
    
    if (existingTheme.isValid()) {
        // Update existing theme
        existingTheme.qssContent = qssContent;
        
        if (m_themeRepository.update(existingTheme)) {
            emit themeChanged(themeName);
            return true;
        }
    } else {
        // Create new theme
        Theme newTheme(m_currentUser.id, themeName, qssContent);
        
        if (m_themeRepository.create(newTheme)) {
            emit themeChanged(themeName);
            return true;
        }
    }
    
    setError("Failed to save theme: " + themeName);
    return false;
}

bool DatabaseService::loadTheme(const QString& themeName, QString& qssContent)
{
    if (!m_currentUser.isValid()) {
        setError("No current user set");
        return false;
    }
    
    Theme theme = m_themeRepository.findByUserIdAndName(m_currentUser.id, themeName);
    if (theme.isValid()) {
        qssContent = theme.qssContent;
        return true;
    }
    
    setError("Theme not found: " + themeName);
    return false;
}

QList<Theme> DatabaseService::getUserThemes()
{
    if (!m_currentUser.isValid()) {
        setError("No current user set");
        return QList<Theme>();
    }
    
    return m_themeRepository.findByUserId(m_currentUser.id);
}

bool DatabaseService::deleteTheme(const QString& themeName)
{
    if (!m_currentUser.isValid()) {
        setError("No current user set");
        return false;
    }
    
    Theme theme = m_themeRepository.findByUserIdAndName(m_currentUser.id, themeName);
    if (theme.isValid()) {
        if (m_themeRepository.deleteById(theme.id)) {
            emit themeChanged(QString()); // Empty string indicates theme was deleted
            return true;
        }
    }
    
    setError("Failed to delete theme: " + themeName);
    return false;
}

bool DatabaseService::applyTheme(const QString& themeName)
{
    QString qssContent;
    if (loadTheme(themeName, qssContent)) {
        // Apply the theme to the application
        QApplication::instance()->setStyleSheet(qssContent);
        emit themeChanged(themeName);
        return true;
    }
    
    return false;
}

bool DatabaseService::saveSetting(const QString& key, const QString& value)
{
    // TODO: Implement settings repository
    Q_UNUSED(key)
    Q_UNUSED(value)
    setError("Settings not implemented yet");
    return false;
}

QString DatabaseService::getSetting(const QString& key, const QString& defaultValue)
{
    // TODO: Implement settings repository
    Q_UNUSED(key)
    setError("Settings not implemented yet");
    return defaultValue;
}

bool DatabaseService::isInitialized() const
{
    return DatabaseManager::instance()->isConnected() && m_currentUser.isValid();
}

QString DatabaseService::getLastError() const
{
    return m_lastError;
}

void DatabaseService::setError(const QString& error)
{
    m_lastError = error;
    qWarning() << "DatabaseService error:" << error;
    emit databaseError(error);
}
