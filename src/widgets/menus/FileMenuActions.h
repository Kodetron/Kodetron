#ifndef FILEMENUACTIONS_H
#define FILEMENUACTIONS_H

#include <QObject>      // Base class for all Qt objects
#include <QMainWindow>  // Main window class
#include <QMenu>        // Menus
#include "../KodetronEditor.h" // Use KodetronEditor as the text editor widget

class QAction;
class QFileDialog;
class QFile;
class QTextStream;
class QMessageBox;
class QFileInfo;

class FileMenuActions : public QObject {
    Q_OBJECT // Required for Qt's meta-object system (signals and slots)

public:

    FileMenuActions(QMainWindow *mainWindow, KodetronEditor *textEditor, KodetronEditor *inputEditor,
                    KodetronEditor *outputEditor, QMenu *fileMenu, QObject *parent = nullptr);

private:
    QMainWindow *m_mainWindow;
    KodetronEditor *m_textEditor;
    KodetronEditor *m_inputEditor;
    KodetronEditor *m_outputEditor;
    QMenu *m_fileMenu;

    void createActions();
};

#endif