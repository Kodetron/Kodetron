#include "DatabaseManager.h"
#include <QApplication>
#include <QDebug>
#include <QDateTime>

// Example usage of DatabaseManager in Kodetron
void setupDatabaseExample()
{
    // Create database manager instance
    DatabaseManager *dbManager = new DatabaseManager();
    
    // Initialize database (creates kodetron.db in current directory)
    if (!dbManager->initialize()) {
        qDebug() << "Failed to initialize database:" << dbManager->getLastError();
        return;
    }
    
    qDebug() << "Database initialized successfully!";
    
    // Example 1: Create a user
    int userId = dbManager->createUser("coder123", "coder123@example.com");
    if (userId > 0) {
        qDebug() << "Created user with ID:" << userId;
        
        // Example 2: Set user settings
        dbManager->setSetting(userId, "theme", "dark");
        dbManager->setSetting(userId, "font_size", "12");
        dbManager->setSetting(userId, "auto_save", "true");
        
        // Example 3: Create a code template
        QString templateContent = R"(
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main() {
    // Your code here
    return 0;
}
)";
        
        int templateId = dbManager->createTemplate(userId, "Basic C++", templateContent);
        if (templateId > 0) {
            qDebug() << "Created template with ID:" << templateId;
        }
        
        // Example 4: Create a Codeforces account
        int cfAccountId = dbManager->createCodeforcesAccount(userId, "coder123", "api_key_here", "api_secret_here");
        if (cfAccountId > 0) {
            qDebug() << "Created Codeforces account with ID:" << cfAccountId;
            
            // Example 5: Create a contest
            QDateTime startTime = QDateTime::currentDateTime().addDays(1);
            QDateTime endTime = startTime.addSecs(7200); // 2 hours
            
            int contestId = dbManager->createContest(userId, cfAccountId, "Practice Contest", startTime, endTime);
            if (contestId > 0) {
                qDebug() << "Created contest with ID:" << contestId;
            }
        }
        
        // Example 6: Create a code snippet
        QString snippetBody = R"(
for (int i = 0; i < n; i++) {
    // Loop body
}
)";
        
        int snippetId = dbManager->createSnippet(userId, "For Loop", "for", snippetBody, "Standard for loop");
        if (snippetId > 0) {
            qDebug() << "Created snippet with ID:" << snippetId;
        }
        
        // Example 7: Create a custom theme
        QString themeQSS = R"(
QMainWindow {
    background-color: #1e1e1e;
    color: #ffffff;
}

QPlainTextEdit {
    background-color: #1e1e1e;
    color: #d4d4d4;
    border: none;
}
)";
        
        int themeId = dbManager->createTheme(userId, "Dark Theme", themeQSS);
        if (themeId > 0) {
            qDebug() << "Created theme with ID:" << themeId;
        }
        
        // Example 8: Retrieve and display user data
        qDebug() << "\n=== User Data ===";
        QVariantMap user = dbManager->getUser(userId);
        qDebug() << "User:" << user["username"].toString() << "(" << user["email"].toString() << ")";
        
        qDebug() << "\n=== User Settings ===";
        QVariantMap settings = dbManager->getAllSettings(userId);
        for (auto it = settings.begin(); it != settings.end(); ++it) {
            qDebug() << it.key() << ":" << it.value().toString();
        }
        
        qDebug() << "\n=== Templates ===";
        QList<QVariantMap> templates = dbManager->getTemplates(userId);
        for (const QVariantMap &template_ : templates) {
            qDebug() << "Template:" << template_["name"].toString();
        }
        
        qDebug() << "\n=== Codeforces Accounts ===";
        QList<QVariantMap> accounts = dbManager->getCodeforcesAccounts(userId);
        for (const QVariantMap &account : accounts) {
            qDebug() << "Handle:" << account["handle"].toString();
        }
        
        qDebug() << "\n=== Contests ===";
        QList<QVariantMap> contests = dbManager->getContests(userId);
        for (const QVariantMap &contest : contests) {
            qDebug() << "Contest:" << contest["name"].toString() 
                     << "starts at" << contest["start_time"].toDateTime().toString();
        }
        
        qDebug() << "\n=== Snippets ===";
        QList<QVariantMap> snippets = dbManager->getSnippets(userId);
        for (const QVariantMap &snippet : snippets) {
            qDebug() << "Snippet:" << snippet["name"].toString() 
                     << "(" << snippet["prefix"].toString() << ")";
        }
        
        qDebug() << "\n=== Themes ===";
        QList<QVariantMap> themes = dbManager->getThemes(userId);
        for (const QVariantMap &theme : themes) {
            qDebug() << "Theme:" << theme["name"].toString();
        }
        
    } else {
        qDebug() << "Failed to create user:" << dbManager->getLastError();
    }
    
    // Clean up
    delete dbManager;
}

// Example of how to integrate DatabaseManager into main.cpp
void integrateDatabaseIntoMain()
{
    // In your main.cpp, you would add:
    /*
    #include "database/DatabaseManager.h"
    
    int main(int argc, char *argv[]) {
        QApplication app(argc, argv);
        
        // Initialize database
        DatabaseManager *dbManager = new DatabaseManager(&app);
        if (!dbManager->initialize()) {
            qDebug() << "Database initialization failed:" << dbManager->getLastError();
            return -1;
        }
        
        // Create main window and pass database manager
        MainWindow mainWindow;
        mainWindow.setDatabaseManager(dbManager);
        
        // ... rest of your application setup
        
        return app.exec();
    }
    */
} 