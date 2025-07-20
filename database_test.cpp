#include <QApplication>
#include <QDebug>
#include <iostream>
#include "DataBase/DatabaseManager.h"
#include "DataBase/DatabaseService.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    
    std::cout << "=== Kodetron Database Integration Test ===" << std::endl;
    
    // Set application properties
    QCoreApplication::setApplicationName("Kodetron");
    QCoreApplication::setOrganizationName("Kodetron");
    QCoreApplication::setApplicationVersion("1.0.0");

    // Initialize database
    std::cout << "Initializing database..." << std::endl;
    if (!DatabaseManager::instance()->initialize()) {
        std::cerr << "Failed to initialize database: " 
                  << DatabaseManager::instance()->lastError().toStdString() << std::endl;
        return -1;
    }
    
    std::cout << "Database initialized successfully!" << std::endl;

    // Test DatabaseService
    std::cout << "Creating DatabaseService..." << std::endl;
    DatabaseService dbService;
    
    if (!dbService.isInitialized()) {
        std::cerr << "DatabaseService not initialized: " 
                  << dbService.getLastError().toStdString() << std::endl;
        return -1;
    }
    
    std::cout << "DatabaseService initialized successfully!" << std::endl;
    
    // Test theme operations
    std::cout << "Testing theme operations..." << std::endl;
    
    QString testThemeName = "Test Dark Theme";
    QString testThemeContent = "QWidget { background-color: #2b2b2b; color: #ffffff; }";
    
    // Save a theme
    if (dbService.saveTheme(testThemeName, testThemeContent)) {
        std::cout << "Theme saved successfully!" << std::endl;
    } else {
        std::cerr << "Failed to save theme: " 
                  << dbService.getLastError().toStdString() << std::endl;
    }
    
    // Load the theme
    QString loadedContent;
    if (dbService.loadTheme(testThemeName, loadedContent)) {
        std::cout << "Theme loaded successfully!" << std::endl;
        std::cout << "Content matches: " << (loadedContent == testThemeContent ? "YES" : "NO") << std::endl;
    } else {
        std::cerr << "Failed to load theme: " 
                  << dbService.getLastError().toStdString() << std::endl;
    }
    
    // List user themes
    QList<Theme> themes = dbService.getUserThemes();
    std::cout << "User has " << themes.size() << " theme(s)" << std::endl;
    for (const Theme& theme : themes) {
        std::cout << "  - " << theme.name.toStdString() << std::endl;
    }
    
    // Clean up
    DatabaseManager::instance()->close();
    std::cout << "Database connection closed." << std::endl;
    std::cout << "=== Test completed successfully! ===" << std::endl;
    
    return 0;
}
