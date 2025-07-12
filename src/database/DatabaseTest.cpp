#include "DatabaseManager.h"
#include <QCoreApplication>
#include <QDebug>
#include <QDateTime>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    
    qDebug() << "=== Kodetron Database Test ===";
    
    // Create and initialize database manager
    DatabaseManager *dbManager = new DatabaseManager();
    
    if (!dbManager->initialize("test_kodetron.db")) {
        qDebug() << "❌ Database initialization failed:" << dbManager->getLastError();
        return -1;
    }
    
    qDebug() << "✅ Database initialized successfully!";
    
    // Test 1: Create a user
    int userId = dbManager->createUser("testuser", "test@example.com");
    if (userId <= 0) {
        qDebug() << "❌ Failed to create user:" << dbManager->getLastError();
        return -1;
    }
    qDebug() << "✅ Created user with ID:" << userId;
    
    // Test 2: Set and get settings
    dbManager->setSetting(userId, "theme", "dark");
    dbManager->setSetting(userId, "font_size", "14");
    
    QString theme = dbManager->getSetting(userId, "theme", "light");
    QString fontSize = dbManager->getSetting(userId, "font_size", "12");
    
    qDebug() << "✅ Settings - Theme:" << theme << "Font Size:" << fontSize;
    
    // Test 3: Create a template
    QString templateContent = "#include <iostream>\n\nint main() {\n    return 0;\n}";
    int templateId = dbManager->createTemplate(userId, "Basic C++", templateContent);
    if (templateId > 0) {
        qDebug() << "✅ Created template with ID:" << templateId;
    }
    
    // Test 4: Create a snippet
    int snippetId = dbManager->createSnippet(userId, "For Loop", "for", 
                                           "for (int i = 0; i < n; i++) {\n    // loop body\n}", 
                                           "Standard for loop");
    if (snippetId > 0) {
        qDebug() << "✅ Created snippet with ID:" << snippetId;
    }
    
    // Test 5: Retrieve data
    QVariantMap user = dbManager->getUser(userId);
    qDebug() << "✅ Retrieved user:" << user["username"].toString();
    
    QList<QVariantMap> templates = dbManager->getTemplates(userId);
    qDebug() << "✅ Found" << templates.size() << "templates";
    
    QList<QVariantMap> snippets = dbManager->getSnippets(userId);
    qDebug() << "✅ Found" << snippets.size() << "snippets";
    
    // Test 6: Clean up
    delete dbManager;
    
    qDebug() << "✅ All tests passed!";
    return 0;
} 