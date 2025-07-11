#ifndef FILEMENUACTIONS_H
#define FILEMENUACTIONS_H

#include <QObject>      // Base class for all Qt objects
#include <QMainWindow>  // Main window class
#include <QMenu>        // Menus
#include "CodeEditor.h" // Include CodeEditor as it's used for text editor widgets

class QAction;
class QFileDialog;
class QFile;
class QTextStream;
class QMessageBox;
class QFileInfo;

class FileMenuActions : public QObject {
    Q_OBJECT // Required for Qt's meta-object system (signals and slots)

public:

    FileMenuActions(QMainWindow *mainWindow, CodeEditor *textEditor, CodeEditor *inputEditor,
                    CodeEditor *outputEditor, QMenu *fileMenu, QObject *parent = nullptr);

private:
    QMainWindow *m_mainWindow;
    CodeEditor *m_textEditor;
    CodeEditor *m_inputEditor;
    CodeEditor *m_outputEditor;
    QMenu *m_fileMenu;

    void createActions();
};

#endif