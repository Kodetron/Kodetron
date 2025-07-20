#include <QApplication> // Core application class
#include <QDebug>

#include "MainWindow/MainWindow.h"
#include "DataBase/DatabaseManager.h"

int main(int argc, char *argv[]) {
    // Creates application instance
    QApplication application(argc, argv);
    
    // Set application properties for database path
    QCoreApplication::setApplicationName("Kodetron");
    QCoreApplication::setOrganizationName("Kodetron");
    QCoreApplication::setApplicationVersion("1.0.0");

    // Initialize database
    if (!DatabaseManager::instance()->initialize()) {
        qCritical() << "Failed to initialize database:" << DatabaseManager::instance()->lastError();
        return -1;
    }
    
    qDebug() << "Database initialized successfully";

    // Links the main window for the application
    MainWindow main_window;
    main_window.showMaximized();
    main_window.show();

    // Starts the application event loop, this makes the GUI responsive.
    int result = application.exec();
    
    // Close database connection before exit
    DatabaseManager::instance()->close();
    
    return result;
}
