#include <QApplication> // Core application class

#include "MainWindow/MainWindow.h"

int main(int argc, char *argv[]) {
    // Creates application instance
    QApplication application(argc, argv);

    // Links the main window for the application
    MainWindow main_window;
    main_window.showMaximized();
    main_window.show();

    // Starts the application event loop, this makes the GUI responsive.
    return application.exec();
}
