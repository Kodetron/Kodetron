#include "MainWindow.h"
#include "../App/App.h"
#include "../utils/StyleLoader/StyleReader.h"

#include <iostream>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setWindowTitle("Kodetron");
    setMinimumSize(500, 350);
    setWindowIcon(QIcon("../assets/Logo.png"));

    setObjectName("main_window");

    App *app = new App(this);
    setCentralWidget(app);

    loadStyleSheet();
}
void MainWindow::loadStyleSheet() {
    QString styleSheet = StyleLoader::read("../src/MainWindow/MainWindow.qss");
    if (!styleSheet.isEmpty()) {
        setStyleSheet(styleSheet);
    }
}
