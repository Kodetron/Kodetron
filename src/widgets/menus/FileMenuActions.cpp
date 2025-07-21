#include "FileMenuActions.h"

#include <QAction>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QFileInfo>
#include <QApplication>


FileMenuActions::FileMenuActions(QMainWindow *mainWindow, KodetronEditor *textEditor, KodetronEditor *inputEditor,
                                 KodetronEditor *outputEditor, QMenu *fileMenu, QObject *parent)
    : QObject(parent),
      m_mainWindow(mainWindow),
      m_textEditor(textEditor),
      m_inputEditor(inputEditor),
      m_outputEditor(outputEditor),
      m_fileMenu(fileMenu)
{
    // Call the private helper method to create and set up all actions
    createActions();
}

void FileMenuActions::createActions() {
    // New Action: Clears all text editors and resets the window title
    QAction *new_action = new QAction("New", m_mainWindow);
    m_fileMenu->addAction(new_action);
    QObject::connect(new_action, &QAction::triggered, [&]() {
        m_textEditor->clear();
        m_inputEditor->clear();
        m_outputEditor->clear();
        m_mainWindow->setWindowTitle("Kodetron - New Document");
    });

    // Open Action: Opens a file and loads its content into the main text editor
    QAction *open_action = new QAction("Open...", m_mainWindow);
    m_fileMenu->addAction(open_action);
    QObject::connect(open_action, &QAction::triggered, [&]() {
        QString file_name = QFileDialog::getOpenFileName(m_mainWindow, "Open File", QDir::homePath());
        if (!file_name.isEmpty()) {
            QFile file(file_name);
            if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
                QTextStream input_stream(&file);
                m_textEditor->setText(input_stream.readAll());
                file.close();
                m_mainWindow->setWindowTitle("Kodetron - " + QFileInfo(file_name).fileName());
            } else {
                QMessageBox::warning(m_mainWindow, "Error", "Could not open file: " + file.errorString());
            }
        }
    });

    // Save Action: Saves the content of the main text editor to a file
    QAction *save_action = new QAction("Save", m_mainWindow);
    m_fileMenu->addAction(save_action);
    QObject::connect(save_action, &QAction::triggered, [&]() {
        // For simplicity, "Save" will prompt for a file name if not already associated
        // In a real IDE, you'd track the current file path.
        QString file_name = QFileDialog::getSaveFileName(m_mainWindow, "Save File", QDir::homePath());
        if (!file_name.isEmpty()) {
            QFile file(file_name);
            if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
                QTextStream output_stream(&file);
                output_stream << m_textEditor->text();
                file.close();
                m_mainWindow->setWindowTitle("Kodetron - " + QFileInfo(file_name).fileName());
            } else {
                QMessageBox::warning(m_mainWindow, "Error", "Could not save file: " + file.errorString());
            }
        }
    });

    // Save As Action: Always prompts for a new file name to save the content
    QAction *save_as_action = new QAction("Save As...", m_mainWindow);
    m_fileMenu->addAction(save_as_action);
    QObject::connect(save_as_action, &QAction::triggered, [&]() {
        QString file_name = QFileDialog::getSaveFileName(m_mainWindow, "Save File As", QDir::homePath());
        if (!file_name.isEmpty()) {
            QFile file(file_name);
            if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
                QTextStream output_stream(&file);
                output_stream << m_textEditor->text();
                file.close();
                m_mainWindow->setWindowTitle("Kodetron - " + QFileInfo(file_name).fileName());
            } else {
                QMessageBox::warning(m_mainWindow, "Error", "Could not save file: " + file.errorString());
            }
        }
    });

    // Exit Action: Quits the application
    QAction *exit_action = new QAction("Exit", m_mainWindow);
    m_fileMenu->addAction(exit_action);
    QObject::connect(exit_action, &QAction::triggered, &QApplication::quit);
}
