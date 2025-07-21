#include "DatabaseManager.h"
#include <iostream>
#include <QCoreApplication>
#include <QStandardPaths>
#include <QDir>

DatabaseManager::DatabaseManager() : db(nullptr) {
    db_path = getDatabasePath();
}

DatabaseManager::~DatabaseManager() {
    if (db) {
        sqlite3_close(db);
    }
}

std::string DatabaseManager::getDatabasePath() {
    // Use Qt to get a portable application data directory
    QString appDataDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir().mkpath(appDataDir); // Create directory if it doesn't exist
    
    QString dbPath = appDataDir + "/kodetron.db";
    return dbPath.toStdString();
}

bool DatabaseManager::initializeDatabase() {
    int rc = sqlite3_open(db_path.c_str(), &db);
    
    if (rc != SQLITE_OK) {
        logError("Database opening", sqlite3_errmsg(db));
        return false;
    }
    
    // Enable foreign key constraints
    if (!executeSQL("PRAGMA foreign_keys = ON;")) {
        return false;
    }
    
    return createTables();
}

bool DatabaseManager::isDatabaseInitialized() const {
    return db != nullptr;
}

bool DatabaseManager::createTables() {
    // Create Settings table first (referenced by Users)
    std::string createSettings = R"(
        CREATE TABLE IF NOT EXISTS Settings (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            name VARCHAR NOT NULL
        );
    )";
    
    if (!executeSQL(createSettings)) {
        return false;
    }
    
    // Create Users table
    std::string createUsers = R"(
        CREATE TABLE IF NOT EXISTS Users (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            codeforces_handle VARCHAR,
            email VARCHAR,
            settings_id INTEGER,
            FOREIGN KEY (settings_id) REFERENCES Settings(id) ON DELETE SET NULL
        );
    )";
    
    if (!executeSQL(createUsers)) {
        return false;
    }
    
    // Create Templates table
    std::string createTemplates = R"(
        CREATE TABLE IF NOT EXISTS Templates (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            name VARCHAR NOT NULL,
            content VARCHAR NOT NULL,
            user_id INTEGER,
            FOREIGN KEY (user_id) REFERENCES Users(id) ON DELETE CASCADE
        );
    )";
    
    if (!executeSQL(createTemplates)) {
        return false;
    }
    
    // Create Snippets table
    std::string createSnippets = R"(
        CREATE TABLE IF NOT EXISTS Snippets (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            name VARCHAR NOT NULL UNIQUE,
            content VARCHAR NOT NULL,
            user_id INTEGER,
            FOREIGN KEY (user_id) REFERENCES Users(id) ON DELETE CASCADE
        );
    )";
    
    return executeSQL(createSnippets);
}

bool DatabaseManager::executeSQL(const std::string& sql) {
    char* errorMessage = nullptr;
    int rc = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errorMessage);
    
    if (rc != SQLITE_OK) {
        logError("SQL execution", errorMessage ? errorMessage : "Unknown error");
        if (errorMessage) {
            sqlite3_free(errorMessage);
        }
        return false;
    }
    
    return true;
}

void DatabaseManager::logError(const std::string& operation, const std::string& error) {
    std::cerr << "Database error during " << operation << ": " << error << std::endl;
}

// User operations
bool DatabaseManager::createUser(const std::string& codeforces_handle, const std::string& email) {
    const char* sql = "INSERT INTO Users (codeforces_handle, email) VALUES (?, ?);";
    sqlite3_stmt* stmt;
    
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        logError("Preparing user creation", sqlite3_errmsg(db));
        return false;
    }
    
    sqlite3_bind_text(stmt, 1, codeforces_handle.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, email.c_str(), -1, SQLITE_STATIC);
    
    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    
    if (rc != SQLITE_DONE) {
        logError("Creating user", sqlite3_errmsg(db));
        return false;
    }
    
    return true;
}

bool DatabaseManager::getUserById(int id, User& user) {
    const char* sql = "SELECT id, codeforces_handle, email, settings_id FROM Users WHERE id = ?;";
    sqlite3_stmt* stmt;
    
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        logError("Preparing user query", sqlite3_errmsg(db));
        return false;
    }
    
    sqlite3_bind_int(stmt, 1, id);
    
    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        user.id = sqlite3_column_int(stmt, 0);
        
        const char* handle = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        user.codeforces_handle = handle ? handle : "";
        
        const char* email_text = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        user.email = email_text ? email_text : "";
        
        user.settings_id = sqlite3_column_int(stmt, 3);
        
        sqlite3_finalize(stmt);
        return true;
    }
    
    sqlite3_finalize(stmt);
    return false;
}

bool DatabaseManager::updateUser(const User& user) {
    const char* sql = "UPDATE Users SET codeforces_handle = ?, email = ?, settings_id = ? WHERE id = ?;";
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        logError("Preparing user update", sqlite3_errmsg(db));
        return false;
    }
    sqlite3_bind_text(stmt, 1, user.codeforces_handle.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, user.email.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 3, user.settings_id);
    sqlite3_bind_int(stmt, 4, user.id);

    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    return rc == SQLITE_DONE;
}

bool DatabaseManager::deleteUser(int id) {
    const char* sql = "DELETE FROM Users WHERE id = ?;";
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        logError("Preparing user deletion", sqlite3_errmsg(db));
        return false;
    }
    sqlite3_bind_int(stmt, 1, id);

    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    return rc == SQLITE_DONE;
}

std::vector<User> DatabaseManager::getAllUsers() {
    std::vector<User> users;
    const char* sql = "SELECT id, codeforces_handle, email, settings_id FROM Users;";
    sqlite3_stmt* stmt;
    
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        logError("Preparing users query", sqlite3_errmsg(db));
        return users;
    }
    
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        User user;
        user.id = sqlite3_column_int(stmt, 0);
        
        const char* handle = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        user.codeforces_handle = handle ? handle : "";
        
        const char* email_text = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        user.email = email_text ? email_text : "";
        
        user.settings_id = sqlite3_column_int(stmt, 3);
        
        users.push_back(user);
    }
    
    sqlite3_finalize(stmt);
    return users;
}

// Template operations
bool DatabaseManager::createTemplate(const std::string& name, const std::string& content, int user_id) {
    const char* sql = "INSERT INTO Templates (name, content, user_id) VALUES (?, ?, ?);";
    sqlite3_stmt* stmt;
    
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        logError("Preparing template creation", sqlite3_errmsg(db));
        return false;
    }
    
    sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, content.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 3, user_id);
    
    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    
    return rc == SQLITE_DONE;
}

bool DatabaseManager::getTemplateById(int id, Template& template_obj) {
    const char* sql = "SELECT id, name, content, user_id FROM Templates WHERE id = ?;";
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        logError("Preparing template query", sqlite3_errmsg(db));
        return false;
    }
    sqlite3_bind_int(stmt, 1, id);

    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        template_obj.id = sqlite3_column_int(stmt, 0);
        const char* name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        template_obj.name = name ? name : "";
        const char* content = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        template_obj.content = content ? content : "";
        template_obj.user_id = sqlite3_column_int(stmt, 3);
        sqlite3_finalize(stmt);
        return true;
    }
    sqlite3_finalize(stmt);
    return false;
}

std::vector<Template> DatabaseManager::getTemplatesByUserId(int user_id) {
    std::vector<Template> templates;
    const char* sql = "SELECT id, name, content, user_id FROM Templates WHERE user_id = ?;";
    sqlite3_stmt* stmt;
    
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        logError("Preparing templates query", sqlite3_errmsg(db));
        return templates;
    }
    
    sqlite3_bind_int(stmt, 1, user_id);
    
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        Template tmpl;
        tmpl.id = sqlite3_column_int(stmt, 0);
        
        const char* name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        tmpl.name = name ? name : "";
        
        const char* content = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        tmpl.content = content ? content : "";
        
        tmpl.user_id = sqlite3_column_int(stmt, 3);
        
        templates.push_back(tmpl);
    }
    
    sqlite3_finalize(stmt);
    return templates;
}

bool DatabaseManager::updateTemplate(const Template& template_obj) {
    const char* sql = "UPDATE Templates SET name = ?, content = ? WHERE id = ?;";
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        logError("Preparing template update", sqlite3_errmsg(db));
        return false;
    }
    sqlite3_bind_text(stmt, 1, template_obj.name.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, template_obj.content.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 3, template_obj.id);

    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    return rc == SQLITE_DONE;
}

bool DatabaseManager::deleteTemplate(int id) {
    const char* sql = "DELETE FROM Templates WHERE id = ?;";
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        logError("Preparing template deletion", sqlite3_errmsg(db));
        return false;
    }
    sqlite3_bind_int(stmt, 1, id);

    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    return rc == SQLITE_DONE;
}

// Snippet operations
bool DatabaseManager::createSnippet(const std::string& name, const std::string& content, int user_id) {
    const char* sql = "INSERT INTO Snippets (name, content, user_id) VALUES (?, ?, ?);";
    sqlite3_stmt* stmt;
    
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        logError("Preparing snippet creation", sqlite3_errmsg(db));
        return false;
    }
    
    sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, content.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 3, user_id);
    
    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    
    return rc == SQLITE_DONE;
}

bool DatabaseManager::getSnippetById(int id, Snippet& snippet) {
    const char* sql = "SELECT id, name, content, user_id FROM Snippets WHERE id = ?;";
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        logError("Preparing snippet query", sqlite3_errmsg(db));
        return false;
    }
    sqlite3_bind_int(stmt, 1, id);

    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        snippet.id = sqlite3_column_int(stmt, 0);
        const char* name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        snippet.name = name ? name : "";
        const char* content = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        snippet.content = content ? content : "";
        snippet.user_id = sqlite3_column_int(stmt, 3);
        sqlite3_finalize(stmt);
        return true;
    }
    sqlite3_finalize(stmt);
    return false;
}

std::vector<Snippet> DatabaseManager::getSnippetsByUserId(int user_id) {
    std::vector<Snippet> snippets;
    const char* sql = "SELECT id, name, content, user_id FROM Snippets WHERE user_id = ?;";
    sqlite3_stmt* stmt;
    
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        logError("Preparing snippets query", sqlite3_errmsg(db));
        return snippets;
    }
    
    sqlite3_bind_int(stmt, 1, user_id);
    
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        Snippet snippet;
        snippet.id = sqlite3_column_int(stmt, 0);
        
        const char* name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        snippet.name = name ? name : "";
        
        const char* content = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        snippet.content = content ? content : "";
        
        snippet.user_id = sqlite3_column_int(stmt, 3);
        
        snippets.push_back(snippet);
    }
    
    sqlite3_finalize(stmt);
    return snippets;
}

bool DatabaseManager::updateSnippet(const Snippet& snippet) {
    const char* sql = "UPDATE Snippets SET name = ?, content = ? WHERE id = ?;";
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        logError("Preparing snippet update", sqlite3_errmsg(db));
        return false;
    }
    sqlite3_bind_text(stmt, 1, snippet.name.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, snippet.content.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 3, snippet.id);

    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    return rc == SQLITE_DONE;
}

bool DatabaseManager::deleteSnippet(int id) {
    const char* sql = "DELETE FROM Snippets WHERE id = ?;";
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        logError("Preparing snippet deletion", sqlite3_errmsg(db));
        return false;
    }
    sqlite3_bind_int(stmt, 1, id);

    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    return rc == SQLITE_DONE;
}

// Settings operations
bool DatabaseManager::createSettings(const std::string& name) {
    const char* sql = "INSERT INTO Settings (name) VALUES (?);";
    sqlite3_stmt* stmt;
    
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        logError("Preparing settings creation", sqlite3_errmsg(db));
        return false;
    }
    
    sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_STATIC);
    
    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    
    return rc == SQLITE_DONE;
}

bool DatabaseManager::getSettingsById(int id, Settings& settings) {
    const char* sql = "SELECT id, name FROM Settings WHERE id = ?;";
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        logError("Preparing settings query", sqlite3_errmsg(db));
        return false;
    }
    sqlite3_bind_int(stmt, 1, id);

    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        settings.id = sqlite3_column_int(stmt, 0);
        const char* name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        settings.name = name ? name : "";
        sqlite3_finalize(stmt);
        return true;
    }
    sqlite3_finalize(stmt);
    return false;
}

bool DatabaseManager::updateSettings(const Settings& settings) {
    const char* sql = "UPDATE Settings SET name = ? WHERE id = ?;";
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        logError("Preparing settings update", sqlite3_errmsg(db));
        return false;
    }
    sqlite3_bind_text(stmt, 1, settings.name.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 2, settings.id);

    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    return rc == SQLITE_DONE;
}

bool DatabaseManager::deleteSettings(int id) {
    const char* sql = "DELETE FROM Settings WHERE id = ?;";
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        logError("Preparing settings deletion", sqlite3_errmsg(db));
        return false;
    }
    sqlite3_bind_int(stmt, 1, id);

    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    return rc == SQLITE_DONE;
}