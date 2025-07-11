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
#include <QFileInfo>    // For QFileInfo::fileName()
#include <QLabel>       // For a placeholder label in the sidebar
#include <QDockWidget>  // Include for QDockWidget
#include <QToolButton>  // For icon-only buttons in the dock widget
#include <QIcon>        // For setting icons on QToolButton
#include <QSize>        // For specifying icon size
#include <QStyle>       // For standard icons
#include <QStyleOption> // For standard icons

#include "CodeEditor.h"
#include "FileMenuActions.h"

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
    output_horizontal_splitter->addWidget(output_editor);
    output_horizontal_splitter->addWidget(output_examples_editor);
    output_horizontal_splitter->hide();

    // Set the editor_io_splitter as the main content of the central widget's layout
    main_layout->addWidget(editor_io_splitter);

    // Creates the QDockWidget for the sidebar
    QDockWidget *sidebar_dock_widget = new QDockWidget(&main_window);
    sidebar_dock_widget->setAllowedAreas(Qt::LeftDockWidgetArea);
    sidebar_dock_widget->setFeatures(QDockWidget::NoDockWidgetFeatures);

    // Creates a widget to hold content for the dock widget
    QWidget *sidebar_content_widget = new QWidget(sidebar_dock_widget);
    QVBoxLayout *sidebar_layout = new QVBoxLayout(sidebar_content_widget);
    sidebar_layout->setContentsMargins(0, 0, 0, 0);
    sidebar_layout->setSpacing(0);

    QToolButton *toggle_output_examples_button = new QToolButton(sidebar_content_widget);
    QIcon documentIcon = main_window.style()->standardIcon(QStyle::SP_FileIcon);
    toggle_output_examples_button->setIcon(documentIcon);
    toggle_output_examples_button->setIconSize(QSize(32, 32));
    toggle_output_examples_button->setToolTip("Toggle Output Examples Window");
    toggle_output_examples_button->setToolButtonStyle(Qt::ToolButtonIconOnly);

    bool examples_visible = false;

    // Connect the button to toggle the visibility of the output examples
    QObject::connect(toggle_output_examples_button, &QToolButton::clicked, [&]() {
        // Save current sizes to restore later
        QList<int> editor_io_sizes = editor_io_splitter->sizes();
        QList<int> input_output_sizes = input_output_splitter->sizes();
        
        if (!examples_visible) {
            output_editor->setParent(nullptr);
            
            while (output_horizontal_splitter->count() > 0) {
                QWidget *w = output_horizontal_splitter->widget(0);
                w->setParent(nullptr);
            }
            output_horizontal_splitter->addWidget(output_editor);
            output_horizontal_splitter->addWidget(output_examples_editor);
            
            input_output_splitter->addWidget(output_horizontal_splitter);
            output_horizontal_splitter->show();
            
            output_horizontal_splitter->setSizes(QList<int>({1, 1}));
            
            examples_visible = true;
        } else {
            output_horizontal_splitter->setParent(nullptr);
            output_horizontal_splitter->hide();
            
            output_editor->setParent(nullptr);
            
            input_output_splitter->addWidget(output_editor);
            
            examples_visible = false;
        }
        
        input_output_splitter->setSizes(input_output_sizes);
        editor_io_splitter->setSizes(editor_io_sizes);
    });

    sidebar_layout->addWidget(toggle_output_examples_button);
    sidebar_layout->addStretch(1);

    sidebar_dock_widget->setWidget(sidebar_content_widget);

    // Add the dock widget to the main window's left dock area
    main_window.addDockWidget(Qt::LeftDockWidgetArea, sidebar_dock_widget);

    sidebar_dock_widget->setFixedWidth(50);

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