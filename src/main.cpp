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
    // Set margins to 0 for the main layout to ensure it fills the central widget
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

    // Creates the text editor widget for standard output exmaples
    CodeEditor *output_examples_editor = new CodeEditor(&main_window);
    output_examples_editor->setPlaceholderText("Output Examples");

    // Creates a horizontal splitter for the code editor and input/output sections
    QSplitter *editor_io_splitter = new QSplitter(Qt::Horizontal, central_widget);
    editor_io_splitter->addWidget(text_editor);

    // Creates a vertical splitter for the input and output sections
    QSplitter *input_output_splitter = new QSplitter(Qt::Vertical, editor_io_splitter);
    input_output_splitter->addWidget(input_editor);
    input_output_splitter->addWidget(output_editor);


    // Creates a horizontal splitter for the standard output and standard output examples
    QSplitter *output_main_splitter = new QSplitter(Qt::Vertical, input_output_splitter);
    output_main_splitter->addWidget(output_editor);
    output_main_splitter->addWidget(output_examples_editor);
    output_main_splitter->hide(); // Initially hide the output examples splitter

    // Set the editor_io_splitter as the main content of the central widget's layout
    main_layout->addWidget(editor_io_splitter);


    // Create the QDockWidget for the right sidebar
    QDockWidget *sidebar_dock_widget = new QDockWidget(&main_window);
    sidebar_dock_widget->setAllowedAreas(Qt::LeftDockWidgetArea);
    sidebar_dock_widget->setFeatures(QDockWidget::NoDockWidgetFeatures);

    // Create a widget to hold content for the dock widget
    QWidget *sidebar_content_widget = new QWidget(sidebar_dock_widget);
    QVBoxLayout *sidebar_layout = new QVBoxLayout(sidebar_content_widget);
    sidebar_layout->setContentsMargins(0, 0, 0, 0);
    sidebar_layout->setSpacing(0);

    QToolButton *toggle_output_examples_button = new QToolButton(sidebar_content_widget);
    toggle_output_examples_button->setIcon(QIcon("C:/Users/aleja/OneDrive/Documentos/UNAL/2025_S1/INGESOFT_I/Kodetron/icons/comparison.png"));

    toggle_output_examples_button->setIconSize(QSize(32, 32)); // Set icon size
    toggle_output_examples_button->setToolTip("Toggle Output Examples Window"); // Tooltip for clarity
    toggle_output_examples_button->setToolButtonStyle(Qt::ToolButtonIconOnly); // Ensure only icon is shown

    // Connect the button to toggle the visibility of the output_main_splitter
    QObject::connect(toggle_output_examples_button, &QToolButton::clicked, [&]() {
        if (output_main_splitter->isVisible()) {
            // If visible, hide it and replace output_main_splitter with output_editor
            output_main_splitter->hide(); // Changed line
            // Find the index of output_main_splitter in input_output_splitter
            int index = input_output_splitter->indexOf(output_main_splitter); // Added line
            if (index != -1) { // Added line
                // Remove output_main_splitter (which contains output_editor and output_examples_editor)
                // and re-insert just output_editor at the same position.
                // QSplitter doesn't have removeWidget, so we manage visibility and re-parenting.
                // The trick is to ensure output_editor is always a direct child of input_output_splitter
                // when output_main_splitter is hidden.
                // This approach ensures output_editor remains visible and correctly sized.
                output_main_splitter->setParent(nullptr); // Detach from its current parent
                input_output_splitter->insertWidget(index, output_editor); // Insert output_editor back
                output_editor->show(); // Ensure it's visible
            }
            // Ensure input_editor takes up all space when output_main_splitter is hidden
            input_output_splitter->setSizes(QList<int>() << input_editor->height() << output_editor->height()); // Changed line
        } else {
            // If hidden, show it and insert it into the input_output_splitter
            // First, remove output_editor from input_output_splitter if it's there
            // Find the index of output_editor in input_output_splitter
            int index = input_output_splitter->indexOf(output_editor); // Added line
            if (index != -1) { // Added line
                output_editor->setParent(nullptr); // Detach output_editor from input_output_splitter
                input_output_splitter->insertWidget(index, output_main_splitter); // Insert output_main_splitter
            } else { // Added line
                // If output_editor was somehow not there, just add output_main_splitter
                input_output_splitter->addWidget(output_main_splitter); // Added line
            }
            output_main_splitter->show(); // Changed line
            // Set initial sizes for the newly revealed splitter
            output_main_splitter->setSizes(QList<int>() << output_editor->height() / 2 << output_examples_editor->height() / 2); // Changed line
            // Adjust the main input/output splitter sizes
            input_output_splitter->setSizes(QList<int>() << input_editor->height() / 2 << output_main_splitter->height()); // Changed line
        }
    });

    sidebar_layout->addWidget(toggle_output_examples_button);
    sidebar_layout->addStretch(1);
    sidebar_dock_widget->setWidget(sidebar_content_widget);

    // Add the dock widget to the main window's right dock area
    main_window.addDockWidget(Qt::LeftDockWidgetArea, sidebar_dock_widget);


    sidebar_dock_widget->setFixedWidth(100);

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