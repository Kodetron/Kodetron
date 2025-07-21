#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <sqlite3.h>
#include <string>
#include <vector>
#include <memory>
#include <QStandardPaths>
#include <QDir>
#include <QString>

struct User {
    int id;
    std::string codeforces_handle;
    std::string email;
    int settings_id;
};

struct Template {
    int id;
    std::string name;
    std::string content;
    int user_id;
};

struct Snippet {
    int id;
    std::string name;
    std::string content;
    int user_id;
};

struct Settings {
    int id;
    std::string name;
};

class DatabaseManager {
public:
    DatabaseManager();
    ~DatabaseManager();
    
    // Database initialization
    bool initializeDatabase();
    bool isDatabaseInitialized() const;
    
    // User operations
    bool createUser(const std::string& codeforces_handle, const std::string& email);
    bool getUserById(int id, User& user);
    bool updateUser(const User& user);
    bool deleteUser(int id);
    std::vector<User> getAllUsers();
    
    // Template operations
    bool createTemplate(const std::string& name, const std::string& content, int user_id);
    bool getTemplateById(int id, Template& template_obj);
    bool updateTemplate(const Template& template_obj);
    bool deleteTemplate(int id);
    std::vector<Template> getTemplatesByUserId(int user_id);
    
    // Snippet operations
    bool createSnippet(const std::string& name, const std::string& content, int user_id);
    bool getSnippetById(int id, Snippet& snippet);
    bool updateSnippet(const Snippet& snippet);
    bool deleteSnippet(int id);
    std::vector<Snippet> getSnippetsByUserId(int user_id);
    
    // Settings operations
    bool createSettings(const std::string& name);
    bool getSettingsById(int id, Settings& settings);
    bool updateSettings(const Settings& settings);
    bool deleteSettings(int id);
    
private:
    sqlite3* db;
    std::string db_path;
    
    // Helper methodss
    std::string getDatabasePath();
    bool createTables();
    bool executeSQL(const std::string& sql);
    void logError(const std::string& operation, const std::string& error);
};

#endif // DATABASEMANAGER_H