#include <QApplication> // Core application class
#include <QMainWindow>  // Main window class
#include <QTextEdit>    // Text editing widget
#include <QMenuBar>     // Menu bar
#include <QMenu>        // Menus
#include <QAction>      // Actions for menus/toolbars
#include <QFileDialog>  // For opening/saving files
#include <QFile>        // For file I/O
#include <QTextStream>  // For reading/writing textgit 
#include <QMessageBox>  // For displaying messages
#include <QSplitter>    // For creating resizable sections
#include <QVBoxLayout>  // For vertical layout
#include <QFileInfo>    // For QFileInfo::fileName()
#include <QLabel>       // For a placeholder label in the sidebar
#include <QDockWidget>  // Include for QDockWidget
#include <QToolButton>  // For icon-only buttons in the dock widget
#include <QIcon>        // For setting icons on QToolButton
#include <QSize>        // For specifying icon size
#include <QStyle>       // For standard icons
#include <QStyleOption> // For standard icons
#include <QPalette>     // For application palette
#include <QColor>       // For colors

#include "CodeEditor.h"
#include "FileMenuActions.h"
#include "ToolbarManager.h"

void setupDarkTheme(QApplication &app) {
    QPalette darkPalette;
    
    // Set dark colors for the palette
    darkPalette.setColor(QPalette::Window, QColor(53, 53, 53));
    darkPalette.setColor(QPalette::WindowText, QColor(255, 255, 255));
    darkPalette.setColor(QPalette::Base, QColor(25, 25, 25));
    darkPalette.setColor(QPalette::AlternateBase, QColor(53, 53, 53));
    darkPalette.setColor(QPalette::ToolTipBase, QColor(255, 255, 255));
    darkPalette.setColor(QPalette::ToolTipText, QColor(255, 255, 255));
    darkPalette.setColor(QPalette::Text, QColor(255, 255, 255));
    darkPalette.setColor(QPalette::Button, QColor(53, 53, 53));
    darkPalette.setColor(QPalette::ButtonText, QColor(255, 255, 255));
    darkPalette.setColor(QPalette::BrightText, QColor(255, 0, 0));
    darkPalette.setColor(QPalette::Link, QColor(42, 130, 218));
    darkPalette.setColor(QPalette::Highlight, QColor(42, 130, 218));
    darkPalette.setColor(QPalette::HighlightedText, QColor(255, 255, 255));
    
    // Set the application palette
    app.setPalette(darkPalette);
    
    // Set the application style sheet for additional dark styling
    app.setStyleSheet("QToolTip { color: #ffffff; background-color: #2a82da; border: 1px solid white; }");
}

int main(int argc, char *argv[]) {
    // Creates application instance
    QApplication application(argc, argv);
    
    // Apply dark theme to the entire application
    setupDarkTheme(application);

    // Creates the main window for the application
    QMainWindow main_window;
    main_window.setWindowTitle("Kodetron");

    // Creates a central widget to hold the layout
    QWidget *central_widget = new QWidget(&main_window);
    main_window.setCentralWidget(central_widget);

    // Creates a vertical layout for the main window content
    QVBoxLayout *main_layout = new QVBoxLayout(central_widget);
    main_layout->setContentsMargins(0, 0, 0, 0);

    // Creates a text editor widget for coding
    CodeEditor *text_editor = new CodeEditor(&main_window);

    // Creates the text editor widget for standard input
    CodeEditor *input_editor = new CodeEditor(&main_window);
    input_editor->setPlaceholderText("Standard Input");

    // Creates the text editor widget for standard output
    CodeEditor *output_editor = new CodeEditor(&main_window);
    output_editor->setReadOnly(true);
    output_editor->setPlaceholderText("Standard Output");

    // Creates the text editor widget for standard output examples
    CodeEditor *output_examples_editor = new CodeEditor(&main_window);
    output_examples_editor->setPlaceholderText("Output Examples");

    // Creates a horizontal splitter for the code editor and input/output sections
    QSplitter *editor_io_splitter = new QSplitter(Qt::Horizontal, central_widget);
    editor_io_splitter->addWidget(text_editor);

    // Creates a vertical splitter for the input and output sections
    QSplitter *input_output_splitter = new QSplitter(Qt::Vertical, editor_io_splitter);
    input_output_splitter->addWidget(input_editor);
    input_output_splitter->addWidget(output_editor);

    editor_io_splitter->addWidget(input_output_splitter);

    // Create a horizontal splitter to hold standard output and example output
    QSplitter *output_horizontal_splitter = new QSplitter(Qt::Horizontal);
    output_horizontal_splitter->hide();

    // Set the editor_io_splitter as the main content of the central widget's layout
    main_layout->addWidget(editor_io_splitter);

    // Create and setup the toolbar using the ToolbarManager
    ToolbarManager *toolbar_manager = new ToolbarManager(&main_window, 
                                                        output_editor, 
                                                        output_examples_editor, 
                                                        input_output_splitter, 
                                                        output_horizontal_splitter, 
                                                        editor_io_splitter);

    // Creates a menu bar for the main window
    QMenuBar *menu_bar = main_window.menuBar();

    // Creates file menu and adds it to the menu bar
    QMenu *file_menu = menu_bar->addMenu("File");

    FileMenuActions *file_actions = new FileMenuActions(&main_window, text_editor, input_editor, output_editor, file_menu, &main_window);

    main_window.showMaximized();
    main_window.show();

    // Starts the application event loop, this makes the GUI responsive.
    return application.exec();
}