#include <QWidget>      // Base class for all user interface objects
#include <QApplication> // Core application class
#include <QMainWindow>  // Main window class
#include <QTextEdit>    // Text editing widget
#include <QMenuBar>     // Menu bar
#include <QMenu>        // Menus
#include <QAction>      // Actions for menus/toolbars
#include <QFileDialog>  // For opening/saving files
#include <QFile>        // For file I/O
#include <QTextStream>  // For reading/writing text
#include <QMessageBox>  // For displaying messages
#include <QSplitter>    // For creating resizable sections
#include <QVBoxLayout>  // For vertical layout

int main(int argc, char *argv[]) {
    // Creates application instance   
    QApplication application(argc, argv);

    // Creates the main window for the application
    QMainWindow main_window;
    main_window.setWindowTitle("Kodetron");

    // Creates a central widget to hold the layout
    QWidget *central_widget = new QWidget(&main_window);
    main_window.setCentralWidget(central_widget);

    // Creates a vertical layout for the main window content
    QVBoxLayout *main_layout = new QVBoxLayout(central_widget);

    // Creates a text editor widget that allows users to edit text
    QT  extEdit *text_editor = new QTextEdit(&main_window);

    // Creates the text editor widget for standard input
    QTextEdit *input_editor = new QTextEdit(&main_window);

    // Creates the text editor widget for standard output (read-only)
    QTextEdit *output_editor = new QTextEdit(&main_window);
    output_editor->setReadOnly(true);

    // Creates a horizontal splitter for the code editor and input/output sections
    QSplitter *horizontal_splitter = new QSplitter(Qt::Horizontal, central_widget);
    horizontal_splitter->addWidget(text_editor);

    // Creates a vertical splitter for the input and output sections
    QSplitter *vertical_splitter = new QSplitter(Qt::Vertical, horizontal_splitter);
    vertical_splitter->addWidget(input_editor);
    vertical_splitter->addWidget(output_editor);

    horizontal_splitter->addWidget(vertical_splitter);

    main_layout->addWidget(horizontal_splitter);

    // Creates a menu bar for the main window
    QMenuBar *menu_bar = main_window.menuBar();

    // Creates file menu and adds it to the menu bar
    QMenu *file_menu = menu_bar->addMenu("File");

    // Creates different actions for the file menu
    // New Action
    QAction *new_action = new QAction("New", &main_window);
    file_menu->addAction(new_action);
    QObject::connect(new_action, &QAction::triggered, [&]() {
        text_editor->clear();
        input_editor->clear();
        output_editor->clear();
        main_window.setWindowTitle("Kodetron - New Document");
    });

    // Open Action (will only open into the code editor for now)
    QAction *open_action = new QAction("Open...", &main_window);
    file_menu->addAction(open_action);
    QObject::connect(open_action, &QAction::triggered, [&]() {
        QString file_name = QFileDialog::getOpenFileName(&main_window, "Open File", QDir::homePath());
        if (!file_name.isEmpty()) {
            QFile file(file_name);
            if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
                QTextStream input_stream(&file);
                text_editor->setText(input_stream.readAll());
                file.close();
                main_window.setWindowTitle("Kodetron - " + QFileInfo(file_name).fileName());
            } else {
                QMessageBox::warning(&main_window, "Error", "Could not open file: " + file.errorString());
            }
        }
    });

    // Save Action (will only save from the code editor for now)
    QAction *save_action = new QAction("Save", &main_window);
    file_menu->addAction(save_action);
    QObject::connect(save_action, &QAction::triggered, [&]() {
        QString file_name = QFileDialog::getSaveFileName(&main_window, "Save File", QDir::homePath());
        if (!file_name.isEmpty()) {
            QFile file(file_name);
            if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
                QTextStream output_stream(&file);
                output_stream << text_editor->toPlainText();
                file.close();
                main_window.setWindowTitle("Kodetron - " + QFileInfo(file_name).fileName());
            } else {
                QMessageBox::warning(&main_window, "Error", "Could not save file: " + file.errorString());
            }
        }
    });

    // Save As Action (will only save from the code editor for now)
    QAction *save_as_action = new QAction("Save As...", &main_window);
    file_menu->addAction(save_as_action);
    QObject::connect(save_as_action, &QAction::triggered, [&]() {
        QString file_name = QFileDialog::getSaveFileName(&main_window, "Save File As", QDir::homePath());
        if (!file_name.isEmpty()) {
            QFile file(file_name);
            if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
                QTextStream output_stream(&file);
                output_stream << text_editor->toPlainText();
                file.close();
                main_window.setWindowTitle("Kodetron - " + QFileInfo(file_name).fileName());
            } else {
                QMessageBox::warning(&main_window, "Error", "Could not save file: " + file.errorString());
            }
        }
    });

    // Exit Action
    QAction *exit_action = new QAction("Exit", &main_window);
    file_menu->addAction(exit_action);
    QObject::connect(exit_action, &QAction::triggered, &QApplication::quit);

    main_window.showMaximized();
    main_window.show();

    // Starts the application event loop, this makes the GUI responsive.
    return application.exec();
}